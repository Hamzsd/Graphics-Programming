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

bool running = true;

effect eff;
lighting light;
dynamic_lights dynamic;
glm::mat4 projection;
render_object plane;
render_object sphere;
render_object box[4];

glm::vec3 lightAim(0.0f, 0.0f, 0.0f);
float power = 1.0f;

void initialise()
{
	srand(time(NULL));

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	projection = glm::perspective(glm::degrees(glm::quarter_pi<float>()),
								  800.0f/600.0f,
								  0.1f,
								  10000.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);

	if (!eff.addShader("multi_light.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("multi_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);

	light.data.ambient = glm::vec4(0.0f, 0.0f, 0.1f, 1.0f);
	light.data.diffuse = glm::vec4(0.0f, 0.0f, 0.2f, 1.0f);
	light.data.specular = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
	light.data.lightDir = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	light.create();

	spot_light_data spot;
	spot.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	spot.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	spot.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	spot.lightPos = glm::vec4(0.0f, 2.0f, 0.0f, 1.0f);
	spot.attenuation = glm::vec4(0.05f, 0.02f, 0.01f, 1.0f);
	spot.lightDir = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	spot.power = 1.0f;
	spot.dist = 10.0f;
	dynamic.addSpot(spot);

	point_light_data point[4];
	glm::vec4 cols[4];
	cols[0] = glm::vec4(0.2f, 0.0f, 0.0f, 1.0f);
	cols[1] = glm::vec4(0.0f, 0.2f, 0.0f, 1.0f);
	cols[2] = glm::vec4(0.0f, 0.0f, 0.2f, 1.0f);
	cols[3] = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	for (int i = 0; i < 4; ++i)
	{
		point[i].ambient = cols[i];
		point[i].diffuse = cols[i];
		point[i].specular = cols[i];
		point[i].attenuation = glm::vec4(0.02f, 0.01f, 0.005f, 1.f);
		point[i].dist = 10.0f;
	}
	point[0].lightPos = glm::vec4(-2.0f, 2.0f, -2.0f, 1.0f);
	point[1].lightPos = glm::vec4(2.0f, 2.0f, -2.0f, 1.0f);
	point[2].lightPos = glm::vec4(-2.0f, 2.0f, 2.0f, 1.0f);
	point[3].lightPos = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
	for (int i = 0; i < 4; ++i)
		dynamic.addPoint(point[i]);



	plane.geometry = createPlane(20, 20);
	glm::vec4 colour(0.1f, 0.1f, 0.1f, 1.0f);
	plane.material = new material();
	plane.material->data.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	plane.material->data.ambient = colour;
	plane.material->data.diffuse = colour;
	plane.material->data.specular = colour;
	plane.material->data.shininess = 2.0f;
	plane.material->create();

	sphere.geometry = createSphere(20, 20);
	sphere.material = new material();
	sphere.material->data.emissive = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sphere.material->data.shininess = 1.0f;
	sphere.material->create();
	sphere.transform.position = glm::vec3(0.0f, 2.0f, 0.0f);
	sphere.transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);

	geometry* geom = createBox();
	for (int i = 0; i < 4; ++i)
	{
		box[i].geometry = geom;
		glm::vec4 colour(0.1f * i, 0.6f - (0.1f * i), (float)rand() / (float)RAND_MAX, 1.0f);
		box[i].material = new material;
		box[i].material->data.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		box[i].material->data.ambient = colour;
		box[i].material->data.diffuse = colour;
		box[i].material->data.specular = colour;
		box[i].material->data.shininess = i + 1;
		box[i].material->create();
		box[i].transform.scale = glm::vec3(0.25f, 0.25f, 0.25f);
	}
	box[0].transform.position = glm::vec3(-2.0f, 0.25f, -2.0f);
	box[1].transform.position = glm::vec3(2.0f, 0.25f, -2.0f);
	box[2].transform.position = glm::vec3(-2.0f, 0.25f, 2.0f);
	box[3].transform.position = glm::vec3(2.0f, 0.25f, 2.0f);
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	if (glfwGetKey('O'))
		power = std::max<float>(power - 0.1f, 0.0f);
	if (glfwGetKey('P'))
		power += 0.1f;
	if (glfwGetKey(GLFW_KEY_UP))
		lightAim.z -= 0.1f;
	if (glfwGetKey(GLFW_KEY_DOWN))
		lightAim.z += 0.1f;
	if (glfwGetKey(GLFW_KEY_LEFT))
		lightAim.x -= 0.1f;
	if (glfwGetKey(GLFW_KEY_RIGHT))
		lightAim.x += 0.1f;
	if (glfwGetKey('W'))
		plane.transform.move(glm::vec3(0.0f, 0.05f, 0.0f));
	if (glfwGetKey('S'))
		plane.transform.move(glm::vec3(0.0f, -0.05f, 0.0f));
	if (glfwGetKey('A'))
		plane.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey('D'))
		plane.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
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
	glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.f),
								 glm::vec3(0.0f, 0.0f, 0.0f),
								 glm::vec3(0.0f, 1.0f, 0.0f));
	eff.begin();

	// Create light direction from lightAngle
	spot_light_data spot = dynamic.data.spots[0];
	spot.lightDir = glm::vec4(lightAim, 1.0f) - spot.lightPos;
	spot.lightDir = glm::normalize(spot.lightDir);
	spot.power = power;
	// Set the light
	dynamic.setSpot(0, spot); //here the buffer is 0 so spotlight doesnt get set......

	dynamic.bind(&eff);
	light.bind(&eff);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(glm::vec3(10.0f, 10.0f, 10.0f)));

	render(&eff, view, projection, plane);
	render(&eff, view, projection, sphere);
	for (int i = 0; i < 4; ++i)
		render(&eff, view, projection, box[i]);

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