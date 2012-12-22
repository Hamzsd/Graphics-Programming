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

GLuint shaders[2];
GLuint program;
GLint MVPuniform;
GLint MITuniform;
GLint diffuseMaterialUniform;
GLint diffuseLightUniform;
GLint lightDirUniform;
glm::mat4 projection;
render_object object;
float lightAngle = 0.0f;

GLuint vertexBuffer;
GLuint vao;

bool gp;
GLuint filter;
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };
GLuint fogfilter = 0;
GLfloat fogColour[4] = {0.5, 0.5, 0.5f, 1.0f};

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	projection = glm::perspective(glm::degrees(glm::quarter_pi<float>()),
											800.0f/600.0f,
											0.1f,
											10000.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	shaders[0] = loadShader("diffuse.vert", GL_VERTEX_SHADER);
	shaders[1] = loadShader("diffuse.frag", GL_FRAGMENT_SHADER);
	if (shaders[0] && shaders[1])
	{
		program = createProgram(shaders, 2);
		if (!program)
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);

	MVPuniform = glGetUniformLocation(program, "modelViewProjection");
	MITuniform = glGetUniformLocation(program, "modelInverseTranspose");
	diffuseMaterialUniform = glGetUniformLocation(program, "diffuseMaterial");
	diffuseLightUniform = glGetUniformLocation(program, "diffuseLight");
	lightDirUniform = glGetUniformLocation(program, "lightDir");

	object.geometry = createBox();	

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, fogColour);
	glFogf(GL_FOG_DENSITY, 0.35f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 5.0f);
	glEnable(GL_FOG);
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
	std::cout<<"This is in the update of project 40_01";
}

void render(const glm::mat4& view, const glm::mat4& projection, const render_object& object)
{
		glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
		glUniformMatrix4fv(MVPuniform, 1, GL_FALSE, glm::value_ptr(mvp));
		glm::mat4 mit = glm::inverse(glm::transpose(object.transform.getTransformationMatrix()));
		glUniformMatrix4fv(MITuniform, 1, GL_FALSE, glm::value_ptr(mit));
		glUniform4fv(diffuseMaterialUniform, 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
		glUniform4fv(diffuseLightUniform, 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

		// Calculate light direction vector by assuming angle rotated around Y-axis
		glm::vec3 lightDirection = glm::vec3(-cosf(lightAngle), 0.0f, -sinf(lightAngle));
		// Current light direction goes from the centre outwards. therefore, we invert it
		glUniform3fv(lightDirUniform, 1, glm::value_ptr(-lightDirection));

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
