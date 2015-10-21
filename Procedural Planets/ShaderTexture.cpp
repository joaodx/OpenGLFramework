#include "ShaderTexture.h"
#include <stdio.h>



ShaderTexture *ShaderTexture::c_lastShader = NULL;

ShaderTexture::ShaderTexture(const char * vertex_file_path, const char * fragment_file_path)
		:m_vsFileName(vertex_file_path), m_psFileName(fragment_file_path),m_loaded(false)
	

{
	m_timer.start();
}


ShaderTexture::~ShaderTexture()
{
}

GLuint ShaderTexture::LoadShaders()
{
	if(!m_loaded)
	{
		m_loaded = true;
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
		// Read the Vertex ShaderTexture code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(m_vsFileName, std::ios::in);
		if(VertexShaderStream.is_open())
		{
			std::string Line = "";
			while(getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}

		
		// Read the Fragment ShaderTexture code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(m_psFileName, std::ios::in);
		if(FragmentShaderStream.is_open()){
			std::string Line = "";
			while(getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}
 
		GLint Result = GL_FALSE;
		int InfoLogLength;
 
		// Compile Vertex ShaderTexture
		printf("Compiling shader : %s\n", m_vsFileName);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, (const GLchar**)&VertexSourcePointer , NULL);
		glCompileShader(VertexShaderID);
 
		// Check Vertex ShaderTexture
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

		
 
		// Compile Fragment ShaderTexture
		printf("Compiling shader : %s\n", m_psFileName);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, (const GLchar**)&FragmentSourcePointer , NULL);
		glCompileShader(FragmentShaderID);
 
		// Check Fragment ShaderTexture
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
 
		// Link the program
		fprintf(stdout, "Linking program\n");
		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);
 
		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
 
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
 
		return ProgramID;
	}

	return 0;
}

