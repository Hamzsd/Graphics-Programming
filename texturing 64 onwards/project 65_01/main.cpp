#pragma comment(lib, "DevIL")
#pragma comment(lib, "ILU")
#pragma comment(lib, "ILUT")

#include <IL\ilut.h>

GLuint image;

int main()
{
	// initilise the image library
	ilInit();
	// initialise imae library utilities
	iluInit();
	// initialise openGL usage
	ilutRenderer(ILUT_OPENGL);
	 //load image
	image = ilutGLLoadImage((wchar_t*)"brick.jpg");
	// check fr any error
	ILenum error = ilGetError();
	//display the error message
	printf("%s\n", iluErrorString(error));

	return 0;
}