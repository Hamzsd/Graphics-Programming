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
#include <cstdlib>
#include <iostream>
#include "geometry.h"
#include "shader.h"

bool running = true;

GLuint shaders[2];
GLuint program;
GLint MVPuniform;
GLint ambientMaterialUniform;
GLint ambientLightUniform;
glm::mat4 projection;
render_object object;
float light = 1.0f;

glm::vec3 vertices[6] = 
{
	// Triangle 1 
	glm::vec3(-1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 0.0f),
	// Triangle 2
	glm::vec3(-1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f)
};

GLuint vertexBuffer;
GLuint vao;

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	projection = glm::perspective(50.0f,
											800.0f/600.0f,
											0.1f,
											10000.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	shaders[0] = loadShader("ambient.vert", GL_VERTEX_SHADER);
	shaders[1] = loadShader("ambient.frag", GL_FRAGMENT_SHADER);
	if (shaders[0] && shaders[1])
	{
		program = createProgram(shaders, 2);
		if (!program)
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);//TODO 36 is crashing on this line, seems like it cant load shaders into program

	MVPuniform = glGetUniformLocation(program, "modelViewProjection");
	ambientMaterialUniform = glGetUniformLocation(program, "ambientMaterial");
	ambientLightUniform = glGetUniformLocation(program, "ambientLight");

	object.geometry = createTorus(6, 50, 50);		
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
		light = std::min<float>(light +0.01f, 1.0f);
	if (glfwGetKey('O'))
		light = std::max<float>(light - 0.01, 0.0f);
}

void render(const glm::mat4& view, const glm::mat4& projection, const render_object& object)
{
		glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
		glUniformMatrix4fv(MVPuniform, 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform4fv(ambientMaterialUniform, 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		glUniform4fv(ambientLightUniform, 1, glm::value_ptr(glm::vec4(light, light, light, 1.0f)));

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
	glUseProgram(program);

	render(view, projection, object);

	glUseProgram(0);
	glfwSwapBuffers();
}

void cleanup()
{
	if (program) glDeleteProgram(program);
	if (shaders[0]) glDeleteShader(shaders[0]);
	if (shaders[1]) glDeleteShader(shaders[1]);
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
