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
spot_light light;
GLuint lightBuffer;
glm::mat4 projection;
render_object plane;
render_object sphere;
render_object box;
glm::vec3 lightAim(0.0f, 0.0f, 0.0f);
float time = 0.0f;
float emissive = 0.5f;

GLuint vertexBuffer;
GLuint vao;

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	projection = glm::perspective(50.0f, 800.0f/600.0f, 0.1f, 10000.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	s = new shader("spot");
	if (!s->shaders[0] || !s->shaders[1] || !s->program)
		exit(EXIT_FAILURE);

	light.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light.lightPos = glm::vec4(0.0f, 2.0f, 0.0f, 1.0f);
	light.attenuation = glm::vec4(0.2f, 0.1f, 0.05f, 1.0f);
	light.lightDir = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	light.power = 2.0f;
	
		
	// Create light buffer
	glGenBuffers(1, &lightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(spot_light), &light, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	plane.geometry = createPlane(20, 20);
	glm::vec4 colour(0.5f, 0.5, 0.5, 1.0f);
	plane.material = createMaterial(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),colour, colour, colour, 10.0f);
	plane.materialBuffer = createMaterialBuffer(plane.material);

	sphere.geometry = createSphere(20, 20);
	colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sphere.material = createMaterial(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),colour, colour, colour, 1.0f);
	sphere.materialBuffer = createMaterialBuffer(sphere.material);
	sphere.transform.position = glm::vec3(0.0f, 2.0f, 0.0f);
	sphere.transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);

	box.geometry = createCylinder(10, 20);
	colour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	box.material = createMaterial(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f), colour, colour, colour, 1.0f);
	box.materialBuffer = createMaterialBuffer(box.material);
	box.transform.position = glm::vec3(-2.0f, 0.25f, 2.0f);
	box.transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	if (glfwGetKey('O'))
		light.power = std::max<float>(light.power - 0.1, 0.0f);
	if (glfwGetKey('P'))
		light.power += 0.5f;
	if (glfwGetKey(GLFW_KEY_UP))
		lightAim.z -= 0.05f;
	if (glfwGetKey(GLFW_KEY_DOWN))
		lightAim.z += 0.05f;
	if (glfwGetKey(GLFW_KEY_LEFT))
		lightAim.x -= 0.05f;
	if (glfwGetKey(GLFW_KEY_RIGHT))
		lightAim.x += 0.05f;
	if (glfwGetKey('W'))
	{
		sphere.transform.move(glm::vec3(0.0f, 0.05f, 0.0f));
		light.lightPos.y += 0.05f;
	}
	if (glfwGetKey('S'))
		sphere.transform.move(glm::vec3(0.0f, -0.05f, 0.0f));
	if (glfwGetKey('A'))
		box.transform.rotate(-glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey('D'))
		box.transform.rotate(glm::pi<float>() / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	
	time += deltaTime;
	if (time >= 1.0f)
	{
		emissive = emissive == 0.5f ? 0.0f : 0.5f;
		box.material->emissive = glm::vec4(0.0f, emissive, 0.0f, 1.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, box.materialBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &box.material->emissive);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		time = 0.0f;
	}
}

void render(const glm::mat4& view, const glm::mat4& projection, const render_object& object)
{
		glm::mat4 mvp = projection * view * object.transform.getTransformationMatrix();
		glUniformMatrix4fv(s->uniforms["modelViewProjection"], 1, GL_FALSE,glm::value_ptr(mvp));
		glm::mat4 mit = glm::inverse(glm::transpose(object.transform.getTransformationMatrix()));
		glUniformMatrix4fv(s->uniforms["modelInverseTranspose"], 1, GL_FALSE, glm::value_ptr(mit));
		glUniformMatrix4fv(s->uniforms["model"], 1, GL_FALSE, glm::value_ptr(object.transform.getTransformationMatrix()));
		
		//light.lightPos = glm::vec4(sphere.transform.position.x, sphere.transform.position.y, sphere.transform.position.z, 1.0f);
		
		// Create light direction from lightAim
		light.lightDir = glm::vec4(lightAim, 1.0f) - light.lightPos;
		light.lightDir = glm::normalize(light.lightDir);

		//change the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4), sizeof(glm::vec4), &light.lightDir);
		glBufferSubData(GL_UNIFORM_BUFFER, 6 * sizeof(glm::vec4), sizeof(float), &light.power);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		s->setSpotLight(lightBuffer);
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
	render(view, projection, plane);
	render(view, projection, box);

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
