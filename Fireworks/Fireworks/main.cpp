// Uncomment for Win32 systems
// Search for and include the opengl32 and glu (GL-Utility) libraries
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
 
#include <iostream>
#include <ctime>
#include <time.h>         // Needed to seed the random number generator
 
//#include <windows.h> // *** IMPORTANT: Uncomment for Win32 systems - This must come -BEFORE- gl.h in the include list! ***
 
#include "GLee.h"
#include <GL\glfw.h>
 
#include "Firework.h"    // Include our fireworks class
 
using namespace std;
 
GLint windowWidth  = 800; // Define our window width
GLint windowHeight = 600; // Define our window height
GLint frameCount   = 0;     // Keep track of how many frames we've drawn
GLuint texture;                 // Identifier for our texture
 
const int FIREWORKS = 15; // Number of fireworks
 
// Create our array of fireworks
Firework fw[FIREWORKS];
 
// Function to set some initial OpenGL state-machine properties
void initGL()
{
    glfwSwapInterval(1); // Lock to vertical sync of monitor (normally 60Hz, so 60fps)
 
    // ----- Window and Projection Settings -----
 
    // Set the window title
    glfwSetWindowTitle("GLFW Fireworks with Trails");
 
    // Setup our viewport to be the entire size of the window
    glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
 
    // Change to the projection matrix, reset the matrix and set up orthagonal projection (i.e. 2D)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, 0, 1); // Parameters: left, right, bottom, top, near, far
 
    //  Enable smooth shading (i.e. gouraud shading)
    glShadeModel(GL_SMOOTH);
 
    // Set our clear colour to black with full opacity. Syntax is (r, g, b, alpha).
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    // Disable depth testing (because we're working in 2D!)
    glDisable(GL_DEPTH_TEST);
 
    // Enable blending (we need this to be able to use an alpha component)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    // Set the accumulation buffer clearing colour to black at 0,0f alpha
    glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);
 
    glEnable(GL_POINT_SMOOTH); // Smooth the points so that they're circular and not square
 
    glPointSize(5.0f);
 
    // Enable two dimensional texturing
    //checkGLError("InitGL");
	
}
 
// Function to draw our OpenGL scene
int drawScene()
{
    // Set our clear mode to clear the screen -only-
    glClear(GL_COLOR_BUFFER_BIT);
 
    // Take the contents of the current accumulation buffer and copy it to the colour buffer with each pixel multiplied by a factor
    // i.e. we clear the screen, draw the last frame again (which we saved in the accumulation buffer), then draw our stuff at its new location on top of that
    glAccum(GL_RETURN, 0.99f);
 
    // Clear the accumulation buffer (don't worry, we re-grab the screen into the accumulation buffer after drawing our current frame!)
    glClear(GL_ACCUM_BUFFER_BIT);
 
    // Set ModelView matrix mode and reset to the default identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Displacement trick for exact pixelisation
    glTranslatef(0.375, 0.375, 0);
 
    // Draw our fireworks
    for (int loop = 0; loop < FIREWORKS; loop++)
    {
        for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
        {
 
            // Set the point size of the firework particles (this needs to be called BEFORE opening the glBegin(GL_POINTS) section!)
            glPointSize(fw[loop].particleSize);
 
            glBegin(GL_POINTS);
                // Set colour to yellow on the way up, then whatever colour firework should be when exploded
                if (fw[loop].hasExploded == false)
                {
                    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
                }
                else
                {
                    glColor4f(fw[loop].red, fw[loop].green, fw[loop].blue, fw[loop].alpha);
                }
 
                // Draw the point
                glVertex2f(fw[loop].x[particleLoop], fw[loop].y[particleLoop]);
            glEnd();
        }
 
        // Move the firework appropriately depending on its explosion state
        if (fw[loop].hasExploded == false)
        {
            fw[loop].move();
        }
        else
        {
            fw[loop].explode();
        }
    }
 
    // ----- Stop Drawing Stuff! ------
 
    glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)
 
    // Take the contents of the current draw buffer and copy it to the accumulation buffer with each pixel modified by a factor
    // The closer the factor is to 1.0f, the longer the trails... Don't exceed 1.0f - you get garbage!
    glAccum(GL_ACCUM, 0.99f);
 
    // Return openGL error status
    return 0;
 
} // End of drawScene function
 
// Our main function
int main()
{
	srand((unsigned)time(NULL)); // Seed the random number generator
 
	// Define our buffer settings
    int	redBits     = 8,   greenBits = 8,    blueBits    = 8;
    int alphaBits  = 64, depthBits = 24,   stencilBits = 8;
 
    // Flag to keep our main loop running
    bool running = true;
 
    // Initialise glfw
    glfwInit();
 
    // Create a window
    if(!glfwOpenWindow(windowWidth, windowHeight, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW))
    {
        cout << "Failed to open window!" << endl;
        glfwTerminate();
        return 0;
    }
 
    // Call our initGL function to set up our OpenGL options
    initGL();
 
    while (running == true)
    {
        // Draw our scene
        drawScene();
 
		// Increase our frame counter
        frameCount++;
 
        // Exit if ESC was pressed or the window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
 
    glfwTerminate();
 
    return 0;
}