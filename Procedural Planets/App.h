#pragma once

// find library for vector matrizes etc...
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Include GLEW
#include <GL/glew.h>
#include <AntTweakBar.h>




#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "RenderObject.h"
#include "ObjQuadTiles.h"

class App
{
private:
	Mesh *firstObj;
	Shader *shader;
	Camera *m_camera;
	ResourceManager *m_resource;
	RenderManager *m_render;
	RenderObject *m_defered_n;
	RenderObject *m_atmosphere;
	RenderObject *m_ocean;
	RenderObject *m_squared_n;
    RenderObject *m_plane;
	RenderObject *m_sun;
	RenderObject* m_secondMoon;
	RenderObject* m_thirdMoon;
	RenderObject* m_fourthMoon;
	RenderObject* m_fifthMoon;
	RenderObject *m_moon;
	GLFWwindow* m_window;
	// Active shader
	GLuint programID;
	TwBar* m_TwBar;
    //Time
    Timer * m_time;

	float defered;
	float squared;

	bool ocean;
	bool atmosphere;
public:
	App(GLFWwindow* window, TwBar* TwBar);
	~App();

	void onUpdate();
	void onInit();
	void onShutdown();
};