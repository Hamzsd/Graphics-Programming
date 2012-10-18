#pragma once

#include <vector>
#include <glew.h>
#include <glm\glm.hpp>
#include <GL\glfw.h>
#include <glm\gtc\type_ptr.hpp>//for mat4
#include <glm\gtc\matrix_transform.hpp>

struct geometry
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	
	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	

	geometry()
		: vertexBuffer(0), normalBuffer(0), vao(0), indexBuffer(0)
	{
	}

	~geometry()
	{
		if (vertexBuffer) glDeleteBuffers(1, &vertexBuffer);
		if (normalBuffer) glDeleteBuffers(1, &normalBuffer);
		if (indexBuffer)glDeleteBuffers(1, &indexBuffer);
		if (vao) glDeleteVertexArrays(1, &vao);
	}
};

struct transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	transform()
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void move(const glm::vec3& movement)
	{
		position += movement; 
	}

	void rotate(float angle, const glm::vec3& axis)
	{
		rotation = glm::rotate(rotation, glm::degrees(angle), axis);
	}

	glm::mat4 getTransformationMatrix() const
	{
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
		matrix = glm::scale(matrix, scale);
		glm::mat4 rotMatrix = glm::mat4_cast(rotation);
		matrix *= rotMatrix;
		return matrix;
	}
};

struct material
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	float padding[3];
};

struct render_object
{
	geometry* geometry;
	transform transform;
	material* material;
	GLuint materialBuffer;
};

geometry* createBox();
geometry* createTetrahedron();
geometry* createPyramid();
geometry* createDisk(int slices);
geometry* createCylinder(int stacks, int slices);
geometry* createSphere(int stacks, int slices);
geometry* createTorus(float radius, int stacks, int slices);
geometry* createPlane(int width, int depth);
geometry* createSierpinski(int divisions);
geometry* createVertexBuffer(geometry* geom);
geometry* createIndexBuffer(geometry* geom);
geometry* createNormalBuffer(geometry* geom);

