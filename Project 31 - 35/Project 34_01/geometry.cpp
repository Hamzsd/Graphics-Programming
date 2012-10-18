#include "geometry.h"
#include <glm\gtx\constants.hpp>

//create box code
glm::vec3 box_vertices[8] =
{
		glm::vec3(-1.0f, 1.0f, 1.0f),		// 0
		glm::vec3(1.0f, 1.0f, 1.0f),		// 1
		glm::vec3(1.0f, -1.0f, 1.0f),		// 2
		glm::vec3(-1.0f, -1.0f, 1.0f),		// 3
		glm::vec3(1.0f, 1.0f, -1.0f),		// 4
		glm::vec3(-1.0f, 1.0f, -1.0f),		// 5
		glm::vec3(-1.0f, -1.0f, -1.0f),		// 6
		glm::vec3(1.0f, -1.0f, -1.0f)		// 7

};

unsigned int box_indices[36] = 
{
	// Side 1
	0, 1, 3,
	1, 2, 3,
	//Side 2
	1, 4, 2,
	4, 7, 2,
	// Side 3
	4, 5, 7,
	5, 6, 7,
	// Side 4
	5, 0, 6,
	0, 3, 6,
	// Side 5
	5, 4, 0,
	4, 1, 0,
	// Side 6
	3, 2, 6,
	2, 7, 6
};

geometry* createBox()
{
	geometry* geom = new geometry();
	for(int i = 0; i < 8; ++i)
		geom->vertices.push_back(box_vertices[i]);
	for (int i = 0; i < 36; ++i)
		geom->indices.push_back(box_indices[i]);

	createVertexBuffer(geom);
	createIndexBuffer(geom);
	
	return geom;
}

//tetrahedron code
glm::vec3 tetrahedron_vertices[4] = 
{
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(0.0f, -1.0f, -1.0f)
};

unsigned int tetrahedron_indices[12] = 
{
	// Side 1
	0, 2, 1, 
	// Side 2
	0, 3, 2, 
	//Side 3
	0, 1, 3, 
	// Bottom
	1, 2, 3
};

geometry* createTetrahedron()
{
	geometry* geom = new geometry();
	for (int i = 0; i < 4; ++i)
		geom->vertices.push_back(tetrahedron_vertices[i]);
	for (int i = 0; i < 12; ++i)
		geom->indices.push_back(tetrahedron_indices[i]);
	
	createVertexBuffer(geom);
	createIndexBuffer(geom);

	return geom;
}

//pyramid code
glm::vec3 pyramid_vertices[5] = 
{
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f)
};

unsigned int pyramid_indices[18] = 
{
	// Side 1
	0, 2, 1,
	// Side 2
	0, 3, 2,
	// Side 3
	0, 4, 3, 
	// Side 4
	0, 1, 4, 
	// Bottom
	1, 3, 4, 
	1, 2, 3
};

geometry* createPyramid()
{
	geometry* geom = new geometry();
	for (int i = 0; i < 5; ++i)
		geom->vertices.push_back(pyramid_vertices[i]);
	for (int i = 0; i < 18; ++i)
		geom->indices.push_back(pyramid_indices[i]);

	createVertexBuffer(geom);
	createIndexBuffer(geom);

	return geom;
}

//create a disk
geometry* createDisk(int slices)
{
	geometry* geom = new geometry();
	glm::vec3 centre(0.0f, 0.0f, 0.0f);
	glm::vec3 vertex_prev(1.0f, 0.01, 0.0f);
	glm::vec3 vertex_current;
	float deltaAngle = (2.0f * glm::pi<float>()) / slices;
	for (int i = 1; i <= slices; ++i)
	{
		vertex_current = glm::vec3(cos(i * deltaAngle), 0.0f, sin(i * deltaAngle));
		geom->vertices.push_back(centre);
		geom->vertices.push_back(vertex_prev);
		geom->vertices.push_back(vertex_current);
		vertex_prev = vertex_current;
	}

	createVertexBuffer(geom);

	return geom;
}

//create cylinder
geometry* createCylinder(int stacks, int slices)
{
	geometry* geom = new geometry();
	//Create top
	glm::vec3 centre(0.0f, 1.0f, 0.0f);
	glm::vec3 vertex_prev(1.0f, 1.0, 0.0f);
	glm::vec3 vertex_current;
	float deltaAngle = (2 * glm::pi<float>()) / slices;
	for (int i = 1; i <= slices; ++i)
	{
		vertex_current = glm::vec3(cos(i * deltaAngle), 1.0f, sin(i * deltaAngle));
		geom->vertices.push_back(centre);
		geom->vertices.push_back(vertex_prev);
		geom->vertices.push_back(vertex_current);
		vertex_prev = vertex_current;
	}
	
	//create Bottom
	centre = glm::vec3(0.0f, -1.0f, 0.0f);
	vertex_prev = glm::vec3(1.0f, -1.0f, 0.0f);
	for (int i = 1; i <= slices; ++i)
	{
		vertex_current = glm::vec3(cos(i * deltaAngle), -1.0f, sin(i * deltaAngle));
		geom->vertices.push_back(centre);
		geom->vertices.push_back(vertex_prev);
		geom->vertices.push_back(vertex_current);
		vertex_prev = vertex_current;
	}

	//Create Stack
	glm::vec3 vertices[4];
	float deltaHeight = 2.0f / stacks;
	for (int i = 0; i < stacks; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			vertices[0] = glm::vec3(cos(j * deltaAngle), 1.0f - (deltaHeight * i), sin(j * deltaAngle));
			vertices[1] = glm::vec3(cos((j + 1) * deltaAngle), 1.0f - (deltaHeight * i), sin((j + 1) * deltaAngle));
			vertices[2] = glm::vec3(cos(j * deltaAngle), 1.0f - (deltaHeight * (i + 1)), sin(j * deltaAngle));
			vertices[3] = glm::vec3(cos((j + 1) * deltaAngle), 1.0f - (deltaHeight *(i + 1)), sin((j + 1) * deltaAngle));
			//triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->vertices.push_back(vertices[3]);
			geom->vertices.push_back(vertices[2]);
			//triangle 2
			geom->vertices.push_back(vertices[0]);
			geom->vertices.push_back(vertices[1]);
			geom->vertices.push_back(vertices[3]);
		}
	}

	createVertexBuffer(geom);

	return geom;
}

//Create sphere
geometry* createSphere(int stacks, int slices)
{
	geometry* geom = new geometry();
	//create required values
	float deltaRho = glm::pi<float>() / stacks;
	float deltaTheta = 2.0f * glm::pi<float>() / slices;
	for (int i = 0; i < stacks; ++i)
	{
		float rho = i * deltaRho;
		glm::vec3 vertices[4];
		for (int j = 0; j < slices; ++j)
		{
			// Vertex 0 
			float theta = j * deltaTheta; 
			vertices[0] = glm::vec3(-sin(theta) * sin(rho), cos(theta) * sin(rho), cos(rho));
			// Vertex 1
			vertices[1] = glm::vec3(-sin(theta) * sin(rho + deltaRho), cos(theta) * sin(rho + deltaRho), cos(rho + deltaRho));
			// Vertex 2
			theta = ((j + 1) == slices) ? 0.0f : (j + 1) * deltaTheta;
			vertices[2] = glm::vec3(-sin(theta) * sin(rho), cos(theta) * sin(rho), cos(rho));
			// Vertex 3
			vertices[3] = glm::vec3(-sin(theta) * sin(rho + deltaRho), cos(theta) * sin(rho + deltaRho), cos(rho + deltaRho));

			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->vertices.push_back(vertices[1]);
			geom->vertices.push_back(vertices[2]);
			// Triangle 2
			geom->vertices.push_back(vertices[1]);
			geom->vertices.push_back(vertices[3]);
			geom->vertices.push_back(vertices[2]);
		}
	}

	createVertexBuffer(geom);

	return geom;
}

//create Torus
geometry* createTorus(float radius, int stacks, int slices)
{
	geometry* geom = new geometry();
	float deltaStack = 2.0f * glm::pi<float>() / stacks;
	float deltaSlice = 2.0f * glm::pi<float>() / slices;
	for (int i = 0; i < stacks; ++i)
	{
		float a0 = i * deltaStack;
		float a1 = a0 + deltaStack;
		glm::vec3 vertices[4];
		for (int j = 0; j <= slices; ++j)
		{
			float c = cos(j * deltaSlice);
			float r = c + radius;
			vertices[0] = glm::vec3(sin(a0) * r, sin(j * deltaSlice), cos(a0) * r);
			vertices[1] = glm::vec3(sin(a1) * r, sin(j * deltaSlice), cos(a1) * r);
			c = cos((j + 1) * deltaSlice);
			r = c + radius;
			vertices[2] = glm::vec3(sin(a0) * r, sin((j + 1) * deltaSlice), cos(a0) * r);
			vertices[3] = glm::vec3(sin(a1) * r, sin((j + 1) * deltaSlice), cos(a1) * r);
			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->vertices.push_back(vertices[1]);
			geom->vertices.push_back(vertices[2]);
			// Triangle 2
			geom->vertices.push_back(vertices[1]);
			geom->vertices.push_back(vertices[3]);
			geom->vertices.push_back(vertices[2]);
		}
	}

	createVertexBuffer(geom);

	return geom;
}

//create Plane
geometry* createPlane(int width, int depth)
{
	geometry* geom = new geometry();
	glm::vec3 vertices[4];
	for (int x = 0; x < width; ++x)
	{
		for (int z = 0; z < depth; ++z)
		{
			vertices[0] = glm::vec3(-((float)width / 2.0f) + x, 0.0f, -((float)depth / 2.0f) + z);
			vertices[1] = glm::vec3(-((float)width / 2.0f) + (x + 1), 0.0f, -((float)depth / 2.0f) + z);
			vertices[2] = glm::vec3(-((float)width / 2.0f) + x, 0.0f, -((float)depth / 2.0f) + (z + 1));
			vertices[3] = glm::vec3(-((float)width / 2.0f) + (x+1), 0.0f, -((float)depth / 2.0f) + (z + 1));
			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->vertices.push_back(vertices[3]);
			geom->vertices.push_back(vertices[2]);
			// Triangle 2
			geom->vertices.push_back(vertices[0]);
			geom->vertices.push_back(vertices[1]);
			geom->vertices.push_back(vertices[3]);
		}
	}
	
	createVertexBuffer(geom);

	return geom;
}

//Create serpinski
//TODO finish this and get it working and then add in use of vertex array
void divide_sierpinski(geometry* geom, const glm::vec3 vertices[4], int count)
{
	int j;
	glm::vec3 ab[3], bc[3], ac[3], ad[3], bd[3], cd[3]; //midpoints of tetrahedron

	if (count>0)
	{
		for(j = 0; j<3; j++) ab[j] = (vertices[0] + vertices[1])/glm::vec3(2.0f, 2.0f,2.0f);
		for(j = 0; j<3; j++) bc[j] = (vertices[1] + vertices[3])/glm::vec3(2.0f, 2.0f,2.0f);
		for(j = 0; j<3; j++) ac[j] = (vertices[0] + vertices[3])/glm::vec3(2.0f, 2.0f,2.0f);
		for(j = 0; j<3; j++) ad[j] = (vertices[0] + vertices[4])/glm::vec3(2.0f, 2.0f,2.0f);
		for(j = 0; j<3; j++) bd[j] = (vertices[1] + vertices[4])/glm::vec3(2.0f, 2.0f,2.0f);
		for(j = 0; j<3; j++) cd[j] = (vertices[3] + vertices[4])/glm::vec3(2.0f, 2.0f,2.0f);

		for (int i = 0; i < 3; ++i)
		{
		geom->vertices.push_back(ab[i]);
		geom->vertices.push_back(bc[i]);
		geom->vertices.push_back(ac[i]);
		geom->vertices.push_back(ad[i]);
		geom->vertices.push_back(bd[i]);
		geom->vertices.push_back(cd[i]);
		}
		
		//divide_sierpinski(geom,tetrahedron_vertices, count-1);
		/*divide_sierpinski(geom, ab, count-1);
		divide_sierpinski(geom, bc, count-1);
		divide_sierpinski(geom, ac, count-1);
		divide_sierpinski(geom, ad, count-1);
		divide_sierpinski(geom, bd, count-1);
		divide_sierpinski(geom, cd, count-1);*/
	}
	else
		for (int i = 0; i < 4; ++i)
			geom->vertices.push_back(tetrahedron_vertices[i]);
		for (int i = 0; i < 12; ++i)
			geom->indices.push_back(tetrahedron_indices[i]);

	////when count is 0 
	//if (count == 0)
	//{
	//	for (int i = 0; i < 4; ++i)
	//	geom->vertices.push_back(tetrahedron_vertices[i]);
	//for (int i = 0; i < 12; ++i)
	//	geom->indices.push_back(tetrahedron_indices[i]);
	//}
}

geometry* createSierpinski(int divisions)
{
	glm::vec3 vertices[4];
	std::memcpy(vertices, tetrahedron_vertices, sizeof(glm::vec3) * 4);
	geometry* geom = new geometry();
	divide_sierpinski(geom, vertices, divisions);
	return geom;
}

geometry* createVertexBuffer(geometry* geom)
{
	glGenVertexArrays(1, &geom->vao);
	glBindVertexArray(geom->vao);

	glGenBuffers(1, &geom->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geom->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, geom->vertices.size() * sizeof(glm::vec3), &geom->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return geom;
}

geometry* createIndexBuffer(geometry* geom)
{
	glGenBuffers(1, &geom->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom->indices.size() * sizeof(unsigned int), &geom->indices[0],GL_STATIC_DRAW);

	return geom;
}