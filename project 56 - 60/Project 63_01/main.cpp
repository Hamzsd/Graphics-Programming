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
chase_camera* cam1;
arc_ball_camera* cam2;
target_camera* cam3;
first_person_camera* cam4;
camera* currentCam;

glm::vec3 lightAim(0.0f, 0.0f, 0.0f);
float power = 1.0f;

void initialise()
{
	srand(time(NULL));
		
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	//Target Camera
	cam3 = new target_camera();
	cam3->setProjection(glm::pi<float>() / 4.0f, 800.0f/600.0f, 0.1f, 10000.0f);
	cam3->setPositon(glm::vec3(10.f, 10.0f, 10.0f));
	cam3->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

	//FPS camera
	cam4 = new first_person_camera();
	cam4->setProjection(glm::pi<float>() / 4.0f, 800.0f/600.0f, 0.1f, 10000.0f);
	cam4->setPositon(glm::vec3(0.0f, 2.0f, 0.0f));

	//arcBall
	cam2 = new arc_ball_camera();
	cam2->setProjection(45.0f, 800.0f/600.0f, 0.1f, 10000.0f);
	cam2->setTarget(glm::vec3(0.0f, 2.0f, 0.0f));
	cam2->setDistance(10.0f);
	cam2->setMinRotationY(-glm::pi<float>() / 2.0f);
	cam2->setMaxRotationY(glm::pi<float>() / 2.0f);
	cam2->setRotationX(0.0f);
	cam2->setRotationY(0.0f);

	//chaseCamera
	cam1 = new chase_camera();
	cam1->setProjection(45.0f, 800.0f/600.0f, 0.1f, 10000.0f);
	cam1->setFollowPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	cam1->setPositionOffset(glm::vec3(10.0f, 0.0f, 10.0f));
	cam1->setSpringiness(0.5f);

	currentCam = cam3;

	if (!eff.addShader("multi_light.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("multi_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);

	scene = loadScene("StackOfCubes.json");
	//scene = loadScene("scene2.json");
}

//=================Move camera methods=======================================
void moveChaseCam()
{
	if (glfwGetKey('W'))
		cam1->rotate(glm::vec3(glm::pi<float>() / 100.0f, 0.0f, 0.0f));
	if (glfwGetKey('S'))
		cam1->rotate(glm::vec3(-glm::pi<float>() / 100.0f, 0.0f, 0.0f));
	if (glfwGetKey('A'))
		cam1->rotate(glm::vec3(0.0f, -glm::pi<float>() / 100.0f, 0.0f));
	if (glfwGetKey('D'))
		cam1->rotate(glm::vec3(0.0f, glm::pi<float>() / 100.0f, 0.0f));
}

void moveArcCam()
{
	if (glfwGetKey('W'))
		cam2->rotate(glm::pi<float>() / 100.0f, 0.0f);
	if (glfwGetKey('S'))
		cam2->rotate(-glm::pi<float>() / 100.0f, 0.0f);
	if (glfwGetKey('A'))
		cam2->rotate(0.0f, glm::pi<float>() / 100.0f);
	if (glfwGetKey('D'))
		cam2->rotate(0.0f, -glm::pi<float>() / 100.0f);
}

void moveFPSCam(float deltaTime, float speed)
{
	//fps cam controls
	if (glfwGetKey('W'))
		cam4->move(glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('S'))
		cam4->move(-glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('A'))
		cam4->move(glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey('D'))
		cam4->move(-glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey(GLFW_KEY_LEFT))
		cam4->rotate(glm::pi<float>() * deltaTime, 0.0f);
	if (glfwGetKey(GLFW_KEY_RIGHT))
		cam4->rotate(-glm::pi<float>() * deltaTime, 0.0f);
}
//================================================================================
void update(double deltaTime)
{
	float speed = 5.0f;

	currentCam->update(deltaTime);
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	
	if (glfwGetKey(GLFW_KEY_KP_1))//selecte chase cam
		currentCam = cam1;
	if (currentCam == cam1)
		moveChaseCam();

	if (glfwGetKey(GLFW_KEY_KP_2))//select arc cam
		currentCam = cam2;
	if (currentCam == cam2)
		moveArcCam();

	if (glfwGetKey(GLFW_KEY_KP_3))//select target
		currentCam = cam3;

	if (glfwGetKey(GLFW_KEY_KP_4))//select fps
		currentCam = cam4;
	if (currentCam == cam4)
		moveFPSCam(deltaTime, speed);

	if(glfwGetKey(GLFW_KEY_SPACE))
	{
		scene->objects["cube1"]->transform.move(glm::vec3(0.0f, 0.01f, 0.1f));
		scene->objects["cube2"]->transform.move(glm::vec3(0.1f, 0.01f, 0.1));
		scene->objects["cube3"]->transform.move(glm::vec3(-0.1f, 0.01f, -0.1f));
		scene->objects["cube4"]->transform.move(glm::vec3(-0.1f, 0.01f, 0.1f));
		scene->objects["cube5"]->transform.move(glm::vec3(0.1f, 0.01f, -0.1f));
		scene->objects["cube6"]->transform.move(glm::vec3(0.1f, 0.01f, 0.1f));

		scene->objects["cube1"]->transform.rotate(30, glm::vec3(1.0f, 1.0f, 4.0f));
		scene->objects["cube2"]->transform.rotate(30, glm::vec3(1.0f, 1.0f, 4.0f));
		scene->objects["cube3"]->transform.rotate(30, glm::vec3(1.0f, 1.0f, 4.0f));
		scene->objects["cube4"]->transform.rotate(30, glm::vec3(1.0f, 1.0f, 4.0f));
		scene->objects["cube5"]->transform.rotate(30, glm::vec3(1.0f, 1.0f, 4.0f));
		scene->objects["cube6"]->transform.rotate(30, glm::vec3(1.0f, 1.0f, 4.0f));
	}
	if(glfwGetKey('V'))
	{
		scene->objects["cube1"]->transform.move(-glm::vec3(0.0f, 0.0f, 0.1f));
		scene->objects["cube2"]->transform.move(-glm::vec3(0.1f, 0.0f, 0.1));
		scene->objects["cube3"]->transform.move(-glm::vec3(-0.1f, 0.0f, -0.1f));
		scene->objects["cube4"]->transform.move(-glm::vec3(-0.1f, -0.1f, 0.1f));
		scene->objects["cube5"]->transform.move(-glm::vec3(0.1f, -0.1f, -0.1f));
		scene->objects["cube6"]->transform.move(-glm::vec3(0.1f, -0.1f, 0.1f));
	}
}

void render(const effect* eff, const glm::mat4 view, const glm::mat4& projection, const render_object* object)
{
	glm::mat4 mvp = projection * view * object->transform.getTransformationMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	glm::mat4 mit = glm::inverse(glm::transpose(object->transform.getTransformationMatrix()));
	glUniformMatrix4fv(eff->getUniformIndex("modelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(mit));
	glUniformMatrix4fv(eff->getUniformIndex("model"), 1, GL_FALSE, glm::value_ptr(object->transform.getTransformationMatrix()));

	object->material->bind(eff);

	glBindVertexArray(object->geometry->vao);
	if (object->geometry->indexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->geometry->indexBuffer);
		glDrawElements(GL_TRIANGLES, object->geometry->indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, object->geometry->vertices.size());
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

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(currentCam->getPosition()));

	std::hash_map<std::string, render_object*>::const_iterator iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, currentCam->getView(), currentCam->getProjecion(), iter->second);

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




