#include "QuadMesh.h"
// Include GLEW
#include <GL/glew.h>



// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data_new_mesh[] = {
      1.0f,1.0f, 0.0f,	 //0 
      1.0f,-1.0f, 0.0f,  //1
	 -1.0f,-1.0f,0.0f,   //2
	 -1.0f,1.0f, 0.0f,   //3 
};

static const GLfloat g_uv_buffer_data_new_mesh[] = {
      1.0f,1.0f,		 //0 
      1.0f,0.0f,		 //1
	  0.0f,0.0f,		 //2
	  0.0f,1.0f,         //3 

};



// An array of 3 vectors which represents 3 vertices
static const unsigned g_index_buffer_data_new_mesh_cube[] = {
	 0,1,2, 
	 2,3,0,//front
};//85 vertex

// One color for each vertex. They were generated randomly.


#undef max
#undef min

QuadMesh::QuadMesh(const string &fileName)
	: Mesh(),m_vertexCount(0), m_indexCount(0), m_transparency(false)
{
	// fetch file name
	string name(fileName);
#ifndef _WIN32
	name = "/app_home/" + name;
#endif

	vector<GLfloat> new_mesh;
	generateCubeVertixes(new_mesh);

	// load the object file into buffers
	// Read our .obj file
	std::vector< GLfloat > vertices;
	std::vector< GLfloat > uvs;
	std::vector< GLfloat > normals; // Won't be used at the moment.
	loadBuffers(name.c_str(), vertices, uvs,normals);
	
	// record the buffer sizes
	m_vertexCount = new_mesh.size();
	//m_indexCount = m_indexBuffer.size();
	//m_vertexCount = 30;
	//m_indexCount = 12;
	// generate and bind VAO
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

	// generate, bind and buffer EBO/IBO
	GLuint m_ebo;
	glGenBuffers(1,&m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data_new_mesh_cube) , g_index_buffer_data_new_mesh_cube, GL_STATIC_DRAW);

	// generate, bind and buffer VBO
	GLuint m_vbo;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_new_mesh), g_vertex_buffer_data_new_mesh, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, new_mesh.size() * sizeof(GLfloat), &new_mesh[0], GL_STATIC_DRAW);

	//VERTEX
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, stride, (const GLvoid*)offset);
	glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 3* sizeof(GLfloat), (const GLvoid*)0);
	//offset += 4 * sizeof(float);

	//UVS
	// generate , bind a UVS buffer
	GLuint uvsbuffer;
	glGenBuffers(1, &uvsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvsbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_new_mesh), g_uv_buffer_data_new_mesh, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2, GL_FLOAT,GL_FALSE, 2* sizeof(GLfloat), (const GLvoid*)0);

	// unbind VAO, VBO and IBO/EBO
	glBindVertexArray(0);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
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

QuadMesh::~QuadMesh()
{
	//glDeleteBuffers(2, m_vbos);
}

void QuadMesh::draw()
{
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(0);*/

	glBindVertexArray(m_vao);
	//glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	//glPatchParameteri(GL_PATCH_VERTICES, 6);
	//glDrawArrays( GL_PATCHES, 0, m_vertexCount);	
	glDrawElements(GL_TRIANGLES, 1152, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

bool QuadMesh::loadBuffers(const char * path, std::vector < GLfloat > & out_vertices,
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

void QuadMesh::generateCubeVertixes(vector<GLfloat> &new_mesh)
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


