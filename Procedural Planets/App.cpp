#include "App.h"



// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

App::App(GLFWwindow* window, TwBar* TwBar) : m_window(window), m_TwBar(TwBar),defered(false)
	,squared(true),ocean(true),atmosphere(true)
{


		TwAddVarRW(m_TwBar, "Squared Render Technique" , TW_TYPE_BOOL32, &squared, "help='Turn on square tile'  ");

}

App::~App()
{

}

void App::onInit()
{
	m_camera = new Camera(m_window);
	m_resource = new ResourceManager();
	m_render = new RenderManager(m_camera,m_TwBar);
	
	firstObj = m_resource->createMesh("media/cube.obj");

	//shader = m_resource->createShader("Shaders/vertexshader.glsl","Shaders/tessControl.glsl","Shaders/TessEvaluation.glsl",
	//									"Shaders/geometyShader2.glsl", "Shaders/fragmentshader.glsl");
	//Shader with simple output and input
	Shader *normalShader = m_resource->createShader("Shaders/vertexshader2.glsl","Shaders/tessControl2.glsl","Shaders/TessEvaluation2.glsl",
										"Shaders/geometyShader2.glsl", "Shaders/fragmentshader2.glsl");

	//defered Render
	Shader *shader3 = m_resource->createShader("Shaders/vertexshader.glsl","Shaders/tessControl.glsl","Shaders/TessEvaluation.glsl",
										"Shaders/geometyShader.glsl", "Shaders/fragmentshader.glsl");

	Shader *shader4 = m_resource->createShader("Shaders/vs2.glsl","Shaders/tcs2.glsl","Shaders/tes2.glsl",
										"Shaders/gs2.glsl", "Shaders/fs2.glsl");
	//not defered
	Shader *shader5 = m_resource->createShader("Shaders/vertexshaderSecond.glsl","Shaders/tessControlSecond.glsl","Shaders/TessEvaluationPlanetTest.glsl",
										"Shaders/geometyShaderSecond.glsl", "Shaders/fragmentshaderSecond.glsl");
	//square mesh
	Shader *shader6 = m_resource->createShader("Shaders/vsQuadTile.glsl","Shaders/tcsQuadTile.glsl","Shaders/tesQuadTile.glsl",
										"Shaders/gsQuadTile.glsl", "Shaders/fsQuadTile.glsl");
	//Atmosphere
	Shader *shader7 = m_resource->createShader("Shaders/AVertexShader.glsl","Shaders/ATCShader.glsl","Shaders/ATEShader.glsl",
										"Shaders/AGShader.glsl", "Shaders/AFragmentShader.glsl");
	//ocean
	Shader *shader8 = m_resource->createShader("Shaders/OVertexShader.glsl","Shaders/OTCShader.glsl","Shaders/OTEShader.glsl",
										"Shaders/OGShader.glsl", "Shaders/OFragmentShader.glsl");
	//sun
	Shader *shader9 = m_resource->createShader("Shaders/SVertexShader.glsl","Shaders/STCShader.glsl","Shaders/STEShader.glsl",
										"Shaders/SGShader.glsl", "Shaders/SFragmentShader.glsl");
	
	//moon
	Shader *shader10 = m_resource->createShader("Shaders/MVertexShader.glsl","Shaders/MTCShader.glsl","Shaders/MTEShader.glsl",
										"Shaders/MGShader.glsl", "Shaders/MFragmentShader.glsl");


	Mesh *normalMesh = m_resource->createMesh("media/sphere.obj");
	Mesh *ObjQuadTileMesh = new ObjQuadTiles("media/monkey.obj");
	Mesh *ObjAtmosphere = new ObjQuadTiles("media/monkey.obj");


	m_sun = new RenderObject(m_render, normalMesh, normalShader, 0);
	m_sun->translate(0.0,0.0,0.0);
	

	m_secondMoon = new RenderObject(m_render, normalMesh, normalShader, 0);
	m_secondMoon->translate(10.0, 0.0, .0);	
    m_secondMoon->setdiffuseColour(glm::vec4(1, 0, 0, 0));
    m_secondMoon->setSpecularColour(glm::vec4(1, 1, 0, 0));
    m_secondMoon->setAsLight();

	m_thirdMoon = new RenderObject(m_render, normalMesh, normalShader, 0);
	m_thirdMoon->translate(-10.0, 0.0, 0.0);
    m_thirdMoon->setdiffuseColour(glm::vec4(0, 1, 0, 0));
    m_thirdMoon->setSpecularColour(glm::vec4(1, 0, 0, 0));
    m_thirdMoon->setAsLight();

	m_fourthMoon = new RenderObject(m_render, normalMesh, normalShader, 0);
	m_fourthMoon->translate(0.0, 10.0, 0.0);	
    m_fourthMoon->setdiffuseColour(glm::vec4(1, 0, 1, 0));
    m_fourthMoon->setSpecularColour(glm::vec4(0, 0, 1, 0));
    m_fourthMoon->setAsLight();

	m_fifthMoon = new RenderObject(m_render, normalMesh, normalShader, 0);
	m_fifthMoon->translate(0.0, -10.0, 0.0);
    m_fifthMoon->setdiffuseColour(glm::vec4(0, 1, 0, 0));
    m_fifthMoon->setSpecularColour(glm::vec4(1, 0, 1, 0));
    m_fifthMoon->setAsLight();

	
	m_moon = new RenderObject(m_render,ObjAtmosphere,shader10,0);
	m_moon->translate(50.0,100.0,0.0);
	//m_moon->addToScene();

	m_squared_n = new RenderObject(m_render,ObjQuadTileMesh,shader6,0);
	//m_squared_n->addToScene();
	m_squared_n->translate(-100.0,1.0,10.0);

	//m_defered_n = new RenderObject(m_render,firstObj,shader3,0); // this is for defered render for use of shadows later on.
	//m_defered_n->addToScene();

	m_ocean = new RenderObject(m_render,ObjAtmosphere,shader8,0);
	//m_ocean->addToScene();

	m_atmosphere = new RenderObject(m_render,ObjAtmosphere,shader7,0);
	//m_atmosphere->addToScene();
	
	//m_squared_n->addChild(m_moon);
	//m_squared_n->addChild(m_ocean);
	//m_squared_n->addChild(m_atmosphere);

	
	
	//m_sun->addChild(m_ocean);
	//m_sun->addChild(m_squared_n);
	//m_sun->addChild(m_atmosphere);
	
	//m_sun->addChild(m_moon);
	//m_secondMoon->addToScene();	
    m_sun->addChild(m_secondMoon);
    m_sun->addChild(m_thirdMoon);
    m_sun->addChild(m_fourthMoon);
    m_sun->addChild(m_fifthMoon);
	m_sun->addToScene();
	/*m_secondMoon->addToScene();
    m_thirdMoon->addToScene();
	m_fourthMoon->addToScene();
	m_fifthMoon->addToScene();*/
	
	

	m_render->Update();
}

void App::onUpdate()
{
	
    //Animation    
    
    //m_secondMoon->translate(-0.02, 0, 0);
    glm::dquat quat(0.5,0,1,0);
    m_sun->rotate(quat);
    /*m_secondMoon->rotate(quat);
    m_thirdMoon->rotate(quat);
    quat = glm::dquat(5, 1, 0, 0);
    m_fourthMoon->rotate(quat);
    m_fifthMoon->rotate(quat);*/


	if(squared && !defered)
	{
		defered = false;
		//m_squared_n->visible(true);
		//m_ocean->visible(true);
		//m_atmosphere->visible(true);
		m_render->Update();
	}


	/*
	if(!ocean)
	{
		m_ocean->visible(false);
		m_render->Update();
	}


	if(!atmosphere)
	{
		m_atmosphere->visible(false);
		m_render->Update();
	}

	*/
	
	m_render->Render(defered, squared, false);
	//firstObj->draw();
	
   
}

void App::onShutdown()
{
	m_resource->unload();
}