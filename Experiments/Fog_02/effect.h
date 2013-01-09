#pragma once

#include <string>
#include <hash_map>
#include <glew.h>
#include <glm\glm.hpp>

using std::string;
class effect
{
private:
	GLuint _program;
	std::vector<GLuint> _shaders;
	std::hash_map<std::string, GLint> _uniforms;
	std::hash_map<std::string, GLint> _blockUniforms;
public:
	effect();
	~effect();
	void begin();
	void end();
	bool addShader(const std::string&  filename, GLenum type);
	bool create();
	GLint getUniformIndex(const std::string& name) const { return _uniforms.at(name); }
	GLint getUniformBlockIndex(const std::string& name) const { return _blockUniforms.at(name); }
	
	// Setting vectors
	void setUniform(string sName, glm::vec2* vVectors, int iCount = 1);
	void setUniform(string sName, const glm::vec2 vVector);
	void setUniform(string sName, glm::vec3* vVectors, int iCount = 1);
	void setUniform(string sName, const glm::vec3 vVector);
	void setUniform(string sName, glm::vec4* vVectors, int iCount = 1);
	void setUniform(string sName, const glm::vec4 vVector);

	// Setting floats
	void setUniform(string sName, float* fValues, int iCount = 1);
	void setUniform(string sName, const float fValue);

	// Setting 3x3 matrices
	void setUniform(string sName, glm::mat3* mMatrices, int iCount = 1);
	void setUniform(string sName, const glm::mat3 mMatrix);

	// Setting 4x4 matrices
	void setUniform(string sName, glm::mat4* mMatrices, int iCount = 1);
	void setUniform(string sName, const glm::mat4 mMatrix);

	// Setting integers
	void setUniform(string sName, int* iValues, int iCount = 1);
	void setUniform(string sName, const int iValue);
};

GLuint loadShader(const std::string& filename, GLenum type);

GLuint createProgram(GLuint* shaders, int count);
