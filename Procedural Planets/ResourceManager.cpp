#include "ResourceManager.h"




ResourceManager::ResourceManager()
{
}

void ResourceManager::cacheLoad(const vector<string> &meshes, const vector<pair<string, string> > &shaders, const vector<string> &textures, const vector<string> &fonts)
{
	for (unsigned c = 0; c < meshes.size(); ++c)
		createMesh(meshes[c]);
	//for (unsigned c = 0; c < shaders.size(); ++c)
		//createShader(shaders[c].first, shaders[c].second);
	for (unsigned c = 0; c < textures.size(); ++c)
		createTexture(textures[c]);
}

Mesh *ResourceManager::createMesh(const string &filename)
{
	if (meshes.find(filename) == meshes.end())
	{
		Mesh* mesh = new ObjMesh(filename);
		meshes[filename] = mesh;
	}
	return meshes[filename];	
}

Shader *ResourceManager::createShader(const string & vertex_file_path, const string & tc_file_path, const string & te_file_path
		,const string & geometry_file_path, const string & fragment_file_path)
{
	string id = vertex_file_path + tc_file_path + te_file_path + geometry_file_path + fragment_file_path;
	if (shaders.find(id) == shaders.end())
	{
		shaders[id] = new Shader(vertex_file_path.c_str(), tc_file_path.c_str(), 
									te_file_path.c_str(), geometry_file_path.c_str(), 
									fragment_file_path.c_str());
		shaders[id]->LoadShaders();
	}
	return shaders[id];
}

void ResourceManager::unload()
{
	for (map<string, Mesh*>::iterator i = meshes.begin(); i != meshes.end(); ++i)
		delete i->second;
	for (map<string, unsigned>::iterator i = textures.begin(); i != textures.end(); ++i)
		glDeleteTextures(1, &i->second);
	for (map<string, Shader*>::iterator i = shaders.begin(); i != shaders.end(); ++i)
		delete i->second;
}


ResourceManager::~ResourceManager()
{
}

// If texture is not found, texture index will be huge number
unsigned ResourceManager::createTexture(const string &filename, unsigned interpolationMode, unsigned wrapModeS, unsigned wrapModeT)
{
	string fn(filename);
	if (textures.find(filename) == textures.end())
	{
		FILE *file = fopen(fn.c_str(), "rb");
		if (file == NULL)
			return -1;

		BitmapHeader header;
		fread(&header, sizeof(header), 1, file);
		//header.fixByteOrder();

		unsigned char *data = (unsigned char *)malloc(header.width * header.height * 3);
		fread(data, header.width * header.height * 3, 1, file);
		fclose(file);

		unsigned texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT);
#ifndef _WIN32
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header.width, header.height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header.width, header.height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
#endif
		textures[filename] = texture;
	}

	return textures[filename];
}


