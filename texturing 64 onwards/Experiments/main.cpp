#pragma comment(lib, "GLFWDLL")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "DevIL")
#pragma comment(lib, "ILU")
#pragma comment(lib, "ILUT")

#define GLFW_DLL

#include <glew.h>
#include <GL\glfw.h>
#include <IL\ilut.h>
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
target_camera* cam;
first_person_camera* cam1;
float screenHeight = 600.0f;
float screenWidth = 800.0f;

glm::vec3 lightAim(0.0f, 0.0f, 0.0f);
float power = 1.0f;

void initialise()
{
	srand(time(NULL));
		
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	//Target Camera
	cam = new target_camera();
	cam->setProjection(glm::degrees(glm::pi<float>() / 4.0f), screenWidth/screenHeight, 0.1f, 10000.0f);
	cam->setPositon(glm::vec3(-0.55f, 2.76f, -22.8f));
	cam->setTarget(glm::vec3(2.0f, 0.0f, 0.0f));

	//fps cam
	cam1 = new first_person_camera();
	cam1->setProjection(glm::pi<float>() / 4.0f, screenWidth/screenHeight, 0.1f, 10000.0f);
	cam1->setPositon(glm::vec3(-0.55f, 2.76f, -22.8f));
	cam1->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

	if (!eff.addShader("lit_textured.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	
	if (!eff.addShader("lighting.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("point_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("spot_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("lit_textured.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);

	scene = loadScene("myScene.json");
	//scene->objects["seat2"]->transform.scale = glm::vec3(1.0f, 5.0f, 1.0f);
	
}

//=================Move camera methods=======================================
//void moveChaseCam()
//{
//	if (glfwGetKey('W'))
//		cam1->rotate(glm::vec3(glm::pi<float>() / 100.0f, 0.0f, 0.0f));
//	if (glfwGetKey('S'))
//		cam1->rotate(glm::vec3(-glm::pi<float>() / 100.0f, 0.0f, 0.0f));
//	if (glfwGetKey('A'))
//		cam1->rotate(glm::vec3(0.0f, -glm::pi<float>() / 100.0f, 0.0f));
//	if (glfwGetKey('D'))
//		cam1->rotate(glm::vec3(0.0f, glm::pi<float>() / 250.0f, 0.0f));
//	cam1->setFollowPosition(object.transform.position);
//}
//
//void moveArcCam()
//{
//	if (glfwGetKey('W'))
//		cam2->rotate(glm::pi<float>() / 100.0f, 0.0f);
//	if (glfwGetKey('S'))
//		cam2->rotate(-glm::pi<float>() / 100.0f, 0.0f);
//	if (glfwGetKey('A'))
//		cam2->rotate(0.0f, glm::pi<float>() / 100.0f);
//	if (glfwGetKey('D'))
//		cam2->rotate(0.0f, -glm::pi<float>() / 100.0f);
//}
//
void moveFPSCam(float deltaTime, float speed)
{
	//fps cam controls
	if (glfwGetKey('W'))
		cam1->move(glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('S'))
		cam1->move(-glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('A'))
		cam1->move(glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey('D'))
		cam1->move(-glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey('E'))
		cam1->rotate(-glm::pi<float>() / 4.0f * deltaTime, 0.0f);
	if (glfwGetKey('Q'))
		cam1->rotate(glm::pi<float>() / 4.0f * deltaTime, 0.0f);
	if (glfwGetKey(GLFW_KEY_LCTRL))
		cam1->move(-glm::vec3(0.0f, speed, 0.0f) * (float)deltaTime);
	if (glfwGetKey(GLFW_KEY_LSHIFT))
		cam1->move(glm::vec3(0.0f, speed, 0.0f) * (float)deltaTime);
}
//================================================================================

void update(double deltaTime)
{
	cam1->update(deltaTime);
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	moveFPSCam(deltaTime, 2.0f);
	float angle  = 0.1f;
	if (glfwGetKey('K'))
	{
		
		angle = angle + 0.1f;
		scene->objects["seat2"]->transform.rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	std::cout<<cam1->getPosition().x<<" , "<<cam1->getPosition().y<<" , "<<cam1->getPosition().z<<"\n";
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

	spot_light_data spot = scene->dynamic.data.spots[0];
	spot.lightDir = glm::vec4(lightAim, 1.0f) - spot.lightPos;
	spot.lightDir = glm::normalize(spot.lightDir);
	spot.power = power;
	// Set the light
	scene->dynamic.setSpot(0, spot); //here the buffer is 0 so spotlight doesnt get set......

	scene->dynamic.bind(&eff);
	scene->light.bind(&eff);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(cam1->getPosition()));
	
	std::hash_map<std::string, render_object*>::const_iterator iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, cam1->getView(), cam1->getProjecion(), iter->second);

	eff.end();
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
	if (!glfwOpenWindow(screenWidth, screenHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
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

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

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




