#pragma once

#include <string>
#include <hash_map>
#include <vector>
#include <glew.h>
#include <glm\glm.hpp>
#include <GL\glfw.h>
#include <glm\gtc\type_ptr.hpp>//for mat4
#include <glm\gtc\matrix_transform.hpp>
#include "geometry.h"

GLuint loadShader(const std::string& filename, GLenum type);
GLuint createProgram(GLuint* shaders, int count);

struct lighting
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec3 lightDir;
};

class shader
{
public:
	GLuint program;
	GLuint shaders[2];
	std::hash_map<std::string, GLint> uniforms;

	shader(const std::string& name);
	~shader();

	void setMaterial(const material* material);
	void setLight(const lighting* light);
};