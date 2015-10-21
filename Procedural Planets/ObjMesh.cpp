#include "ObjMesh.h"
// Include GLEW
#include <GL/glew.h>

// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data_new_mesh[] = {
      1.0f,1.0f, 1.0f,	 //0 
      1.0f,-1.0f, 1.0f,  //1
	 -1.0f,-1.0f,1.0f,   //2
	 -1.0f,1.0f, 1.0f,   //3 

	  1.0f,1.0f,-1.0f,	 //4 
      -1.0f,1.0f, -1.0f, //5
	 -1.0f,-1.0f,-1.0f,  //6
	  1.0f,-1.0f, -1.0f, //7
};

// An array of 3 vectors which represents 3 vertices
static const unsigned g_index_buffer_data_new_mesh[] = {
     0,1,2, /*EXTRA*/ 7,7,3,
	 2,3,0, /*EXTRA*/ 5,4,1, //front


	 4,5,6, /*EXTRA*/ 7,3,2,
	 6,7,4, /*EXTRA*/ 5,2,0, //back


	 4,0,3, /*EXTRA*/ 5,2,7,
	 3,5,4, /*EXTRA*/ 0,6,2, //top


	 7,1,2, /*EXTRA*/ 6,0,0,
	 2,6,7, /*EXTRA*/ 1,5,4, // bottom


	 5,3,2, /*EXTRA*/ 6,4,0,
	 2,6,5, /*EXTRA*/ 3,7,4, //left


	 0,4,7, /*EXTRA*/ 3,1,6,
	 7,1,0, /*EXTRA*/ 4,2,2,//right

};

// An array of 3 vectors which represents 3 vertices
static const unsigned g_index_buffer_data_new_mesh_cube[] = {
     6,1,0, /*EXTRA*/ 2,25,5, //----
	 0,5,6, /*EXTRA*/ 30,11,1,
	 6,7,2, /*EXTRA*/ 11,8,1,
	 2,1,6, /*EXTRA*/ 26,0,7, 
	 8,3,2, /*EXTRA*/ 4,27,7,
	 2,7,8, /*EXTRA*/ 6,13,3, 
	 8,9,4, /*EXTRA*/ 13,31,3,
	 4,3,8, /*EXTRA*/ 28,2,9,

	 11,6,5, /*EXTRA*/ 7,0,10,
	 5,10,11, /*EXTRA*/ 32,16,6,
	 11,12,7, /*EXTRA*/ 16,13,6,
	 7,6,11, /*EXTRA*/ 2,5,12, 
	 13,8,7, /*EXTRA*/ 9,2,12,
	 7,12,13, /*EXTRA*/ 11,18,8,
	 13,14,9, /*EXTRA*/ 18,31,8,
	 9,8,13, /*EXTRA*/ 4,7,14,

	 16,11,10, /*EXTRA*/ 12,5,15,
	 10,15,16, /*EXTRA*/ 34,21,11,
	 16,17,12, /*EXTRA*/ 21,18,11,
	 12,11,16, /*EXTRA*/ 7,10,17, 
	 18,13,12, /*EXTRA*/ 14,7,17,
	 12,17,18, /*EXTRA*/ 16,23,13, 
	 18,19,14, /*EXTRA*/ 23,35,13,
	 14,13,18, /*EXTRA*/ 9,12,19,

	 21,16,15, /*EXTRA*/ 17,10,20,
	 15,20,21, /*EXTRA*/ 36,36,16,
	 21,22,17, /*EXTRA*/ 38,23,16,
	 17,16,21, /*EXTRA*/ 12,15,22, 
	 23,18,17, /*EXTRA*/ 19,12,22,
	 17,22,23, /*EXTRA*/ 21,38,18,
	 23,24,19, /*EXTRA*/ 40,35,18,
	 19,18,23, /*EXTRA*/ 14,17,24, //--- LEFT FACE

	 36,37,21, /*EXTRA*/ 52,38,20, //----
	 21,20,36, /*EXTRA*/ 15,15,37,
	 38,22,21, /*EXTRA*/ 23,17,37,
	 21,37,38, /*EXTRA*/ 36,54,22, 
	 38,39,23, /*EXTRA*/ 54,40,22,
	 23,22,38, /*EXTRA*/ 17,21,39, 
	 40,24,23, /*EXTRA*/ 35,19,39,
	 23,39,40, /*EXTRA*/ 38,56,24,

	 52,53,37, /*EXTRA*/ 68,54,36,
	 37,36,52, /*EXTRA*/ 21,50,53,
	 54,38,37, /*EXTRA*/ 39,21,53,
	 37,53,54, /*EXTRA*/ 52,70,38, 
	 54,55,39, /*EXTRA*/ 70,56,38,
	 39,38,54, /*EXTRA*/ 23,37,55,
	 56,40,39, /*EXTRA*/ 51,23,55,
	 39,55,56, /*EXTRA*/ 54,72,40,

	 68,69,53, /*EXTRA*/ 93,70,52,
	 53,52,68, /*EXTRA*/ 37,50,69,
	 70,54,53, /*EXTRA*/ 55,37,69,
	 53,69,70, /*EXTRA*/ 68,95,54, 
	 70,71,55, /*EXTRA*/ 95,72,54,
	 55,54,70, /*EXTRA*/ 39,53,71, 
	 72,56,55, /*EXTRA*/ 67,39,71,
	 55,71,72, /*EXTRA*/ 70,97,56,

	 93,94,69, /*EXTRA*/ 88,95,68,
	 69,68,93, /*EXTRA*/ 53,88,94,
	 95,70,69, /*EXTRA*/ 71,53,94,
	 69,94,95, /*EXTRA*/ 93,90,70, 
	 95,96,71, /*EXTRA*/ 90,97,70,
	 71,70,95, /*EXTRA*/ 55,69,96,
	 97,72,71, /*EXTRA*/ 92,55,96,
	 71,96,97, /*EXTRA*/ 95,92,72, //--- TOP FACE

	 81,76,77, /*EXTRA*/ 75,60,82, //----
	 77,82,81, /*EXTRA*/ 61,86,76,
	 81,80,75, /*EXTRA*/ 86,79,76,
	 75,76,81, /*EXTRA*/ 59,77,80, 
	 79,74,75, /*EXTRA*/ 73,58,80,
	 75,80,79, /*EXTRA*/ 81,84,74, 
	 79,78,73, /*EXTRA*/ 84,62,74,
	 73,74,79, /*EXTRA*/ 57,75,78,

	 86,81,82, /*EXTRA*/ 80,77,87,
	 82,87,86, /*EXTRA*/ 65,91,81,
	 86,85,80, /*EXTRA*/ 91,84,81,
	 80,81,86, /*EXTRA*/ 75,82,85, 
	 84,79,80, /*EXTRA*/ 78,75,85,
	 80,85,84, /*EXTRA*/ 86,89,79,
	 84,83,78, /*EXTRA*/ 89,64,79,
	 78,79,84, /*EXTRA*/ 73,80,83,

	 91,86,87, /*EXTRA*/ 85,82,92,
	 87,92,91, /*EXTRA*/ 65,96,86,
	 91,90,85, /*EXTRA*/ 96,89,86,
	 85,86,91, /*EXTRA*/ 80,87,90, 
	 89,84,85, /*EXTRA*/ 83,80,90,
	 85,90,89, /*EXTRA*/ 91,94,84, 
	 89,88,83, /*EXTRA*/ 94,66,84,
	 83,84,89, /*EXTRA*/ 78,85,88,

	 96,91,92, /*EXTRA*/ 90,87,97,
	 92,97,96, /*EXTRA*/ 72,71,91,
	 96,95,90, /*EXTRA*/ 71,94,91,
	 90,91,96, /*EXTRA*/ 85,92,95, 
	 94,89,90, /*EXTRA*/ 88,85,95,
	 90,95,94, /*EXTRA*/ 96,69,89,
	 94,93,88, /*EXTRA*/ 69,68,89,
	 88,89,94, /*EXTRA*/ 83,90,93, //--- RIGHT FACE

	  4,9,31, /*EXTRA*/ 8,14,29, //----
	 31,29,4, /*EXTRA*/ 47,28,9,
	 14,33,31, /*EXTRA*/ 35,47,9,
	 31,9,14, /*EXTRA*/ 4,13,33, 
	 14,19,35, /*EXTRA*/ 18,24,33,
	 35,33,14, /*EXTRA*/ 51,31,19, 
	 24,40,35, /*EXTRA*/ 23,51,19,
	 35,19,24, /*EXTRA*/ 14,23,40,

	 29,31,47, /*EXTRA*/ 4,33,45,
	 47,45,29, /*EXTRA*/ 63,28,31,
	 33,49,47, /*EXTRA*/ 51,63,31,
	 47,31,33, /*EXTRA*/ 29,14,49, 
	 33,35,51, /*EXTRA*/ 14,40,49,
	 51,49,33, /*EXTRA*/ 67,47,35,
	 40,56,51, /*EXTRA*/ 39,67,35,
	 51,35,40, /*EXTRA*/ 33,24,56,

	 45,47,63, /*EXTRA*/ 29,49,61,
	 63,61,45, /*EXTRA*/ 82,60,47,
	 49,65,63, /*EXTRA*/ 67,82,47,
	 63,47,49, /*EXTRA*/ 45,33,65, 
	 49,51,67, /*EXTRA*/ 33,56,65,
	 67,65,49, /*EXTRA*/ 92,63,51, 
	 56,72,67, /*EXTRA*/ 55,92,51,
	 67,51,56, /*EXTRA*/ 49,40,72,

	 61,63,82, /*EXTRA*/ 45,65,77,
	 82,77,61, /*EXTRA*/ 81,60,63,
	 65,87,82, /*EXTRA*/ 92,86,63,
	 82,63,65, /*EXTRA*/ 61,49,87, 
	 65,67,92, /*EXTRA*/ 49,72,87,
	 92,87,65, /*EXTRA*/ 91,82,67,
	 72,97,92, /*EXTRA*/ 71,96,67,
	 92,67,72, /*EXTRA*/ 65,56,97, //--- FRONT FACE 

	 4,29,28, /*EXTRA*/ 31,45,3, //----
	 28,3,4, /*EXTRA*/ 27,8,29,
	 45,44,28, /*EXTRA*/ 60,27,29,
	 28,29,45, /*EXTRA*/ 4,47,44, 
	 45,61,60, /*EXTRA*/ 63,77,44,
	 60,44,45, /*EXTRA*/ 59,28,61,
	 77,76,60, /*EXTRA*/ 81, 59, 61,
	 60,61,77, /*EXTRA*/ 45, 82, 76,

	 3,28,27, /*EXTRA*/ 4, 44, 2,
	 27,2,3, /*EXTRA*/ 26, 8, 28,
	 44,43,27, /*EXTRA*/ 59, 26, 28,
	 27,28,44, /*EXTRA*/  3, 45, 43,
	 44,60,59, /*EXTRA*/ 45, 76, 43,
	 59,43,44, /*EXTRA*/ 58, 27, 60,
	 76,75,59, /*EXTRA*/ 81, 58, 60,
	 59,60,76, /*EXTRA*/ 44, 77, 75,

	 2,27,26, /*EXTRA*/ 3, 43, 1,
	 26,1,2, /*EXTRA*/ 25, 6, 27,
	 43,42,26, /*EXTRA*/ 58, 25, 27,
	 26,27,43, /*EXTRA*/ 2, 44, 42,
	 43,59,58, /*EXTRA*/ 44, 75, 42,
	 58,42,43, /*EXTRA*/ 57, 26, 59,
	 75,74,58, /*EXTRA*/ 79, 57, 59,
	 58,59,75, /*EXTRA*/ 43, 76, 74,

	 1,26,25, /*EXTRA*/ 2, 42, 0,
	 25,0,1, /*EXTRA*/ 30, 6, 26, 
	 42,41,25, /*EXTRA*/ 57, 30, 26,
	 25,26,42, /*EXTRA*/ 1, 43, 41,
	 42,58,57, /*EXTRA*/ 43, 74, 41, 
	 57,41,42, /*EXTRA*/ 62, 25, 58,
	 74,73,57, /*EXTRA*/ 79, 62, 58,
	 57,58,74, /*EXTRA*/ 42, 75, 73, //--- BOTTOM FACE 

	  0,25,30, /*EXTRA*/ 1, 41, 5, //----
	 30,5,0, /*EXTRA*/ 32, 6, 25,
	 41,46,30, /*EXTRA*/ 62, 32, 25,
	 30,25,41, /*EXTRA*/ 0, 42, 46,
	 41,57,62, /*EXTRA*/ 42, 73, 46,
	 62,46,41, /*EXTRA*/ 64, 30, 57,
	 73,78,62, /*EXTRA*/ 79, 64, 57,
	 62,57,73, /*EXTRA*/ 41, 74, 78,

	 5,30,32, /*EXTRA*/ 0, 46, 10, 
	 32,10,5, /*EXTRA*/ 34, 11, 30, 
	 46,48,32, /*EXTRA*/ 64, 34, 30,
	 32,30,46, /*EXTRA*/ 5, 41, 48, 
	 46,62,64, /*EXTRA*/ 41, 78, 48,
	 64,48,46, /*EXTRA*/ 66, 32, 62,
	 78,83,64, /*EXTRA*/ 84, 66, 62,
	 64,62,78, /*EXTRA*/ 46, 73, 83,

	 10,32,34, /*EXTRA*/ 5, 48, 15, 
	 34,15,10, /*EXTRA*/ 36, 16, 32, 
	 48,50,34, /*EXTRA*/ 66, 36, 32,
	 34,32,48, /*EXTRA*/ 10, 46, 50,
	 48,64,66, /*EXTRA*/ 46, 83, 50,
	 66,50,48, /*EXTRA*/ 68, 34, 64,
	 83,88,66, /*EXTRA*/ 89, 68, 64,
	 66,64,83, /*EXTRA*/ 48, 78, 88,

	 15,34,36, /*EXTRA*/ 10, 50, 20, 
	 36,20,15, /*EXTRA*/ 21, 21, 34,
	 50,52,36, /*EXTRA*/ 68, 37, 34, 
	 36,34,50, /*EXTRA*/ 15, 48, 52,
	 50,66,68, /*EXTRA*/ 48, 88, 52,
	 68,52,50, /*EXTRA*/ 53, 36, 66,
	 88,93,68, /*EXTRA*/ 94, 69, 66,
	 68,66,88, /*EXTRA*/ 50, 83, 93, //--- BACK FACE 
	 

};//85 vertex

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

#undef max
#undef min

ObjMesh::ObjMesh(const string &fileName)
	: Mesh(),m_vertexCount(0), m_indexCount(0), m_transparency(false)
{
	// fetch file name
	string name(fileName);
#ifndef _WIN32
	name = "/app_home/" + name;
#endif

	vector<GLfloat> new_mesh;
	vector<unsigned> new_index;
	//generateCubeVertixes2(new_mesh,new_index);
	//generateCubeVertixes(new_mesh);
	// load the object file into buffers
	// Read our .obj file
	std::vector< GLfloat > vertices;
	std::vector< GLfloat > uvs;
	std::vector< GLfloat > normals; // Won't be used at the moment.
	loadBuffers(name.c_str(), vertices, uvs,normals);
	
	// record the buffer sizes
	//m_vertexCount = new_mesh.size();
	//m_indexCount = new_index.size();
	m_vertexCount = vertices.size();
	m_indexCount = new_index.size();

	//m_vertexCount = 30;
	//m_indexCount = 12;
	// generate and bind VAO
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

	// generate, bind and buffer VBO
	GLuint m_vbo;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_new_mesh), g_vertex_buffer_data_new_mesh, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	//VERTEX
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, stride, (const GLvoid*)offset);
	glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 3* sizeof(GLfloat), (const GLvoid*)0);
	//offset += 4 * sizeof(float);

	// generate , bind a color buffer
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//COLOR
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3, GL_FLOAT,GL_FALSE, 3* sizeof(GLfloat), (const GLvoid*)0);


	//NORMAL
	// generate , bind a normal buffer
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,3, GL_FLOAT,GL_FALSE, 3* sizeof(GLfloat), (const GLvoid*)0);

	//UVS
	// generate , bind a UVS buffer
	GLuint uvsbuffer;
	glGenBuffers(1, &uvsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvsbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), &uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,2, GL_FLOAT,GL_FALSE, 2* sizeof(GLfloat), (const GLvoid*)0);



	// unbind VAO, VBO and IBO/EBO
	glBindVertexArray(0);	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	



	if(!vertices.empty())
	{
		vertices.clear();
		vertices.shrink_to_fit();
	}
	if(!normals.empty())
	{
		normals.clear();
		normals.shrink_to_fit();
	}
	if(!uvs.empty())
	{
		uvs.clear();
		uvs.shrink_to_fit();
	}
}

ObjMesh::~ObjMesh()
{
	//glDeleteBuffers(2, m_vbos);
}

void ObjMesh::draw()
{
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(0);*/

	glBindVertexArray(m_vao);
	//glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawArrays( GL_PATCHES, 0, m_vertexCount);	
	//glDrawElements(GL_PATCHES, 1152, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

bool ObjMesh::loadBuffers(const char * path, std::vector < GLfloat > & out_vertices,
							std::vector < GLfloat > & out_uvs, std::vector < GLfloat > & out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if( file == NULL )
	{
		printf("Impossible to open the file !\n");
		return false;
	}

	while( 1 )
	{
 
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
 
		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9)
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

			// For each vertex of each triangle
			for( unsigned int i=0; i<vertexIndices.size(); i++ ){
				unsigned int vertexIndex = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
				out_vertices.push_back(vertex.x);
				out_vertices.push_back(vertex.y);
				out_vertices.push_back(vertex.z);
			}

			// For each uv of each triangle
			for( unsigned int i=0; i<uvIndices.size(); i++ ){
				unsigned int uvIndex = uvIndices[i];
				glm::vec2 uv = temp_uvs[ uvIndex-1 ];
				out_uvs.push_back(uv.x);
				out_uvs.push_back(uv.y);
				
			}

			// For each normal of each triangle
			for( unsigned int i=0; i<normalIndices.size(); i++ ){
				unsigned int normalIndex = normalIndices[i];
				glm::vec3 normal = temp_normals[ normalIndex-1 ];
				out_normals.push_back(normal.x);
				out_normals.push_back(normal.y);
				out_normals.push_back(normal.z);
			}
}

void ObjMesh::generateCubeVertixes(vector<GLfloat> &new_mesh)
{
	GLint m_radius = 1;
	GLfloat numberOfCuts = 2;
	GLfloat incrementor = 0;

	if(numberOfCuts == 0)
		incrementor = 2;
	else
		incrementor = m_radius/numberOfCuts;

	GLint count = 0;
	for(float x = -m_radius; x <= m_radius; x = x + incrementor)
	{

		for(float y = -m_radius; y <= m_radius; y = y + incrementor)
		{
			
			for(float z = -m_radius; z <= m_radius; z = z + incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}

			}
			
		}
		
	}
}

void ObjMesh::generateCubeVertixes2(vector<GLfloat> &new_mesh,vector<unsigned> &indexes)
{
	GLint m_radius = 1;
	GLfloat numberOfCuts = 2;
	GLfloat incrementor = 0;

	if(numberOfCuts == 0)
		incrementor = 2;
	else
		incrementor = m_radius/numberOfCuts;

	GLint numberOfFaces = 1;

	

	//// Left face //-----------------------------------------------------------//
	float x = -m_radius;
		for(float y = -m_radius; y <= m_radius; y = y + incrementor)
		{
			
			for(float z = -m_radius; z <= m_radius; z = z + incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}
			}
		}
	// Left face //-----------------------------------------------------------//

	// Top face //-----------------------------------------------------------//
	numberOfFaces++;
	float y = m_radius;
		for(float x = -m_radius; x <= m_radius; x = x + incrementor)
		{
			
			for(float z = -m_radius; z <= m_radius; z = z + incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}
			}
		}
	// Top face //-----------------------------------------------------------//

	// Right face //-----------------------------------------------------------//
	numberOfFaces++;
	x = m_radius;
		for(float y = m_radius; y >= -m_radius; y = y - incrementor)
		{
			
			for(float z = -m_radius; z <= m_radius; z = z + incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}
			}
		}
	// Right face //-----------------------------------------------------------//

	// Bottom face //-----------------------------------------------------------//
	numberOfFaces++;
	y = -m_radius;
		for(float z = -m_radius; z <= m_radius; z = z + incrementor)
		{
			
			for(float x = m_radius; x >= -m_radius; x = x - incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}
			}
		}
	// Bottom face //-----------------------------------------------------------//

	// Front face //-----------------------------------------------------------//
	numberOfFaces++;
	float z = m_radius;
		for(float x = -m_radius; x <= m_radius; x = x + incrementor)
		{
			
			for(float y = -m_radius; y <= m_radius; y = y + incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}
			}
		}
	// Front face //-----------------------------------------------------------//

	// Back face //-----------------------------------------------------------//
	numberOfFaces++;
	z = -m_radius;
		for(float x = -m_radius; x <= m_radius; x = x + incrementor)
		{
			
			for(float y = -m_radius; y <= m_radius; y = y + incrementor)
			{
				float d = glm::distance(glm::vec3(x,y,z),glm::vec3(0,0,0));
				if( d >= m_radius )
				{
					new_mesh.push_back(x);
					new_mesh.push_back(y);
					new_mesh.push_back(z);
				}
			}
		}
	// Back face //-----------------------------------------------------------//
	

	
	generateCubeIndexes(new_mesh,indexes,((m_radius*2)/incrementor),numberOfFaces);
}

void ObjMesh::generateCubeIndexes(vector<GLfloat> &vertexes,vector<unsigned> & new_indexes, float offset,int numberfaces)
{

	int leftFace = ((offset)*(offset+1))*numberfaces;
	int offset1 = (offset+1)*(offset+1);
	int row = 0;
	int col = 0;
	int face = 0;
	int countFace = 0;

	for( int i = 0; i < leftFace+(numberfaces*offset); i++)
	{
		if(row != offset)
		{
			if(col != offset)
			{
				
				if(i%2 == face)
				{
					new_indexes.push_back(i);			
					new_indexes.push_back(i+1);		
					new_indexes.push_back((i+(offset+1))+1);
					new_indexes.push_back(i+(offset+1));

					//extra 3-------------------------------------------------------------//
					//going up on the indexes //
					if(col == 3 )
					{
						int val;
						if(countFace == 0 )
						{
							val = (4*offset1)+offset+row+1;
							new_indexes.push_back(val);
							val = (4*offset1)+offset+row+2;
							new_indexes.push_back(val);													
						}
						else if(countFace == 1 )
						{
							val = ((4*offset1)+((offset*col)+col))+3;
							new_indexes.push_back(val);
							val = ((4*offset1)+((offset*(col+1))+(col+1)))+3;
							new_indexes.push_back(val);								
						}
						else if(countFace == 2 )
						{
							val = (((5*offset1)-1)-((offset)+(col+1)+col));
							new_indexes.push_back(val);
							val = (((5*offset1)-1)-((offset)+(col+2)+col));
							new_indexes.push_back(val);									
						}	
						else if(countFace == 3 )
						{
							val = ((4*offset1)+((offset*col)+col))+1;
							new_indexes.push_back(val);
							val = ((4*offset1)+((offset*(col+1))+(col+1)))+1;
							new_indexes.push_back(val);								
						}
						else if(countFace == 4 )
						{
							val = ((1*offset1)+((offset*col)+col))+3;
							new_indexes.push_back(val);
							val = ((1*offset1)+((offset*(col+1))+(col+1)))+3;
							new_indexes.push_back(val);								
						}
						else if(countFace == 5 )
						{
							val = ((1*offset1)+((offset*col)+col))+1;
							new_indexes.push_back(val);
							val = ((1*offset1)+((offset*(col+1))+(col+1)))+1;
							new_indexes.push_back(val);			
						}
					}
					else
					{
						new_indexes.push_back((i+(offset+1))+2);
						new_indexes.push_back(i+2);		
					}
					//---------------------------------------------------------------------//

					//-------------extra 1-------------------------------------------------//
					//go left
					int extraBroder = 0;
					
					if(countFace == 3 && row == 3)
					{
						int val = col;								
						new_indexes.push_back((col+(offset+1))+1);
						new_indexes.push_back(col+(offset+1));						
					}
					else if(countFace == 4 && row == 3)
					{
						int val = (offset1*3 - ((offset*col)+col) )-2;								
						new_indexes.push_back(val);
						val = (offset1*3 - ((offset*col)+(col+1)) )-2;
						new_indexes.push_back(val);	
					}
					else if(countFace == 5 && row == 3)
					{
						int val = (offset1*3 - ((offset*(col+1))) ) - col;								
						new_indexes.push_back(val);
						val = (offset1*3 - ((offset*(col+2))) ) - col+1;	
						new_indexes.push_back(val);	
					}
					else
					{
					
						if(row == 3)
						extraBroder= 5;

						new_indexes.push_back(i+((offset+extraBroder)+offset+2)+1);
						new_indexes.push_back(i+((offset+extraBroder)+offset+2));
					}
					//---------------------------------------------------------------------//

					//extra 4-------------------------------------------------------------//
					//go left
					if(row == 0 )
					{
						int val;
						if(countFace == 0)
						{
							val = ((4*offset1)-1)-(offset*2+1)+col;		
							new_indexes.push_back(val+1);
							new_indexes.push_back(val);						
						}
						else if(countFace < 4)
						{
							val = i -((offset*2)+2);		
							new_indexes.push_back((val+(offset+1))+1);
							new_indexes.push_back(val+(offset+1));												
						}
						else if( countFace == 4 )
						{

							val = ((offset*(col+1))-1)+col;		
							new_indexes.push_back((val+(offset+1))+1);
							val = ((offset*(col+2))-1)+(col+1);	
							new_indexes.push_back(val+(offset+1));
						
						}else if( countFace == 5 )
						{
							val = (offset*(col))+col+1;		
							new_indexes.push_back((val+(offset+1))+1);
							val = (offset*(col+1))+col+1;	
							new_indexes.push_back(val+(offset+1));						
						}

						
					}
					else
					{
						int val = i -((offset)+1);		
						new_indexes.push_back(val+1);
						new_indexes.push_back(val);						
					
					}
					//---------------------------------------------------------------------//
					
					//extra 2-------------------------------------------------------------//
					//going down on the indexes //
					if(col == 0 )
					{
						int val;
						if(countFace == 0 )
						{
							val = (5*offset1)+offset+col+1;
							new_indexes.push_back(val);
							val = (5*offset1)+offset+col+2;
							new_indexes.push_back(val);													
						}
						else if(countFace == 1 )
						{
							val = ((5*offset1)+((offset*col)+col))+3;
							new_indexes.push_back(val);
							val = ((5*offset1)+((offset*(col+1))+(col+1)))+3;
							new_indexes.push_back(val);								
						}
						else if(countFace == 2 )
						{
							val = (((6*offset1)-1)-((offset)+(col+1)+col));
							new_indexes.push_back(val);
							val = (((6*offset1)-1)-((offset)+(col+2)+col));
							new_indexes.push_back(val);									
						}	
						else if(countFace == 3 )
						{
							val = ((5*offset1)+((offset*col)+col))+1;
							new_indexes.push_back(val);
							val = ((5*offset1)+((offset*(col+1))+(col+1)))+1;
							new_indexes.push_back(val);								
						}
						else if(countFace == 4 )
						{
							val = (((4*offset1)-1)-(((offset*col)+col))+1);
							new_indexes.push_back(val);
							val = (((4*offset1)-1)-(((offset*(col+1))+(col+1)))+1);
							new_indexes.push_back(val);								
						}
						else if(countFace == 5 )
						{
							val = (((4*offset1)-1)-(((offset*col)+col))+3);
							new_indexes.push_back(val);
							val = (((4*offset1)-1)-(((offset*(col+1))+(col+1)))+3);
							new_indexes.push_back(val);				
						}
					}
					else
					{
						new_indexes.push_back((i+(offset)));
						new_indexes.push_back(i-1);		
					}	
					//---------------------------------------------------------------------//
					
					
				}
				else
				{
					new_indexes.push_back(i+1);		
					new_indexes.push_back((i+(offset+1))+1);
					new_indexes.push_back(i+(offset+1));			
					new_indexes.push_back(i);

					//-------------extra 1-------------------------------------------------//
					//going left
					int extraBroder = 0;
					
					if(countFace == 3 && row == 3)
					{
						int val = col;								
						new_indexes.push_back((col+(offset+1))+1);
						new_indexes.push_back(col+(offset+1));						
					}
					else if(countFace == 4 && row == 3)
					{
						int val = (offset1*3 - ((offset*col)+col) )-2;								
						new_indexes.push_back(val);
						val = (offset1*3 - ((offset*col)+(col+1)) )-2;
						new_indexes.push_back(val);	
					}
					else if(countFace == 5 && row == 3)
					{
						int val = (offset1*3 - ((offset*(col+1))) ) - col;								
						new_indexes.push_back(val);
						val = (offset1*3 - ((offset*(col+2))) ) - col+1;	
						new_indexes.push_back(val);	
					}
					else
					{
					
						if(row == 3)
						extraBroder= 5;

						new_indexes.push_back(i+((offset+extraBroder)+offset+2)+1);
						new_indexes.push_back(i+((offset+extraBroder)+offset+2));
					}
					//---------------------------------------------------------------------//
					
					//extra 2-------------------------------------------------------------//	
					//going down on the indexes //
					if(col == 0 )
					{
						int val;
						if(countFace == 0 )
						{
							val = (5*offset1)+offset+col+1;
							new_indexes.push_back(val);
							val = (5*offset1)+offset+col+2;
							new_indexes.push_back(val);													
						}
						else if(countFace == 1 )
						{
							val = ((5*offset1)+((offset*col)+col))+3;
							new_indexes.push_back(val);
							val = ((5*offset1)+((offset*(col+1))+(col+1)))+3;
							new_indexes.push_back(val);								
						}
						else if(countFace == 2 )
						{
							val = (((6*offset1)-1)-((offset)+(col+1)+col));
							new_indexes.push_back(val);
							val = (((6*offset1)-1)-((offset)+(col+2)+col));
							new_indexes.push_back(val);									
						}	
						else if(countFace == 3 )
						{
							val = ((5*offset1)+((offset*col)+col))+1;
							new_indexes.push_back(val);
							val = ((5*offset1)+((offset*(col+1))+(col+1)))+1;
							new_indexes.push_back(val);								
						}
						else if(countFace == 4 )
						{
							val = (((4*offset1)-1)-(((offset*col)+col))+1);
							new_indexes.push_back(val);
							val = (((4*offset1)-1)-(((offset*(col+1))+(col+1)))+1);
							new_indexes.push_back(val);								
						}
						else if(countFace == 5 )
						{
							val = (((4*offset1)-1)-(((offset*col)+col))+3);
							new_indexes.push_back(val);
							val = (((4*offset1)-1)-(((offset*(col+1))+(col+1)))+3);
							new_indexes.push_back(val);				
						}
					}
					else
					{
						new_indexes.push_back(i-1);		
						new_indexes.push_back((i+(offset)));
						
					}					
					//---------------------------------------------------------------------//
					//extra 3-------------------------------------------------------------//
					//going up on the indexes //
					if(col == 3 )
					{
						int val;
						if(countFace == 0 )
						{
							val = (4*offset1)+offset+row+1;
							new_indexes.push_back(val);
							val = (4*offset1)+offset+row+2;
							new_indexes.push_back(val);													
						}
						else if(countFace == 1 )
						{
							val = ((4*offset1)+((offset*col)+col))+3;
							new_indexes.push_back(val);
							val = ((4*offset1)+((offset*(col+1))+(col+1)))+3;
							new_indexes.push_back(val);								
						}
						else if(countFace == 2 )
						{
							val = (((5*offset1)-1)-((offset)+(col+1)+col));
							new_indexes.push_back(val);
							val = (((5*offset1)-1)-((offset)+(col+2)+col));
							new_indexes.push_back(val);									
						}	
						else if(countFace == 3 )
						{
							val = ((4*offset1)+((offset*col)+col))+1;
							new_indexes.push_back(val);
							val = ((4*offset1)+((offset*(col+1))+(col+1)))+1;
							new_indexes.push_back(val);								
						}
						else if(countFace == 4 )
						{
							val = ((1*offset1)+((offset*col)+col))+3;
							new_indexes.push_back(val);
							val = ((1*offset1)+((offset*(col+1))+(col+1)))+3;
							new_indexes.push_back(val);								
						}
						else if(countFace == 5 )
						{
							val = ((1*offset1)+((offset*col)+col))+1;
							new_indexes.push_back(val);
							val = ((1*offset1)+((offset*(col+1))+(col+1)))+1;
							new_indexes.push_back(val);			
						}
					}
					else
					{
						new_indexes.push_back(i+2);
						new_indexes.push_back((i+(offset+1))+2);								
					}
					//---------------------------------------------------------------------//
					//extra 4-------------------------------------------------------------//
					if(row == 0 )
					{
						int val;
						if(countFace == 0)
						{
							val = ((4*offset1)-1)-(offset*2+1)+col;		
							new_indexes.push_back(val+1);
							new_indexes.push_back(val);						
						}
						else if(countFace < 4)
						{
							val = i -((offset*2)+2);		
							new_indexes.push_back((val+(offset+1))+1);
							new_indexes.push_back(val+(offset+1));												
						}
						else if( countFace == 4 )
						{

							val = ((offset*(col+1))-1)+col;		
							new_indexes.push_back((val+(offset+1))+1);
							val = ((offset*(col+2))-1)+(col+1);	
							new_indexes.push_back(val+(offset+1));
						
						}else if( countFace == 5 )
						{
							val = (offset*(col))+col+1;		
							new_indexes.push_back((val+(offset+1))+1);
							val = (offset*(col+1))+col+1;	
							new_indexes.push_back(val+(offset+1));						
						}

						
					}
					else
					{
						int val = i -((offset)+1);		
						new_indexes.push_back(val+1);
						new_indexes.push_back(val);						
					
					}
					//---------------------------------------------------------------------//
				}
				col++;
				
			
			}
			else
			{
				col = 0;
				row++;					
			}
		}
		else
		{
			row = 0;
			col = 0;
			i = i + offset;
			face = (face+1)%2;
			countFace++;
		}
		
	}
}


