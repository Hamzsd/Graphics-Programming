#pragma comment(lib, "GLFW")
#pragma comment(lib, "OpenGL32")

#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\constants.hpp>
#include "geometry.h"
#include <cstdlib>

bool running = true;

render_object cube;
render_object tetrahedron;
geometry geom;

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 projection = glm::perspective(10.0f,
											800.0f/600.0f,
											0.1f, 
											10000.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	//geometry* geom = createBox();//call to create box
	//geometry* geom = createPyramid();
	//geometry* geom = createTetrahedron();
	//geometry* geom = createDisk(200);
	//geometry* geom = createCylinder(5, 10);
	//geometry* geom = createSphere(20, 20);
	//geometry* geom = createTorus(5.0f, 10, 30);
	//geometry* geom = createPlane(20, 20);
	geometry* geom = createSierpinski(5);

	cube.geometry = geom;
	cube.colour = glm::vec3(1.0f, 0.0f, 0.0f);
	cube.transform.position = glm::vec3(0.0f, 0.5f, 0.0f);
	
}

void update(double deltaTime)
{
	 //running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	 //cube.transform.rotate(-glm::pi<float>() / 400.0f, glm::vec3(0.0f, 0.0f, 1.0f)); //Self roation code
	 //cube.transform.rotate(-glm::pi<float>() / 400.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	 //cube.transform.rotate(glm::pi<float>() / 400.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	 //manual rotation code
	 	if (glfwGetKey(GLFW_KEY_UP)) 
		cube.transform.rotate(0.1f,glm::vec3(0.0, 0.0, 0.1)); 
	if (glfwGetKey(GLFW_KEY_DOWN)) 
		cube.transform.rotate(-0.1f,glm::vec3(0.1, 0.0, 0.1));
	if (glfwGetKey(GLFW_KEY_LEFT)) 
		cube.transform.rotate(-0.1f,glm::vec3(0.1, 0.0, 0.0));
	if (glfwGetKey(GLFW_KEY_RIGHT)) 
		cube.transform.rotate(0.1f,glm::vec3(0.1, 0.0, 0.0)); 
	if (glfwGetKey('W'))
		cube.transform.rotate(0.1f, glm::vec3(0.0f, 0.1f, 0.0f));
	if (glfwGetKey('S'))
		cube.transform.rotate(-0.1f, glm::vec3(0.0f, 0.1f, 0.0f));
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f),
								 glm::vec3(0.0f, 0.0f, 0.0f),
								 glm::vec3(0.0f, 1.0f, 0.0f));
	glMatrixMode(GL_MODELVIEW);
	cube.render(view);
	glfwSwapBuffers();
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

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
