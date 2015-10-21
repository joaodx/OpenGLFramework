// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <AntTweakBar.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#define GLFW_CDECL
#include <GLFW\glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

//***********************************************************************//
#include "App.h"
#include "Timer.h"
//***********************************************************************//

using namespace glm;

//************************ Variables ****************************************** //

TwBar* m_TwBar;
const int m_WindowWidth = 1024;
const int m_WindowHeight = 768;
App *app;
Timer *m_time;
float fps;

bool NormalPlanet = false;
//***************************************************************************** //

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods){TwEventMouseButtonGLFW(button, action);}
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos){TwMouseMotion(int(xpos), int(ypos));}
inline void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset){TwEventMouseWheelGLFW(yoffset);}
inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods){TwEventKeyGLFW(key, action);}
inline void TwEventCharGLFW3(GLFWwindow* window, int codepoint){TwEventCharGLFW(codepoint, GLFW_PRESS);}

int main( void )
{
    printf("GL ERROR: %d \n", glGetError());
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
    
	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( m_WindowWidth, m_WindowHeight, "OpenGL Framework", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
    
	//// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window,GLFW_CURSOR,GL_TRUE);

	
	//------ TW bar INIT ------------------------//

	TwInit(TW_OPENGL_CORE, NULL);	
	m_TwBar = TwNewBar("Controls");
	TwDefine(" Controls refresh=0.05 ");
	TwWindowSize(m_WindowWidth, m_WindowHeight);

	// after GLFW initialization
	// directly redirect GLFW events to AntTweakBar
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);
	//--------------------------------------------//

	// Dark blue background
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//-------- APP init ------------------------------------------//
	m_time = new Timer();
	app = new App(window,m_TwBar);
	app->onInit();

	TwAddVarRO(m_TwBar, "FPS" , TW_TYPE_FLOAT, &fps, "help='Number of frames per second'  ");

	//------------------------------------------------------------//
	m_time->start();
	float start = m_time->getCurrentTime();
	int frames = 0;
    
	do{
		++frames;
		float elapsedTime = m_time->time();// / 1000.0f;

		if( elapsedTime >= 1.0)
		{
			fps = frames;
			frames = 0;
			m_time->start();		
		}

		// Draw nothing, see you in tutorial 2 !
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

		app->onUpdate();

		TwDraw(); //draw tweakbar

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
       
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	app->onShutdown();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

