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
render_object object;
render_object plane;
scene_data* scene;
chase_camera* cam1;
arc_ball_camera* cam2;
target_camera* cam3;
first_person_camera* cam4;
camera* currentCam;

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

	if (!eff.addShader("texture.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("texture.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);

	object.geometry = createBox();
	object.material = new material();
	object.material->data.ambient = glm::vec4(0.5f, 0.25f, 0.15f, 1.0f);
	object.material->data.diffuse = glm::vec4(0.5f, 0.25f, 0.15f, 1.0f);
	object.material->data.specular = glm::vec4(0.5f, 0.25f, 0.15f, 1.0f);
	object.material->data.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	object.material->texture = ilutGLLoadImage((wchar_t*)"brick2.jpg");
	object.material->create();

	plane.geometry = createPlane(20,20);
	plane.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	plane.material = new material();
	plane.material->data.ambient = glm::vec4(0.5f, 0.25f, 0.15f, 1.0f);
	plane.material->data.diffuse = glm::vec4(0.5f, 0.25f, 0.15f, 1.0f);
	plane.material->data.specular = glm::vec4(0.5f, 0.25f, 0.15f, 1.0f);
	plane.material->data.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	plane.material->create();
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
	if (glfwGetKey('E'))
		cam4->rotate(glm::pi<float>() * deltaTime, 0.0f);
	if (glfwGetKey('R'))
		cam4->rotate(-glm::pi<float>() * deltaTime, 0.0f);
	if (glfwGetKey(GLFW_KEY_LCTRL))
		cam4->move(-glm::vec3(0.0f, speed, 0.0f) * (float)deltaTime);
	if (glfwGetKey(GLFW_KEY_LSHIFT))
		cam4->move(glm::vec3(0.0f, speed, 0.0f) * (float)deltaTime);
}
//================================================================================

void update(double deltaTime)
{
	currentCam->update(deltaTime);
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);	

	if(glfwGetKey(GLFW_KEY_UP))
		object.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	if(glfwGetKey(GLFW_KEY_DOWN))
		object.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	if(glfwGetKey(GLFW_KEY_LEFT))
		object.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if(glfwGetKey(GLFW_KEY_RIGHT))
		object.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	float speed = 5.0f;
	
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
}

void render(const effect* eff, const glm::mat4 view, const glm::mat4& projection, const render_object* object)
{
	glm::mat4 mvp = projection * view * object->transform.getTransformationMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	
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

	render(&eff, currentCam->getView(), currentCam->getProjecion(), &object);
	render(&eff, currentCam->getView(), currentCam->getProjecion(), &plane);

	eff.end();
	
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




