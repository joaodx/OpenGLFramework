#include "RenderManager.h"


RenderManager::RenderManager(Camera *camera, TwBar* TwBar )
	:  m_camera(camera), m_TwBar(TwBar), wireframe(false)
{
	m_sceneGraph = new SceneGraph();
	m_scene = new SceneManager(m_sceneGraph->getRoot());
	m_ampgain = 0.43;
	m_frqgain = 1.65;
	m_initamp = 1.082;
	m_initfrq = 1.0;
	m_radius = 30.0;
	m_octvs = 10.0;

	simplexNoise = true;
	rigidMultiFractal = false;
	rigid = false;
	multiFractal = false;
	perlinNoise = false;

	//TwAddVarRO(GlobalClass::myBar, "Render Time Step" , TW_TYPE_FLOAT, &ellapsedTime, "");

	//TwAddVarRO(GlobalClass::myBar, "N of Block" , TW_TYPE_INT32, &number_blocks, "");
	//TwAddVarRO(GlobalClass::myBar, "N of Cannon Balls" , TW_TYPE_INT32, &number_balls, "");
	//TwAddVarRO(GlobalClass::myBar, "Type of Shoot" , TW_TYPE_INT32, &shoot, "");

	
	TwAddVarRW(m_TwBar, "WireFrame" , TW_TYPE_BOOLCPP, &wireframe, "help='Activate/Desactivate WireFrame'  ");
	
	TwAddVarRW(m_TwBar, "simplexNoise" , TW_TYPE_BOOLCPP, &simplexNoise, "");
	TwAddVarRW(m_TwBar, "rigidMultiFractal" , TW_TYPE_BOOLCPP, &rigidMultiFractal, "");
	TwAddVarRW(m_TwBar, "rigid" , TW_TYPE_BOOLCPP, &rigid, "");
	TwAddVarRW(m_TwBar, "multiFractal" , TW_TYPE_BOOLCPP, &multiFractal, "");
	TwAddVarRW(m_TwBar, "perlinNoise" , TW_TYPE_BOOLCPP, &perlinNoise, "");

	TwAddVarRW(m_TwBar, "Amplitude Gain" , TW_TYPE_FLOAT, &m_ampgain, "min=0.01 max=6 step=0.01 keyIncr=z keyDecr=Z help='Amplitude gain for the Perlin Noise' ");
	TwAddVarRW(m_TwBar, "Frequency Gain" , TW_TYPE_FLOAT, &m_frqgain, "min=0.01 max=6 step=0.01 keyIncr=x keyDecr=X help='Frequency gain for the Perlin Noise' ");
	TwAddVarRW(m_TwBar, "Initial Amp." , TW_TYPE_FLOAT, &m_initamp, "min=0.001 max=6 step=0.001 keyIncr=c keyDecr=C help='Initial Amplitude, scale factor for the Amplitude in Perlin Noise' ");
	TwAddVarRW(m_TwBar, "Initial Freq." , TW_TYPE_FLOAT, &m_initfrq, "min=0.001 max=6 step=0.001 keyIncr=v keyDecr=V help='Initial Frequency, scale factor for the Frequency in Perlin Noise'  ");
	TwAddVarRW(m_TwBar, "Octavs" , TW_TYPE_FLOAT, &m_octvs, "min=1 max=25 step=1.0 keyIncr=n keyDecr=N help='Octavs of the perlin noise'  ");
	TwAddVarRW(m_TwBar, "Radius" , TW_TYPE_FLOAT, &m_radius, "min=1 max=1000 step=1.0 keyIncr=b keyDecr=B help='Radius of the planet'  ");





	//createFBO();
	m_st = new ShaderTexture("Shaders/vertex2pass2.glsl","Shaders/frag2pass2.glsl");
	//m_st2 = new ShaderTexture("Shaders/QuadVertexShaderpass2.glsl","Shaders/QuadFragShaderpass2.glsl");

	//m_st = new ShaderTexture("Shaders/vertex2pass2.glsl","Shaders/frag2pass2sobel.glsl");
	//m_st = new ShaderTexture("Shaders/vertex2pass2.glsl","Shaders/frag3pass2.glsl");
	//m_st = new ShaderTexture("Shaders/vertex2pass2.glsl","Shaders/frag4pass2.glsl");
	m_st->LoadShaders();
	//m_st2->LoadShaders();
	quad = new QuadMesh("media/cube.obj");
}

RenderManager::~RenderManager()
{
	delete m_scene;
	delete m_sceneGraph;
}

unsigned RenderManager::createTexture(int w, int h, bool isDepth)
{

	// The texture we're going to render to
	unsigned renderedTexture;
	glGenTextures(1, &renderedTexture);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
 
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F ,
		1024, 768, 0,GL_RGBA , GL_FLOAT, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 1024, 768, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0,(!isDepth ?GL_RGB : GL_DEPTH_COMPONENT), w, h, 0,(!isDepth ?GL_RGB : GL_DEPTH_COMPONENT), GL_UNSIGNED_BYTE, 0);
 
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return renderedTexture;
}

unsigned RenderManager::createFBO()
{
	renderTexture = createTexture(1024,768);
	renderTexture2 = createTexture(1024,768);
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, renderTexture, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D, renderTexture2, 0);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RenderManager::initRenderTarget()
{

	

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


 
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return true;
}

void RenderManager::AddNode(Node* node)
{
	m_sceneGraph->addNode(node);
}

void RenderManager::RemoveNode(Node *node)
{
	m_sceneGraph->removeNode(node);
}

void RenderManager::SetView(glm::mat4 &view)
{
	m_view = view;
}

void RenderManager::addLight(lightParameters * light)
{
	m_scene->addLight(light);
}

//float RenderManager::getViewportWidth()
//{
//	return float(m_viewport.mWidth);
//}
//
//float RenderManager::getViewportHeight()
//{
//	return float(m_viewport.mHeight);
//}

void RenderManager::Update()
{
	m_scene->initScene();
}

class ObjectSort
{
	glm::vec3 m_cameraPos;

public:
	ObjectSort(const glm::vec3 &pos) : m_cameraPos(pos) { }

	bool operator()(SceneManager::Renderable &lhs, SceneManager::Renderable &rhs)
	{
		if (lhs.object->getMesh()->getTransparent() && rhs.object->getMesh()->getTransparent())
			//return fabs(lengthSqr(lhs.matrix - m_cameraPos)) > fabs(lengthSqr(rhs.matrix - m_cameraPos));
		return lhs.object->getMesh()->getTransparent() < rhs.object->getMesh()->getTransparent();
	}
};

void RenderManager::Render(bool defered, bool squared, bool poolar)
{


	// fetch the updated object list
	std::vector<SceneManager::Renderable> &objlist = m_scene->getObjList();
	//Ligths Position
    std::vector<lightParameters*> lights = m_scene->getLights();

    //ObjectSort objectSort(m_camera->getPosition());
	//std::sort(objlist.begin(), objlist.end(), objectSort);
    
	m_camera->computeMatricesFromInputs();
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = m_camera->getProjectionMatrix();
	// Camera matrix
	glm::mat4 View       = m_camera->getViewMatrix();


	// Enable depth test
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		glDisable(GL_CULL_FACE);

		//glEnable(GL_CULL_FACE); //Enable cull facing
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		if(wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// render the non-transparent objects
		int transparent = 0;


	if(defered)
	{
		//----------First pass-------------------------------------//		

		// Render to our framebuffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FramebufferName);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0,0,1024,768);

		std::vector<GLenum> myarray;
		myarray.push_back(GL_COLOR_ATTACHMENT0);
		myarray.push_back(GL_COLOR_ATTACHMENT1);
		
		glDrawBuffers(myarray.size(),myarray.data());
		glClearColor(0.0,0.0,0.0,0);
	}
	else
	{
		// CLEAR
		glClearColor(0.0,0.0,0.5,0);
		//glClearColor(0.3,0.3,0.8,0);
	}

	// CLEAR
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    for (int i = 0; i < objlist.size(); i++)
    {
        //----------- SHADER INITIALIZATION -----------------------//		
        GLuint shaderID = objlist[i].shader->getProgramID();
        // Use our shader
        glUseProgram(shaderID);
        //-----------------------------------------------------------//


        glm::mat4 Model = objlist[i].matrix; // Changes for each model !
        glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
        // Get a handle for our "MVP" uniform.
        // Only at initialisation time.
        GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
        GLuint MatrixID2 = glGetUniformLocation(shaderID, "M");
        GLuint MatrixID3 = glGetUniformLocation(shaderID, "V");
        GLuint MatrixID4 = glGetUniformLocation(shaderID, "P");

        GLuint ampgain = glGetUniformLocation(shaderID, "ampgain");
        GLuint frqgain = glGetUniformLocation(shaderID, "frqgain");
        GLuint initamp = glGetUniformLocation(shaderID, "initamp");
        GLuint initfrq = glGetUniformLocation(shaderID, "initfrq");
        GLuint radius = glGetUniformLocation(shaderID, "radius");
        GLuint octv = glGetUniformLocation(shaderID, "octv");

        GLuint noise1 = glGetUniformLocation(shaderID, "simplexNoise");
        GLuint noise2 = glGetUniformLocation(shaderID, "rigidMultiFractal");
        GLuint noise3 = glGetUniformLocation(shaderID, "rigid");
        GLuint noise4 = glGetUniformLocation(shaderID, "multiFractal");
        GLuint noise5 = glGetUniformLocation(shaderID, "perlinNoise");

        /* Test for passing lights to shaders
        glm::vec3 Ligths[4];
        Ligths[0] = glm::vec3(1.0, 0.0, 0.0);
        Ligths[1] = glm::vec3(0.0, 1.0, 0.0);
        Ligths[2] = glm::vec3(0.0, 0.0, 1.0);
        Ligths[3] = glm::vec3(1.0, 0.0, 1.0);
        */

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &View[0][0]);
        glUniformMatrix4fv(MatrixID4, 1, GL_FALSE, &Projection[0][0]);
        
        //Lights Structure
        for (int i = 0; i < lights.size(); i++)
        {
            string variableName("lightSource[" + std::to_string(i) + "].specular"); 
            GLuint LightsID = glGetUniformLocation(shaderID, variableName.c_str());
            glUniform4fv(LightsID, 1, reinterpret_cast<const GLfloat*>(&lights[i]->specular[0]));
            variableName = ("lightSource[" + std::to_string(i) + "].diffuse");
            LightsID = glGetUniformLocation(shaderID, variableName.c_str());
            glUniform4fv(LightsID, 1, reinterpret_cast<const GLfloat*>(&lights[i]->diffuse[0]));
            variableName = ("lightSource[" + std::to_string(i) + "].specular");
            LightsID = glGetUniformLocation(shaderID, variableName.c_str());
            glUniform4fv(LightsID, 1, reinterpret_cast<const GLfloat*>(&lights[i]->specular[0]));
            variableName = ("lightSource[" + std::to_string(i) + "].position");
            LightsID = glGetUniformLocation(shaderID, variableName.c_str());
           /* glm::vec4 lightWorldCoordinates(Model * glm::vec4(0, 0, 0, 1));
            lights[i]->position = glm::vec3(lightWorldCoordinates.x / lightWorldCoordinates.w,
                lightWorldCoordinates.y / lightWorldCoordinates.w,
                lightWorldCoordinates.z / lightWorldCoordinates.w);*/
            glUniform3fv(LightsID, 1, reinterpret_cast<const GLfloat*>(&lights[i]->position[0]));
        }
		
		glUniform1f(ampgain, m_ampgain);
		glUniform1f(frqgain, m_frqgain);
		glUniform1f(initamp, m_initamp);
		glUniform1f(initfrq, m_initfrq);
		glUniform1f(radius, m_radius);
		glUniform1f(octv, m_octvs);

		if(simplexNoise)
		{
			rigidMultiFractal = false;
			rigid = false;
			multiFractal = false;
			perlinNoise = false;
		}
		else if(rigidMultiFractal)
		{
			simplexNoise = false;
			rigid = false;
			multiFractal = false;
			perlinNoise = false;		
		}
		else if(rigid)
		{
			simplexNoise = false;
			rigidMultiFractal = false;
			multiFractal = false;
			perlinNoise = false;
		}
		else if(multiFractal)
		{
			simplexNoise = false;
			rigidMultiFractal = false;
			rigid = false;
			perlinNoise = false;
		}
		else if(perlinNoise)
		{
			simplexNoise = false;
			rigidMultiFractal = false;
			multiFractal = false;
			rigid = false;
		}
		
		glUniform1f(noise1, simplexNoise);
		glUniform1f(noise2, rigidMultiFractal);
		glUniform1f(noise3, rigid);
		glUniform1f(noise4, multiFractal);
		glUniform1f(noise5, perlinNoise);
		/* This condiction is for transparency but is not well implemented
		So I will leave it commented until I figur it out why*/
		/*
		if(( !defered )&& i >= objlist.size()-2)
		{
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			
			if(glm::length( Model * glm::vec4(m_camera->getPosition(),1) ) < m_radius*1.5f && i == objlist.size()-1 )
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);			
			}

			
			
		}
		else
		{
			glDisable(GL_BLEND);
		}
		*/
		// draw the mesh
		objlist[i].object->getMesh()->draw();

	
	}

	if(wireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	
	
	//---------------------------------------------------------//

	if(defered)
	{
		//----------Second pass-------------------------------------//
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0,0,1024,768);
		//glDisable(GL_DEPTH_TEST);
		glClearDepth(1.0);
		glClear(GL_DEPTH_BUFFER_BIT);
		GLuint shaderID = m_st->getProgramID();
		glUseProgram(shaderID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,renderTexture);

		GLuint MatrixID3 = glGetUniformLocation(shaderID, "V");
		glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &View[0][0]);

		glActiveTexture(GL_TEXTURE1); 
		glBindTexture(GL_TEXTURE_2D,renderTexture2);

		quad->draw();

		glBindTexture(GL_TEXTURE_2D,0);	
		//---------------------------------------------------------//
	}



	
}

