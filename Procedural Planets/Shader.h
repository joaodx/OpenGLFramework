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

class Shader
{
private:
	string			m_vsFileName, m_tcsFileName, m_tesFileName, m_gsFileName, m_psFileName;
	Timer			m_timer;
	GLuint			ProgramID;

	static Shader *c_lastShader;
	bool m_loaded;

public:
	Shader(const char * vertex_file_path,const char * tc_file_path,const char * te_file_path
		,const char * geometry_file_path, const char * fragment_file_path);
	Shader(const char * vertex_file_path, const char * fragment_file_path);
	~Shader();	

	GLuint LoadShaders();
	GLuint getProgramID() { return ProgramID; };

};