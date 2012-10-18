#include "geometry.h"
#include <glm\gtx\constants.hpp>
#include <iostream>

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
	return geom;
}

//Create serpinski
void divide_sierpinski(geometry* geom, const glm::vec3 vertices[4], int count)
{
	int j; 
	glm::vec3 ab, bc, ac, ad, bd, cd;
	glm::vec3 tempVert[4];
	glm::vec3 tempVert1[4];
	glm::vec3 tempVert2[4];
	glm::vec3 tempVert3[4];

	if(count > 0)
	{
		ab = (vertices[0] + vertices[1])/glm::vec3(2,2,2);
		ac = (vertices[0] + vertices[2])/glm::vec3(2,2,2);         
		ad = (vertices[0] + vertices[3])/glm::vec3(2,2,2);         
		bc = (vertices[1] + vertices[2])/glm::vec3(2,2,2);         
		bd = (vertices[1] + vertices[3])/glm::vec3(2,2,2);         
		cd = (vertices[2] + vertices[3])/glm::vec3(2,2,2);         

		tempVert[0] = vertices[0];
		tempVert[1] = ab;
		tempVert[2] = ac;
		tempVert[3] = ad;
		
		tempVert1[0] = vertices[1];
		tempVert1[1] = ab;
		tempVert1[2] = bc;
		tempVert1[3] = bd;

		tempVert2[0] = vertices[2];
		tempVert2[1] = ac;
		tempVert2[2] = bc;
		tempVert2[3] = cd;

		tempVert3[0] = vertices[3];
		tempVert3[1] = ad;
		tempVert3[2] = bd;
		tempVert3[3] = cd; 

		 divide_sierpinski(geom, tempVert, count-1);
		 divide_sierpinski(geom, tempVert1, count-1);
		 divide_sierpinski(geom, tempVert2, count-1);
		 divide_sierpinski(geom, tempVert3, count-1);		
	}
	else
	{
		for(int i = 0; i < 4; ++i) geom->vertices.push_back(vertices[i]);
		for(int i = 0; i < 12; ++i) geom->indices.push_back(tetrahedron_indices[i]);
		for(int i = 0; i < 12; ++i) tetrahedron_indices[i] = tetrahedron_indices[i] + 4;
	}
	
}

geometry* createSierpinski(int divisions)
{
	glm::vec3 vertices[4];
	std::memcpy(vertices, tetrahedron_vertices, sizeof(glm::vec3) * 4);
	geometry* geom = new geometry();
	divide_sierpinski(geom, vertices, divisions);
	return geom;
}

glm::vec3 getMidPoint(glm::vec3 a, glm::vec3 b, glm::vec3 c[3])
{
	for (int i = 0; i < 3; ++i)
	{
		c[i] = a[i] + b[i] / glm::vec3(2.0f, 2.0f, 2.0f);
	}
	return c[3];
}





//check midpoint debug code
//		std::cout<<"this is now what the midpoints for ab are before the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//		
//		ab[3] = getMidPoint(vertices[0], vertices[1], ab);
//
//		std::cout<<"this is now what the midpoints for ab are after the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//
//		std::cout<<"this is now what the midpoints for bc are before the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what bc["<<i<<"] = "<<bc[i].x<<" , "<<bc[i].y<<" , "<<bc[i].z<<std::endl;
//	
//		bc[3] = getMidPoint(vertices[1], vertices[2], bc);
//
//		std::cout<<"this is now what the midpoints for bc are after the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what bc["<<i<<"] = "<<bc[i].x<<" , "<<bc[i].y<<" , "<<bc[i].z<<std::endl;
//		
//		std::cout<<"this is now what the midpoints for ac are before the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ac["<<i<<"] = "<<ac[i].x<<" , "<<ac[i].y<<" , "<<ac[i].z<<std::endl;
//		
//		ac[3] = getMidPoint(vertices[0], vertices[2], ac);
//
//		std::cout<<"this is now what the midpoints for ac are after the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ac["<<i<<"] = "<<ac[i].x<<" , "<<ac[i].y<<" , "<<ac[i].z<<std::endl;
//
//		std::cout<<"this is now what the midpoints for ad are before the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ad["<<i<<"] = "<<ad[i].x<<" , "<<ad[i].y<<" , "<<ad[i].z<<std::endl;
//		
//		ad[3] = getMidPoint(vertices[0], vertices[3], ad);
//
//		std::cout<<"this is now what the midpoints for ad are after the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ad["<<i<<"] = "<<ad[i].x<<" , "<<ad[i].y<<" , "<<ad[i].z<<std::endl;
//
//		std::cout<<"this is now what the midpoints for bd are before the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what bd["<<i<<"] = "<<bd[i].x<<" , "<<bd[i].y<<" , "<<bd[i].z<<std::endl;
//
//		bd[3] = getMidPoint(vertices[1], vertices[3], bd);
//
//		std::cout<<"this is now what the midpoints for bd are after the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what bd["<<i<<"] = "<<bd[i].x<<" , "<<bd[i].y<<" , "<<bd[i].z<<std::endl;
//
//		std::cout<<"this is now what the midpoints for cd are before the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what cd["<<i<<"] = "<<cd[i].x<<" , "<<cd[i].y<<" , "<<cd[i].z<<std::endl;
//		
//		cd[3] = getMidPoint(vertices[2], vertices[3], cd);
//
//		std::cout<<"this is now what the midpoints for cd are after the getMidPoint method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what cd["<<i<<"] = "<<cd[i].x<<" , "<<cd[i].y<<" , "<<cd[i].z<<std::endl;

//Refactored code
		//ab[3] = getMidPoint(vertices[0], vertices[1], ab);
		//bc[3] = getMidPoint(vertices[1], vertices[2], bc);
		//ac[3] = getMidPoint(vertices[0], vertices[2], ac);
		//ad[3] = getMidPoint(vertices[0], vertices[3], ad);
		//bd[3] = getMidPoint(vertices[1], vertices[3], bd);
		//cd[3] = getMidPoint(vertices[2], vertices[3], cd);
		//		
		//divide_sierpinski(geom, ab, count-1); //Not sure that this is right....
		//divide_sierpinski(geom, bc, count-1);
		//divide_sierpinski(geom, ac, count-1);
		//divide_sierpinski(geom, ad, count-1);
		//divide_sierpinski(geom, bd, count-1);
		//divide_sierpinski(geom, cd, count-1);





//DIVIDE SIERPINSKI CODE, CRAIGS CODE DO NOT REOMOVE! 


//MARKS CODE DONT DELETE, CALCULATES MIDPOINTS! 
	//glm::vec3 altvert[4];

 //if (count > 0)
 //{
 // for (int i = 0; i < 4; ++i) altvert[i] = vertices[i]/glm::vec3(2,2,2);
 // for (int i = 0; i < 4; ++i) geom->vertices.push_back(altvert[i]);


 // //for (int i = 0; i < 12; ++i) geom->indices.push_back(tetrahedron_indices[i]);
 // divide_sierpinski(geom, altvert, count-1);
 // divide_sierpinski(geom, altvert, count-1);
 // divide_sierpinski(geom, altvert, count-1);
 // divide_sierpinski(geom, altvert, count-1);

 // std::cout << " Vertice 1 : " << vertices[0].x << " , " << vertices[0].y << " , " << vertices[0].z << std::endl;
 // std::cout << " Vertice 2 : " << vertices[1].x << " , " << vertices[1].y << " , " << vertices[1].z << std::endl;
 // std::cout << " Vertice 3 : " << vertices[2].x << " , " << vertices[2].y << " , " << vertices[2].z << std::endl;
 // std::cout << " Vertice 4 : " << vertices[3].x << " , " << vertices[3].y << " , " << vertices[3].z << std::endl;

 // std::cout << " Vertice 1 : " << altvert[0].x << " , " << altvert[0].y << " , " << altvert[0].z << std::endl;
 // std::cout << " Vertice 2 : " << altvert[1].x << " , " << altvert[1].y << " , " << altvert[1].z << std::endl;
 // std::cout << " Vertice 3 : " << altvert[2].x << " , " << altvert[2].y << " , " << altvert[2].z << std::endl;
 // std::cout << " Vertice 4 : " << altvert[3].x << " , " << altvert[3].y << " , " << altvert[3].z << std::endl;


 //}
 //else 
 //{
 // for(int i = 0; i < 4; ++i) geom->vertices.push_back(altvert[i]);
 // for (int i = 0; i < 12; ++i) geom->indices.push_back(tetrahedron_indices[i]);
 //}


//divide seierpinski code, 01/10/2012
//void divide_sierpinski(geometry* geom, const glm::vec3 vertices[4], int count)
//{
//	int j;
//	glm::vec3 ab[3], bc[3], ac[3], ad[3], bd[3], cd[3]; //midpoints of tetrahedron
//	
//	if (count>0)
//	{
//		
//		std::cout<<"\nGetting midpoint of ab"<<std::endl;
//		ab[3] = getMidPoint(vertices[0], vertices[1]);
//		std::cout<<"this is now what the midpoints for ab are in the divide_sierpinski method"<<std::endl;
//		//for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//		
//		std::cout<<"\nGetting midpoint of bc"<<std::endl;
//		bc[3] = getMidPoint(vertices[1], vertices[2]);
//		std::cout<<"this is now what the midpoints for bc are in the divide_sierpinski method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what bc["<<i<<"] = "<<bc[i].x<<" , "<<bc[i].y<<" , "<<bc[i].z<<std::endl;
//		
//		std::cout<<"\nGetting midpoint of ac"<<std::endl;
//		ac[3] = getMidPoint(vertices[0], vertices[2]);
//		std::cout<<"this is now what the midpoints for ac are in the divide_sierpinski method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ac["<<i<<"] = "<<ac[i].x<<" , "<<ac[i].y<<" , "<<ac[i].z<<std::endl;
//		
//		std::cout<<"\nGetting midpoint of ad"<<std::endl;
//		ad[3] = getMidPoint(vertices[0], vertices[3]);
//		std::cout<<"this is now what the midpoints for ad are in the divide_sierpinski method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what ad["<<i<<"] = "<<ad[i].x<<" , "<<ad[i].y<<" , "<<ad[i].z<<std::endl;
//		
//		std::cout<<"\nGetting midpoint of bd"<<std::endl;
//		bd[3] = getMidPoint(vertices[1], vertices[3]);
//		std::cout<<"this is now what the midpoints for bd are in the divide_sierpinski method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what bd["<<i<<"] = "<<bd[i].x<<" , "<<bd[i].y<<" , "<<bd[i].z<<std::endl;
//		
//		std::cout<<"\nGetting midpoint of cd"<<std::endl;
//		cd[3] = getMidPoint(vertices[2], vertices[3]);
//		std::cout<<"this is now what the midpoints for bc are in the divide_sierpinski method"<<std::endl;
//		for(int i = 0; i<3; i++)std::cout<<"This is what cd["<<i<<"] = "<<cd[i].x<<" , "<<cd[i].y<<" , "<<cd[i].z<<std::endl;
//		
//		divide_sierpinski(geom, vertices, count-1);
//		std::cout<<"\nthis is now what the midpoints for ab are after divide 1"<<std::endl;
//		//for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//
//		divide_sierpinski(geom, vertices, count-1);
//		std::cout<<"\nthis is now what the midpoints for ab are after divide 2"<<std::endl;
//		//for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//
//		divide_sierpinski(geom, vertices, count-1);
//		std::cout<<"\nthis is now what the midpoints for ab are after divide 3"<<std::endl;
//		//for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//
//		divide_sierpinski(geom, vertices, count-1);
//		std::cout<<"\nthis is now what the midpoints for ab are after divide 4"<<std::endl;
//		//for(int i = 0; i<3; i++)std::cout<<"This is what ab["<<i<<"] = "<<ab[i].x<<" , "<<ab[i].y<<" , "<<ab[i].z<<std::endl;
//		
//	}
//	else
//	{		
//		for (int i = 0; i < 3; ++i)
//		{
//			geom->vertices.push_back(ab[i]);
//			//geom->vertices.push_back(bc[i]);
//			//geom->vertices.push_back(ac[i]);
//			//geom->vertices.push_back(ad[i]);
//			//geom->vertices.push_back(bd[i]);
//			//geom->vertices.push_back(cd[i]);
//		}		
//	}
//}