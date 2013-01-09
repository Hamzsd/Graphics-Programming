#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string readFile(const std::string& filename)
{
	std::ifstream file(filename);
	std::stringstream buffer;
	buffer <<file.rdbuf();
	std::string content = buffer.str();
	return content;
}

GLuint loadShader(const std::string& filename, GLenum type)
{
	std::string fileContent = readFile(filename);
	GLuint shader = glCreateShader(type);
	const char* source = fileContent.c_str();
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei length;
		char log[1024];
		glGetShaderInfoLog(shader, 1024, &length, log);
		std::cout << "could not compile shader: " << shader << ": " << filename << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint createProgram(GLuint* shaders, int count)
{
	GLuint program = glCreateProgram();
	for (int i = 0; i < count; ++i)
		glAttachShader(program, shaders[i]);
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei length;
		char log[1024];
		glGetProgramInfoLog(program, 1024, &length, log);
		std::cout << "Error linking program." << std::endl;
		std::cout << log << std::endl;
		for (int i = 0; i < count; ++i)
			glDetachShader(program, shaders[i]);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}