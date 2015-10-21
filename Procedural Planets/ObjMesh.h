#pragma once


#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include "Utilities.h"
#include "Mesh.h"

// Include GLEW
#include <GL/glew.h>


using namespace std;

class ObjMesh : public Mesh
{
private:
	unsigned m_vao;
	
	
	int m_vertexCount, m_indexCount;
	float m_width, m_height, m_depth;
	bool m_transparency;

public:
	explicit ObjMesh(const string &fileName = "");
	~ObjMesh();

	void draw();

	void setTransparency(bool transparent) { m_transparency = transparent; }
	bool getTransparent() { return m_transparency; }
	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
	float getDepth() { return m_depth; }
	void generateCubeVertixes(vector<GLfloat> &);
	void generateCubeVertixes2(vector<GLfloat> &,vector<unsigned> &);
	void generateCubeIndexes(vector<GLfloat> &,vector<unsigned> &,float,int);


private: //switch GLfloat to float.
	bool loadBuffers(const char * path, std::vector < GLfloat > & out_vertices, 
						 std::vector < GLfloat > & out_uvs, std::vector < GLfloat > & out_normals);

};