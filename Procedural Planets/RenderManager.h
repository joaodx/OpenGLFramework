#ifndef _RENDERMANAGER_HPP
#define _RENDERMANAGER_HPP

#include "SceneManager.h"
#include "SceneGraph.h"
#include "Camera.h"
#include <vector>
#include <algorithm>

#include <gl\GL.h>
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <AntTweakBar.h>
#include "ShaderTexture.h"
#include "QuadMesh.h"



class RenderManager 
{
public:

    typedef NodeObj::gl_LightSourceParameters lightParameters;

	RenderManager(Camera *camera,TwBar* TwBar);
	~RenderManager();

	void AddNode(Node *node);
	void RemoveNode(Node *node);
	void SetView(glm::mat4 &view);
	void setCameraPosition(const glm::vec3 &cameraPos) { m_cameraPos = cameraPos; }
    void addLight(lightParameters * light);

	float getViewportWidth();
	float getViewportHeight();
	glm::mat4 getInvPerspective() { return glm::inverse(m_perspective); }
	glm::mat4 getInvView() { return glm::inverse(m_view); }

	unsigned createTexture(int w, int h, bool isDepth = false);
	unsigned createFBO();
	bool initRenderTarget();

	void Update();
	void Render(bool defered, bool squared, bool poolar);
	void RenderToTexture();

private:
	void Disable2DRender();
	void Enable2DRender();

	SceneManager *m_scene;
	SceneGraph *m_sceneGraph;
	Camera *m_camera;

	
	glm::mat4 m_perspective;
	glm::mat4 m_view;
	//const FWDisplayInfo &m_viewport;
	glm::vec3 m_colour, m_cameraPos;

	GLfloat m_ampgain;
	GLfloat m_frqgain;
	GLfloat m_initamp;
	GLfloat m_initfrq;
	GLfloat m_radius;
	GLfloat m_octvs;

	TwBar* m_TwBar;

	bool wireframe;

	unsigned FramebufferName;
	unsigned renderTexture;
	unsigned renderTexture2;

	ShaderTexture *m_st;
	ShaderTexture *m_st2;
	QuadMesh *quad;

	//Perlin options------//
	bool simplexNoise;
	bool rigidMultiFractal;
	bool rigid;
	bool multiFractal;
	bool perlinNoise;
	//--------------------//
	
};
#endif