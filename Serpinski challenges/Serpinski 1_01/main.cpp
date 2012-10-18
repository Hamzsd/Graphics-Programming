#pragma comment(lib, "GLFW")
#pragma comment(lib, "OpenGL32")

#include <GL\glfw.h>
#include <cstdlib>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

bool running = true;
const int num_points = 50000;
glm::vec2 points[num_points];

void initialise()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	glm::vec2 v[3] =
	{
		glm::vec2(-1.0, -1.0),
		glm::vec2(0.0f, 1.0),
		glm::vec2(1.0, -1.0)
	};

	//points[0] = glm::vec2(0.25, 0.50);
	for (int i = 0; i < num_points; ++i)
	{
		points[i] =(points[i-1] + v[rand()%3]) / glm::vec2(2.0f, 2.0f);
	}
}

void update(double deltaTime)
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		for (int k = 0; k < num_points; ++k)
			glVertex2fv(glm::value_ptr(points[k]));
	glEnd();
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