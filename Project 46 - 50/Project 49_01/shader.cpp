#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

shader::shader(const std::string& filename)
{
	shaders[0] = loadShader(filename + ".vert", GL_VERTEX_SHADER);
	shaders[1] = loadShader(filename + ".frag", GL_FRAGMENT_SHADER);
	program = createProgram(shaders, 2);

	std::vector<std::string> uniformNames;
	GLint numUniforms, maxLength;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	char* buf = new char[maxLength];
	for (int i = 0; i < numUniforms; ++i)
	{
		GLsizei size;
		glGetActiveUniformName(program, i, maxLength, &size, buf);
		std::string name(buf);
		if (uniformNames.size() > 0 && uniformNames[i - 1] == name)
			break;
		else
			uniformNames.push_back(name);
	}
	delete[] buf;

	for (int i = 0; i < uniformNames.size(); ++i)
	{
		GLint uniformLocation = glGetUniformLocation(program, uniformNames[i].c_str());
		if (uniformLocation != -1) uniforms[uniformNames[i]] = uniformLocation;
	}

}

shader::~shader()
{
	if (program) glDeleteProgram(program);
	if (shaders[0]) glDeleteShader(shaders[0]);
	if (shaders[1]) glDeleteShader(shaders[1]);
}

void shader::setMaterial(const material* material)
{
	glUniform4fv(uniforms["ambientMaterial"], 1, glm::value_ptr(material->ambient));
	glUniform4fv(uniforms["diffuseMaterial"], 1, glm::value_ptr(material->diffuse));
	glUniform4fv(uniforms["specularMaterial"], 1, glm::value_ptr(material->specular));
	glUniform1f(uniforms["shininess"], material->shininess);
}

void shader::setLight(const lighting* light)
{
	glUniform4fv(uniforms["ambientLight"], 1, glm::value_ptr(light->ambient));
	glUniform4fv(uniforms["diffuseLight"], 1, glm::value_ptr(light->diffuse));
	glUniform4fv(uniforms["specularLight"], 1, glm::value_ptr(light->specular));
	glUniform3fv(uniforms["lightDir"], 1, glm::value_ptr(light->lightDir));
}

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