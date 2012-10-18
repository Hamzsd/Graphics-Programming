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
GLuint colourUniform;
glm::mat4 projection;
render_object object;

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
	projection = glm::perspective(glm::degrees(glm::quarter_pi<float>()),
											800.0f/600.0f,
											0.1f,
											10000.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	shaders[0] = loadShader("transform.vert", GL_VERTEX_SHADER);
	shaders[1] = loadShader("transform.frag", GL_FRAGMENT_SHADER);
	if (shaders[0] && shaders[1])
	{
		program = createProgram(shaders, 2);
		if (!program)
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);//is crashing on this line, seems like it cant load shaders into program

	MVPuniform = glGetUniformLocation(program, "modelViewProjection");
	colourUniform = glGetUniformLocation(program, "colour");

	geometry* geom = createBox();
	object.geometry = geom;
	//colour changed to vec4
	object.colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the vertex array, and bind
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate the vertex buffer, and bin
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// Set the buffer data (static draw as we won't change the data)
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

	// Set vertex attribute 0 and enable
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind the vertex array
	glBindVertexArray(0);
		
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
}

void render(const glm::mat4& view, const glm::mat4& projection, const render_object& object)
{
		glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
		glUniformMatrix4fv(MVPuniform, 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform4fv(colourUniform, 1, glm::value_ptr(object.colour));

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

