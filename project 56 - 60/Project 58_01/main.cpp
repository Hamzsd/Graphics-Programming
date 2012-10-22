#pragma comment(lib, "GLFWDLL")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "glew32")

#define GLFW_DLL

#include <glew.h>
#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\constants.hpp>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "render_object.h"
#include "effect.h"
#include "light.h"
#include "scene.h"
#include "camera.h"

bool running = true;

effect eff;
scene_data* scene;
first_person_camera* cam;


glm::vec3 lightAim(0.0f, 0.0f, 0.0f);
float power = 1.0f;

void initialise()
{
	srand(time(NULL));
		
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	//Target Camera
	/*cam = new target_camera();
	cam->setProjection(glm::pi<float>() / 4.0f, 800.0f/600.0f, 0.1f, 10000.0f);
	cam->setPositon(glm::vec3(10.f, 10.0f, 10.0f));
	cam->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));*/

	//FPS camera
	cam = new first_person_camera();
	cam->setProjection(glm::pi<float>() / 4.0f, 800.0f/600.0f, 0.1f, 10000.0f);
	cam->setPositon(glm::vec3(0.0f, 2.0f, 0.0f));

	if (!eff.addShader("multi_light.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("multi_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);

	scene = loadScene("scene.json");
}

void update(double deltaTime)
{
	float speed = 5.0f;
	cam->update(deltaTime);
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	if (glfwGetKey('O'))
		power = std::max<float>(power - 0.1f, 0.0f);
	if (glfwGetKey('P'))
		power += 0.1f;
	if (glfwGetKey(GLFW_KEY_UP))
		lightAim.z -= 0.1f;
	if (glfwGetKey(GLFW_KEY_DOWN))
		lightAim.z += 0.1f;
	//if (glfwGetKey(GLFW_KEY_LEFT))
	//	lightAim.x -= 0.1f;
	//if (glfwGetKey(GLFW_KEY_RIGHT))
	//	lightAim.x += 0.1f;
	if (glfwGetKey('W'))
		cam->move(glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('S'))
		cam->move(-glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('A'))
		cam->move(glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey('D'))
		cam->move(-glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey(GLFW_KEY_LEFT))
		cam->rotate(glm::pi<float>() * deltaTime, 0.0f);
	if (glfwGetKey(GLFW_KEY_RIGHT))
		cam->rotate(-glm::pi<float>() * deltaTime, 0.0f);
}

void render(const effect* eff, const glm::mat4 view, const glm::mat4& projection, const render_object& object)
{
	glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	glm::mat4 mit = glm::inverse(glm::transpose(object.transform.getTransformationMatrix()));
	glUniformMatrix4fv(eff->getUniformIndex("modelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(mit));
	glUniformMatrix4fv(eff->getUniformIndex("model"), 1, GL_FALSE, glm::value_ptr(object.transform.getTransformationMatrix()));

	object.material->bind(eff);

	glBindVertexArray(object.geometry->vao);
	if (object.geometry->indexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.geometry->indexBuffer);
		glDrawElements(GL_TRIANGLES, object.geometry->indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, object.geometry->vertices.size());
	glBindVertexArray(0);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	eff.begin();

	// Create light direction from lightAngle
	spot_light_data spot = scene->dynamic.data.spots[0];
	spot.lightDir = glm::vec4(lightAim, 1.0f) - spot.lightPos;
	spot.lightDir = glm::normalize(spot.lightDir);
	spot.power = power;
	// Set the light
	scene->dynamic.setSpot(0, spot); //here the buffer is 0 so spotlight doesnt get set......

	scene->dynamic.bind(&eff);
	scene->light.bind(&eff);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(cam->getPosition()));

	std::hash_map<std::string, render_object*>::const_iterator iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, cam->getView(), cam->getProjecion(), *iter->second);

	glUseProgram(0);
	glfwSwapBuffers();
}

void cleanup()
{
}

int main()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	if (!glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "Error: " << glewGetErrorString(error) <<std::endl;
		exit(EXIT_FAILURE);
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major , minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL VENDOR : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL version : %s\n", glslVersion);

	initialise();

	double prevTimeStamp = glfwGetTime();
	double currentTimeStamp;
	while (running)
	{
		currentTimeStamp = glfwGetTime();
		update(currentTimeStamp - prevTimeStamp);
		render();
		prevTimeStamp = currentTimeStamp;
	}

	cleanup();

	glfwTerminate();

	exit(EXIT_SUCCESS);

}