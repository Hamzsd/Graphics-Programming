#pragma comment (lib, "GLFWDLL") 
#pragma comment (lib, "OpenGL32")
#pragma comment (lib, "glew32")
//TODO fix unhandled exception when program is closed. 
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
lighting light;
GLuint lightBuffer;
glm::mat4 projection;
render_object object;
float lightAngle = 0.0f;

GLuint vertexBuffer;
GLuint vao;

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	projection = glm::perspective(50.0f, 800.0f/600.0f, 0.1f, 10000.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	s = new shader("phong");
	if (!s->shaders[0] || !s->shaders[1] || !s->program)
		exit(EXIT_FAILURE);
	light.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.diffuse = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	light.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	light.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.lightDir = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
	// Create light buffer
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lighting), &light, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//object.geometry = createTorus(6, 500, 500);
	//object.geometry = createSphere(20, 20);
	//object.geometry = createPlane(10, 10);
	object.geometry = createDisk(50);
	glm::vec4 colour(0.5f, 0.25f, 0.15f, 1.0f);
	object.material = createMaterial(colour, colour, colour, 5.0f);
	object.materialBuffer = createMaterialBuffer(object.material);
	
	object.transform.position = glm::vec3(0.0f, 0.5f, 0.0f);
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	if (glfwGetKey(GLFW_KEY_UP))
		object.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (glfwGetKey(GLFW_KEY_DOWN))
		object.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (glfwGetKey(GLFW_KEY_LEFT))
		object.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey(GLFW_KEY_RIGHT))
		object.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey('W'))
		object.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (glfwGetKey('S'))
		object.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (glfwGetKey('P'))
		lightAngle += glm::pi<float>() / 100.0f;
	if (glfwGetKey('O'))
		lightAngle -= glm::pi<float>() / 100.0f;;
}

void render(const glm::mat4& view, const glm::mat4& projection, const render_object& object)
{
		glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
		glUniformMatrix4fv(s->uniforms["modelViewProjection"], 1, GL_FALSE,glm::value_ptr(mvp));
		glm::mat4 mit = glm::inverse(glm::transpose(object.transform.getTransformationMatrix()));
		glUniformMatrix4fv(s->uniforms["modelInverseTranspose"], 1, GL_FALSE, glm::value_ptr(mit));
		glUniformMatrix4fv(s->uniforms["model"], 1, GL_FALSE, glm::value_ptr(object.transform.getTransformationMatrix()));
		light.lightDir = glm::vec4(-cosf(lightAngle), 0.0f, -sinf(lightAngle), 1.0f);
		
		//change the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), &light.lightDir);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		s->setLight(lightBuffer);
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

	render(view, projection, object);

	glUseProgram(0);
	glfwSwapBuffers();
}

void cleanup()
{
	if (object.geometry) delete object.geometry;
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
