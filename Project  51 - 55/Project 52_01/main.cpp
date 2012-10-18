#pragma comment (lib, "GLFWDLL") 
#pragma comment (lib, "OpenGL32")
#pragma comment (lib, "glew32")

#define GLFW_DLL

#include <glew.h>
#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\constants.hpp>
#include <glm\gtc\type_precision.hpp>
#include <cstdlib>
#include <iostream>
#include "geometry.h"
#include "shader.h"

bool running = true;

shader* s;
point_light light;
GLuint lightBuffer;
glm::mat4 projection;
render_object boxes[4];
render_object sphere;


GLuint vertexBuffer;
GLuint vao;

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	projection = glm::perspective(30.0f, 800.0f/600.0f, 0.1f, 10000.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	s = new shader("point");
	if (!s->shaders[0] || !s->shaders[1] || !s->program)
		exit(EXIT_FAILURE);

	light.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light.specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light.lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light.attenuation = glm::vec4(0.02f, 0.01f, 0.005f, 1.0f);
	light.distance = 10.0f;
		
	// Create light buffer
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(point_light), &light, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	sphere.geometry = createSphere(10, 20);
	glm::vec4 colour(1.0f, 1.0f, 1.0f, 1.0f);
	sphere.material = createMaterial(colour, colour, colour, 10.0f);
	sphere.materialBuffer = createMaterialBuffer(sphere.material);
	sphere.transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);

	geometry* geom = createSphere(5);
	//geometry* geom = createSierpinski(5);
	for (int i = 0; i < 4; ++i)
	{
		boxes[i].geometry = geom;
		glm::vec4 colour = glm::vec4(0.1f * i, 0.6f - (0.1f *i), (float)rand() / (float)RAND_MAX, 1.0f);
		boxes[i].material = createMaterial(colour, colour, colour, 5.0f * (i + 1));
		boxes[i].materialBuffer = createMaterialBuffer(boxes[i].material);
		boxes[i].transform.position = glm::vec3(-1.0f, 0.0f, 5.0f + (-i * 3.0f));
		boxes[i].transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	if (glfwGetKey(GLFW_KEY_UP))
		sphere.transform.move(glm::vec3(0.0f, 0.0f, -0.05f));
	if (glfwGetKey(GLFW_KEY_DOWN))
		sphere.transform.move(glm::vec3(0.0f, 0.0f, 0.05f));
	if (glfwGetKey(GLFW_KEY_LEFT))
		sphere.transform.move(glm::vec3(-0.05f, 0.0f, 0.0f));
	if (glfwGetKey(GLFW_KEY_RIGHT))
		sphere.transform.move(glm::vec3(0.05f, 0.0f, 0.0f));
	if (glfwGetKey('W'))
		sphere.transform.move(glm::vec3(0.0f, 0.05f, 0.0f));
	if (glfwGetKey('S'))
		sphere.transform.move(glm::vec3(0.0f, -0.05f, 0.0f));
	
}

void render(const glm::mat4& view, const glm::mat4& projection, const render_object& object)
{
		glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
		glUniformMatrix4fv(s->uniforms["modelViewProjection"], 1, GL_FALSE,glm::value_ptr(mvp));
		glm::mat4 mit = glm::inverse(glm::transpose(object.transform.getTransformationMatrix()));
		glUniformMatrix4fv(s->uniforms["modelInverseTranspose"], 1, GL_FALSE, glm::value_ptr(mit));
		glUniformMatrix4fv(s->uniforms["model"], 1, GL_FALSE, glm::value_ptr(object.transform.getTransformationMatrix()));
		
		light.lightPos = glm::vec4(sphere.transform.position.x, sphere.transform.position.y, sphere.transform.position.z, 1.0f);
		
		//change the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), &light.lightPos);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		s->setPointLight(lightBuffer);
		s->setMaterial(object.materialBuffer);

		glUniform3fv(s->uniforms["eyePos"], 1, glm::value_ptr(glm::vec3(10.0f, 10.0f, 10.0f)));


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
	glUseProgram(s->program);

	render(view, projection, sphere);
	for (int i = 0; i < 4; ++i)
		render(view, projection, boxes[i]);

	glUseProgram(0);
	glfwSwapBuffers();
}

void cleanup()
{
	//if (object.geometry) delete object.geometry;
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
		std::cout << "Error: " << glewGetErrorString(error) << std::endl;
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

	cleanup();

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
