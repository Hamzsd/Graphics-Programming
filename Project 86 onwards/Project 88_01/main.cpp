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
#include "util.h"
#include "skybox.h"
#include "Point_sprite.h"
#include "frame_buffer.h"

bool running = true;

effect eff;
effect text_eff;
scene_data* scene;

target_camera* cam;
target_camera* cam2;

frame_buffer* framebuffer;

render_object* obj = 0;



float screenHeight = 600.0f;
float screenWidth = 800.0f;

void initialise()
{
	srand(time(NULL));
		
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);

	cam = new target_camera();
	cam2 = new target_camera();

	cam->setProjection(glm::degrees(glm::quarter_pi<float>()), screenWidth/screenHeight, 0.1f, 10000.0f);
	cam->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	cam->setPositon(glm::vec3(5.0f, 5.0f, 5.0f));

	cam2->setProjection(glm::degrees(glm::quarter_pi<float>()), screenWidth/screenHeight, 0.1f, 10000.0f);
	cam2->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	cam2->setPositon(glm::vec3(10.0f, 10.0f, 10.0f));
	
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

	/*if (!text_eff.addShader("texture.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!text_eff.addShader("texture.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);*/

	
	if (!text_eff.addShader("lit_textured.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!text_eff.addShader("lighting.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!text_eff.addShader("point_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!text_eff.addShader("spot_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!text_eff.addShader("lit_textured.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!text_eff.create())
		exit(EXIT_FAILURE);

	scene = loadScene("scene.json");
	obj = new render_object();
	obj->geometry = createBox();
	obj->material = 0;

	framebuffer = new frame_buffer(screenWidth, screenHeight);
	framebuffer->create();
}

//================================================================================

void update(double deltaTime)
{
	cam->update(deltaTime);
	cam2->update(deltaTime);

	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

	if (glfwGetKey(GLFW_KEY_UP))
		cam2->setPositon(cam2->getPosition() - glm::vec3(0.1f, 0.1f, 0.1f));
	if (glfwGetKey(GLFW_KEY_DOWN))
		cam2->setPositon(cam2->getPosition() + glm::vec3(0.1f, 0.1f, 0.1f));
	
}

void render(const effect* eff, const glm::mat4 view, const glm::mat4& projection, const render_object* object)
{
	printf("should not get here \n");
	glm::mat4 mvp = projection * view * object->transform.getTransformationMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	glm::mat4 mit = glm::inverse(glm::transpose(object->transform.getTransformationMatrix()));
	glUniformMatrix4fv(eff->getUniformIndex("modelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(mit));
	glUniformMatrix4fv(eff->getUniformIndex("model"), 1, GL_FALSE, glm::value_ptr(object->transform.getTransformationMatrix()));
	CHECK_GL_ERROR
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), object->transform.scale);
	glUniformMatrix4fv(eff->getUniformIndex("scale"), 1, GL_FALSE, glm::value_ptr(scale));
	CHECK_GL_ERROR
	//object->material->bind(eff);

	glBindVertexArray(object->geometry->vao);
	if (object->geometry->indexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->geometry->indexBuffer);
		glDrawElements(GL_TRIANGLES, object->geometry->indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, object->geometry->vertices.size());
	glBindVertexArray(0);
	CHECK_GL_ERROR
}

void render()
{
	eff.begin();
	scene->light.bind(&eff);
	scene->dynamic.bind(&eff);
	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(cam->getPosition()));
	CHECK_GL_ERROR

	framebuffer->bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::hash_map<std::string, render_object*>::const_iterator iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, cam2->getView(), cam2->getProjecion(), iter->second);

	framebuffer->unbind();
	eff.end();

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_GL_ERROR

	text_eff.begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffer->getTextureID());

	glUniform1i(text_eff.getUniformIndex("tex"), 0);
	CHECK_GL_ERROR

	render(&text_eff, cam->getView(), cam->getProjecion(), obj);

	text_eff.end();
	CHECK_GL_ERROR

	glfwSwapBuffers();
}

void cleanup()
{
}

int main()
{
	SET_DEBUG

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




