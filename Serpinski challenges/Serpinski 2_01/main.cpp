#pragma comment(lib, "GLFW")
#pragma comment(lib, "OpenGL32")

#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\constants.hpp>
#include <cstdlib>
#include <vector>

bool running = true;
std::vector<glm::vec2> vertices;

void triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
	vertices.push_back(a);
	vertices.push_back(b);
	vertices.push_back(c);
}

void divide_triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, int count)
{
	glm::vec2 v0, v1, v2;
	int j;
	if(count>0)
	{
		for(j=0; j<2; j++) v0[j]=(a[j]+b[j])/2;
		for(j=0; j<2; j++) v1[j]=(a[j]+c[j])/2;
		for(j=0; j<2; j++) v2[j]=(b[j]+c[j])/2;
		divide_triangle(a, v0, v1, count-1);
		divide_triangle(c, v1, v2, count-1);
		divide_triangle(b, v2, v0, count-1);
	}
	else triangle(a,b,c); /* draw triangle at end of recursion */

	//draw 1 triangle
	/*if (count == 0)
	{
		triangle(a, b, c);
	}*/
}

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_VERTEX_ARRAY);
	glm::vec2 vertices[3] =
	{
		glm::vec2(-1.0f, -1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, -1.0f)
	};
	divide_triangle(vertices[0], vertices[1], vertices[2], 5);
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glfwSwapBuffers();
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

	glfwTerminate();

	exit(EXIT_SUCCESS);
}