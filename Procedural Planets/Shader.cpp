#include "Shader.h"
#include <stdio.h>



Shader *Shader::c_lastShader = NULL;

Shader::Shader(const char * vertex_file_path,const char * tc_file_path,const char * te_file_path
		,const char * geometry_file_path, const char * fragment_file_path)
		:m_vsFileName(vertex_file_path), m_tcsFileName(tc_file_path),
		m_tesFileName(te_file_path), m_gsFileName(geometry_file_path), m_psFileName(fragment_file_path),m_loaded(false)
	

{
	m_timer.start();
}

Shader::~Shader()
{
}

GLuint Shader::LoadShaders()
{
	if(!m_loaded)
	{
		m_loaded = true;
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint TcShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		GLuint TeShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(m_vsFileName, std::ios::in);
		if(VertexShaderStream.is_open())
		{
			std::string Line = "";
			while(getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}

		// Read the TC Shader code from the file
		std::string TcShaderCode;
		std::ifstream TcShaderStream(m_tcsFileName, std::ios::in);
		if(TcShaderStream.is_open())
		{
			std::string Line = "";
			while(getline(TcShaderStream, Line))
				TcShaderCode += "\n" + Line;
			TcShaderStream.close();
		}

		// Read the Te Shader code from the file
		std::string TeShaderCode;
		std::ifstream TeShaderStream(m_tesFileName, std::ios::in);
		if(TeShaderStream.is_open())
		{
			std::string Line = "";
			while(getline(TeShaderStream, Line))
				TeShaderCode += "\n" + Line;
			TeShaderStream.close();
		}

		// Read the Geometry Shader code from the file
		std::string GeometryShaderCode;
		std::ifstream GeometryShaderStream(m_gsFileName, std::ios::in);
		if(GeometryShaderStream.is_open())
		{
			std::string Line = "";
			while(getline(GeometryShaderStream, Line))
				GeometryShaderCode += "\n" + Line;
			GeometryShaderStream.close();
		}
 
		// Read the Fragment Shader code from the file
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
 
		// Compile Vertex Shader
		printf("Compiling shader : %s\n", m_vsFileName);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, (const GLchar**)&VertexSourcePointer , NULL);
		glCompileShader(VertexShaderID);
 
		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

		 // Compile Tc Shader
		printf("Compiling shader : %s\n", m_tcsFileName);
		char const * TcSourcePointer = TcShaderCode.c_str();
		glShaderSource(TcShaderID, 1, (const GLchar**)&TcSourcePointer , NULL);
		glCompileShader(TcShaderID);
 
		// Check Tc Shader
		glGetShaderiv(TcShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(TcShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> TcShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(TcShaderID, InfoLogLength, NULL, &TcShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &TcShaderErrorMessage[0]);

		// Compile Te Shader
		printf("Compiling shader : %s\n", m_tesFileName);
		char const * TeSourcePointer = TeShaderCode.c_str();
		glShaderSource(TeShaderID, 1, (const GLchar**)&TeSourcePointer , NULL);
		glCompileShader(TeShaderID);
 
		// Check Te Shader
		glGetShaderiv(TeShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(TeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> TeShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(TeShaderID, InfoLogLength, NULL, &TeShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &TeShaderErrorMessage[0]);


		// Compile Geometry Shader
		printf("Compiling shader : %s\n", m_gsFileName);
		char const * GeometrySourcePointer = GeometryShaderCode.c_str();
		glShaderSource(GeometryShaderID, 1, (const GLchar**)&GeometrySourcePointer , NULL);
		glCompileShader(GeometryShaderID);
 
		// Check Geometry Shader
		glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> GeometryShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &GeometryShaderErrorMessage[0]);
 
		// Compile Fragment Shader
		printf("Compiling shader : %s\n", m_psFileName);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, (const GLchar**)&FragmentSourcePointer , NULL);
		glCompileShader(FragmentShaderID);
 
		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
 
		// Link the program
		fprintf(stdout, "Linking program\n");
		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, TcShaderID);
		glAttachShader(ProgramID, TeShaderID);
		glAttachShader(ProgramID, GeometryShaderID);
		glAttachShader(ProgramID, FragmentShaderID);

		
		glLinkProgram(ProgramID);
 
		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

			glDeleteShader(VertexShaderID);
			glDeleteShader(TcShaderID);
			glDeleteShader(TeShaderID);
			glDeleteShader(GeometryShaderID);
			glDeleteShader(FragmentShaderID);
			return NULL;
		}
		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, TcShaderID);
		glDetachShader(ProgramID, TeShaderID);
		glDetachShader(ProgramID, GeometryShaderID);
		glDetachShader(ProgramID, FragmentShaderID);
 
		return ProgramID;
	}

	return 0;
}

