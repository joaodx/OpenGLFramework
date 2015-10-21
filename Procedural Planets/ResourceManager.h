#pragma once
#include "Mesh.h"
#include "ObjMesh.h"
#include <map>
#include <stdio.h>
#include "Shader.h"
#include "Utilities.h"
#include "Thread.h"
// Include GLM
#include <glm\glm.hpp>
#include <gl\GL.h>

class ResourceManager
{
	#pragma pack(push, 1)
	struct BitmapHeader
	{
		unsigned short signature;
		unsigned size;
		unsigned short reserved[2];
		unsigned offset;
		unsigned headerInfoSize;
		unsigned width;
		unsigned height;
		unsigned short planes;
		unsigned short bitsPerPixel;
		unsigned compressionType;
		unsigned imageDataSize;
		unsigned horizontalRes;
		unsigned verticalRes;
		unsigned noOfColours;
		unsigned noOfImportantColours;

		void fixByteOrder();
	};
	#pragma pack(pop)

public:
	ResourceManager();
	~ResourceManager();

	void cacheLoad(const vector<string> &meshes, const vector<pair<string, string> > &shaders, const vector<string> &textures, const vector<string> &fonts);

	Mesh *createMesh(const string &fileName);
	Shader *createShader(const string & vertex_file_path, const string & tc_file_path, const string & te_file_path
							, const string & geometry_file_path, const string & fragment_file_path);
	unsigned createTexture(const string &filename, unsigned interpolationMode = GL_LINEAR,
								unsigned wrapModeS = GL_REPEAT, unsigned wrapModeT = GL_REPEAT);
	//void setContext(CGcontext context) { cgContext = context; }
	void unload();

private:
	unsigned LoadTexturePNG(const char* name, unsigned interpolationMode, unsigned wrapModeS, unsigned wrapModeT);

	map<string, Mesh*> meshes;
	map<string, unsigned> textures;
	map<string, Shader*> shaders;

	//CGcontext cgContext;
};

