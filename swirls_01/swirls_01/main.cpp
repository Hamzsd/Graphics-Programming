// karsithe (hazelmckendrick@gmail.com, http://hazelmckendrick.com)
// cheers to http://in4k.untergrund.net/ and http://www.iquilezles.org/www/

//#define _FATTER_ //also need to link to winmm.lib
#ifndef _FATTER_
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_LEAN
#endif

#include <windows.h>
#include <glew.h>
#include <GL/gl.h>
#include "glext.h"
#include <GL\glfw.h>


// Vertex Shader
// If anyone sees a good way to get rid of this please let me know
// But losing it and having to use a uniform in the frag shader for screen width was actually worse :(
const static GLchar* shaderVert =
    "varying vec3 p;"
    "void main(){"
        "p=gl_Position=gl_Vertex;"
        "p.z=gl_Color.x;"
    "}";

// Pixel Shader
const static GLchar* shaderFrag =
    "varying vec3 p;"
    "void main(){"
        "int l=0;"
        "float s=0;"
        "while(s==0&&l<5){"
            "l++;"
            "for(int i=0;i<9-l;i++)"
            "{"
                "float x=sin(cos(i*2*l-p.z*2)),"
                "y=sin(cos(i*9*l+p.z*3));"
                "s+=(sin(p.z*10/l+i+l)*0.2*l+0.04)/sqrt((p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));"
            "}"
            "if(s<0.3/l||s>0.8)s=0;"  
        "}"
        "int si=s*15;"
        "s=si/15.0;"
        "vec3 c=vec3(sin(p.z*8+p.x)+0.3,cos(p.z*8+p.y)+0.3,0.7)*s/l;"
        "gl_FragColor=vec4(c,1);"
    "}";

PIXELFORMATDESCRIPTOR pfd = {0,0,PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#ifdef _DEBUG
    int WinMain(HINSTANCE hInstance,
                 HINSTANCE hPrevInstance,
                 LPSTR lpCmdLine,
                 int nCmdShow)
#else
    void WinMainCRTStartup()
#endif
{
    // Set up the window
    HDC hDC = GetDC(CreateWindow("edit",0, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 0,0,0,0,0,0,0,0));
    SetPixelFormat(hDC,ChoosePixelFormat(hDC,&pfd),&pfd);
    wglMakeCurrent(hDC,wglCreateContext(hDC));
    ShowCursor(FALSE);

    // Set up shaders
    static GLuint v = ((PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader"))( GL_VERTEX_SHADER );
    static GLuint f = ((PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader"))( GL_FRAGMENT_SHADER );
    static GLuint p = ((PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram"))();
    ((PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource"))( v,1,&shaderVert,NULL );
    ((PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource"))( f,1,&shaderFrag,NULL );
    ((PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader"))(v);
    ((PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader"))(f);
    ((PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader"))(p,v);
    ((PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader"))(p,f);
    ((PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram"))(p);
    ((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))(p);

    float j=0.f;
    do {
#ifdef _FATTER_
        glColor3f(timeGetTime()/30000.f,0,0); 
#else
        glColor3f(j+=0.0001f,0,0); // For when you can't afford windows stuff
#endif
        glRecti(-1,-1,1,1); // "render target" ;)
        SwapBuffers(hDC);
    } while(!GetAsyncKeyState(VK_ESCAPE));

    ExitProcess(0);
    
}
