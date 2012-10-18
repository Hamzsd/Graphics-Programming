#pragma once

#include <vector>
#include <glew.h>
#include <GL\glfw.h>
#include <glm\glm.hpp>

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
geometry* createSphere(int divisions);