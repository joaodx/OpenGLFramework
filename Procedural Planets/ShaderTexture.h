#pragma once


// Include GLM
#include <glm\glm.hpp>

// Include GLEW
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <vector>
#include "Timer.h"

using namespace std;

class ShaderTexture
{
private:
	string			m_vsFileName, m_tcsFileName, m_tesFileName, m_gsFileName, m_psFileName;
	Timer			m_timer;
	GLuint			ProgramID;

	static ShaderTexture *c_lastShader;
	bool m_loaded;

public:

	ShaderTexture(const char * vertex_file_path, const char * fragment_file_path);
	~ShaderTexture();	

	GLuint LoadShaders();
	GLuint getProgramID() { return ProgramID; };

};