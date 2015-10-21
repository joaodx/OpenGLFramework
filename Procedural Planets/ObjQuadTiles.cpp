#include "ObjQuadTiles.h"
// Include GLEW
#include <GL/glew.h>








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

ObjQuadTiles::ObjQuadTiles(const string &fileName)
	: Mesh(),m_vertexCount(0), m_indexCount(0), m_transparency(false)
{
	// fetch file name
	string name(fileName);
#ifndef _WIN32
	name = "/app_home/" + name;
#endif

	vector<GLfloat> new_mesh;
	vector<unsigned> new_index;
	generateCubeVertixes2(new_mesh,new_index);

	// load the object file into buffers
	// Read our .obj file
	std::vector< GLfloat > vertices;
	std::vector< GLfloat > uvs;
	std::vector< GLfloat > normals; // Won't be used at the moment.
	loadBuffers(name.c_str(), vertices, uvs,normals);
	
	// record the buffer sizes
	m_vertexCount = new_mesh.size();
	m_indexCount = new_index.size();
	//m_vertexCount = 30;
	//m_indexCount = 12;
	// generate and bind VAO
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

	// generate, bind and buffer EBO/IBO
	GLuint m_ebo;
	glGenBuffers(1,&m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, new_index.size() * sizeof(unsigned), &new_index[0], GL_STATIC_DRAW);

	// generate, bind and buffer VBO
	GLuint m_vbo;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_new_mesh), g_vertex_buffer_data_new_mesh, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, new_mesh.size() * sizeof(GLfloat), &new_mesh[0], GL_STATIC_DRAW);

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

ObjQuadTiles::~ObjQuadTiles()
{
	//glDeleteBuffers(2, m_vbos);
}

void ObjQuadTiles::draw()
{

	glBindVertexArray(m_vao);
	glPatchParameteri(GL_PATCH_VERTICES, 12);
	glDrawElements(GL_PATCHES, m_indexCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

bool ObjQuadTiles::loadBuffers(const char * path, std::vector < GLfloat > & out_vertices,
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

void ObjQuadTiles::generateCubeVertixes(vector<GLfloat> &new_mesh)
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

void ObjQuadTiles::generateCubeVertixes2(vector<GLfloat> &new_mesh,vector<unsigned> &indexes)
{
	GLint m_radius = 1;
	GLfloat numberOfCuts = 5;
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

		for(float x = m_radius; x >= -m_radius; x = x - incrementor)		
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
	// Bottom face //-----------------------------------------------------------//

	// Front face //-----------------------------------------------------------//
	numberOfFaces++;
	float z = m_radius;
	//FIX THIS
		for(float x = m_radius; x >= -m_radius; x = x - incrementor)
		//for(float x = -m_radius; x <= m_radius; x = x + incrementor)
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

void ObjQuadTiles::generateCubeIndexes(vector<GLfloat> &vertexes,vector<unsigned> & new_indexes, float offset,int numberfaces)
{

	
	int offset1 = (offset+1)*(offset+1);
	//int leftFace = ((offset)*(offset+1))*numberfaces;
	int leftFace = ((offset1*numberfaces)-1)-(offset+1);
	int row = 0;
	int col = 0;
	int face = 0;
	int countFace = 0;

	for( int i = 0; i < leftFace; i++)
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
					if(col == offset-1 )
					{
						int val;
						if(countFace == 0 )
						{							

							/*val = (((5*offset1)-1)-((offset)+(row+1)));
							new_indexes.push_back(val);
							val = (((5*offset1)-1)-((offset)+(row+2)));
							new_indexes.push_back(val);	*/

							val = ((5*offset1)-1)- (offset-row+1+(offset-1));
							new_indexes.push_back(val);
							val = ((5*offset1)-1)- (offset-row+2+(offset-1));
							new_indexes.push_back(val);	
						}
						else if(countFace == 1 )
						{

							/*val = ((4*offset1)+((offset*row)+row))+(offset-1);
							new_indexes.push_back(val);
							val = ((4*offset1)+((offset*(row+1))+(row+1)))+(offset-1);
							new_indexes.push_back(val);	*/

							val = (5*offset1-1)-(((offset+1)*row)+1);
							new_indexes.push_back(val);
							val = (5*offset1-1)-(((offset+1)*(row+1))+1);
							new_indexes.push_back(val);
							
						}
						else if(countFace == 2 )
						{

							val = (4*offset1)+ offset-row+1+(offset-1);
							new_indexes.push_back(val);
							val = (4*offset1)+ offset-row+2+(offset-1);
							new_indexes.push_back(val);		
																
						}	
						else if(countFace == 3 )
						{
							val = (4*offset1)+(((offset+1)*row)+1);
							new_indexes.push_back(val);
							val = (4*offset1)+(((offset+1)*(row+1))+1);
							new_indexes.push_back(val);	

							
						}
						else if(countFace == 4 )
						{	

							val = ((2*offset1)-1) - (((offset*row)+row)+1);
							new_indexes.push_back(val);
							val = ((2*offset1)-1) - (((offset*(row+1))+(row+1))+1);
							new_indexes.push_back(val);
						}
						else if(countFace == 5 )
						{
							val = ((1*offset1)+((offset*row)+row))+1;
							new_indexes.push_back(val);
							val = ((1*offset1)+((offset*(row+1))+(row+1)))+1;
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
					//go right
					int extraBroder = 0;
					
					if(countFace == 3 && row == offset-1)
					{
						int val = col;								
						new_indexes.push_back((col+(offset+1))+1);
						new_indexes.push_back(col+(offset+1));						
					}
					else if(countFace == 4 && row == offset-1)
					{

						int val = ((offset*(col+1))-1)+col;		
						new_indexes.push_back(val);
						val = ((offset*(col+2))-1)+(col+1);	
						new_indexes.push_back(val);
					}
					else if(countFace == 5 && row == offset-1)
					{
						int val = (((offset1*3) -1) - ((offset*col)+col) )-(offset-1);							
						new_indexes.push_back(val);
						val =  (((offset1*3) -1) - ((offset*(col+1))+(col+1)) )-(offset-1);
						new_indexes.push_back(val);	
					}
					else
					{
					
						if(row == offset-1)
						extraBroder= offset+1;

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
							new_indexes.push_back(val+1);
							new_indexes.push_back(val);												
						}
						else if( countFace == 4 )
						{			

							int val = (((offset1*3) -1) - ((offset*col)+col) )-1;								
							new_indexes.push_back(val);
							val = (((offset1*3) -1) - ((offset*(col+1))+(col+1)) )-1;
							new_indexes.push_back(val);	
						
						}else if( countFace == 5 )
						{
							val = (offset*(col))+col+1;		
							new_indexes.push_back(val);
							val = (offset*(col+1))+col+1;	
							new_indexes.push_back(val+1);						
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
							val = (5*offset1)+offset+row+1;
							new_indexes.push_back(val);
							val = (5*offset1)+offset+row+2;
							new_indexes.push_back(val);													
						}
						else if(countFace == 1 )
						{
							val = ((5*offset1)+((offset*row)+row))+(offset-1);
							new_indexes.push_back(val);
							val = ((5*offset1)+((offset*(row+1))+(row+1)))+(offset-1);
							new_indexes.push_back(val);								
						}
						else if(countFace == 2 )
						{
							val = (((6*offset1)-1)-((offset)+(row)+1));
							new_indexes.push_back(val);
							val = (((6*offset1)-1)-((offset)+(row+1)+1));
							new_indexes.push_back(val);									
						}	
						else if(countFace == 3 )
						{
							val = (((6*offset1)-1)-((offset*row)+row+(offset-1)));
							new_indexes.push_back(val);
							val = (((6*offset1)-1)-((offset*(row+1))+(row+1)+(offset-1)));
							new_indexes.push_back(val);								
						}
						else if(countFace == 4 )
						{

							/*val = ((4*offset1)-1) - (((offset*row)+row)+1);
							new_indexes.push_back(val);
							val = ((4*offset1)-1) - (((offset*(row+1))+(row+1))+1);
							new_indexes.push_back(val);*/		

							val = ((3*offset1)+(offset*(row+1))-1)+row;		
							new_indexes.push_back(val);
							val = ((3*offset1)+(offset*(row+2))-1)+(row+1);	
							new_indexes.push_back(val);
						}
						else if(countFace == 5 )
						{
							val = ((4*offset1)-1) - (((offset*row)+row)+(offset-1));
							new_indexes.push_back(val);
							val = ((4*offset1)-1) - (((offset*(row+1))+(row+1))+(offset-1));
							new_indexes.push_back(val);				
						}
					}
					else
					{
						new_indexes.push_back((i+(offset)));
						new_indexes.push_back(i-1);		
					}	
					//---------------------------------------------------------------------//
					
//---------------------------------------------------------------------------------------------------------------------------------------------//					
				}
				else
				{
					new_indexes.push_back(i+1);		
					new_indexes.push_back((i+(offset+1))+1);
					new_indexes.push_back(i+(offset+1));			
					new_indexes.push_back(i);

					//-------------extra 1-------------------------------------------------//
					//going right
					int extraBroder = 0;
					
					if(countFace == 3 && row == offset-1)
					{
						int val = col;								
						new_indexes.push_back((col+(offset+1))+1);
						new_indexes.push_back(col+(offset+1));						
					}
					else if(countFace == 4 && row == offset-1)
					{
						/*int val = (((offset1*3) -1) - ((offset*col)+col) )-1;								
						new_indexes.push_back(val);
						val = (((offset1*3) -1) - ((offset*(col+1))+(col+1)) )-1;
						new_indexes.push_back(val);	*/

						int val = ((offset*(col+1))-1)+col;		
						new_indexes.push_back(val);
						val = ((offset*(col+2))-1)+(col+1);	
						new_indexes.push_back(val);
					}
					else if(countFace == 5 && row == offset-1)
					{
						int val =  (((offset1*3) -1) - ((offset*col)+col) )-(offset-1);							
						new_indexes.push_back(val);
						val =  (((offset1*3) -1) - ((offset*(col+1))+(col+1)) )-(offset-1);
						new_indexes.push_back(val);	
					}
					else
					{
					
						if(row == offset-1)
						extraBroder= offset+1;

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
							val = (5*offset1)+offset+row+1;
							new_indexes.push_back(val);
							val = (5*offset1)+offset+row+2;
							new_indexes.push_back(val);													
						}
						else if(countFace == 1 )
						{
							val = ((5*offset1)+((offset*row)+row))+(offset-1);
							new_indexes.push_back(val);
							val = ((5*offset1)+((offset*(row+1))+(row+1)))+(offset-1);
							new_indexes.push_back(val);								
						}
						else if(countFace == 2 )
						{
							val = (((6*offset1)-1)-((offset)+(row)+1));
							new_indexes.push_back(val);
							val = (((6*offset1)-1)-((offset)+(row+1)+1));
							new_indexes.push_back(val);									
						}	
						else if(countFace == 3 )
						{
							val = (((6*offset1)-1)-((offset*row)+row+(offset-1)));
							new_indexes.push_back(val);
							val = (((6*offset1)-1)-((offset*(row+1))+(row+1)+(offset-1)));
							new_indexes.push_back(val);								
						}
						else if(countFace == 4 )
						{
							/*val = ((4*offset1)-1) - (((offset*row)+row)+1);
							new_indexes.push_back(val);
							val = ((4*offset1)-1) - (((offset*(row+1))+(row+1))+1);
							new_indexes.push_back(val);	*/

							val = ((3*offset1)+(offset*(row+1))-1)+row;		
							new_indexes.push_back(val);
							val = ((3*offset1)+(offset*(row+2))-1)+(row+1);	
							new_indexes.push_back(val);
						}
						else if(countFace == 5 )
						{
							val = ((4*offset1)-1) - (((offset*row)+row)+(offset-1));
							new_indexes.push_back(val);
							val = ((4*offset1)-1) - (((offset*(row+1))+(row+1))+(offset-1));
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
					if(col == offset-1 )
					{
						int val;
						if(countFace == 0 )
						{

							/*val = (((5*offset1)-1)-((offset)+(row+1)));
							new_indexes.push_back(val);
							val = (((5*offset1)-1)-((offset)+(row+2)));
							new_indexes.push_back(val);	*/

							val = ((5*offset1)-1)- (offset-row+1+(offset-1));
							new_indexes.push_back(val);
							val = ((5*offset1)-1)- (offset-row+2+(offset-1));
							new_indexes.push_back(val);	

																			
						}
						else if(countFace == 1 )
						{
							/*val = ((4*offset1)+((offset*row)+row))+(offset-1);
							new_indexes.push_back(val);
							val = ((4*offset1)+((offset*(row+1))+(row+1)))+(offset-1);
							new_indexes.push_back(val);	*/

							val = (5*offset1-1)-(((offset+1)*row)+1);
							new_indexes.push_back(val);
							val = (5*offset1-1)-(((offset+1)*(row+1))+1);
							new_indexes.push_back(val);
								
														
						}
						else if(countFace == 2 )
						{

							val = (4*offset1)+ offset-row+1+(offset-1);
							new_indexes.push_back(val);
							val = (4*offset1)+ offset-row+2+(offset-1);
							new_indexes.push_back(val);	
													
						}	
						else if(countFace == 3 )
						{
							
							val = (4*offset1)+(((offset+1)*row)+1);
							new_indexes.push_back(val);
							val = (4*offset1)+(((offset+1)*(row+1))+1);
							new_indexes.push_back(val);
							
						}
						else if(countFace == 4 )
						{
							/*val = ((2*offset1 -1)-((offset*row)+row))+(offset-1);
							new_indexes.push_back(val);
							val = ((2*offset1 -1)-((offset*(row+1))+(row+1)))+(offset-1);
							new_indexes.push_back(val);	*/

							val = ((2*offset1)-1) - (((offset*row)+row)+1);
							new_indexes.push_back(val);
							val = ((2*offset1)-1) - (((offset*(row+1))+(row+1))+1);
							new_indexes.push_back(val);
						}
						else if(countFace == 5 )
						{
							val = ((1*offset1)+((offset*row)+row))+1;
							new_indexes.push_back(val);
							val = ((1*offset1)+((offset*(row+1))+(row+1)))+1;
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
					//going left
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
							new_indexes.push_back(val+1);
							new_indexes.push_back(val);												
						}
						else if( countFace == 4 )
						{
							/*val = ((offset*(col+1))-1)+col;		
							new_indexes.push_back(val);
							val = ((offset*(col+2))-1)+(col+1);	
							new_indexes.push_back(val);*/

							int val = (((offset1*3) -1) - ((offset*col)+col) )-1;								
							new_indexes.push_back(val);
							val = (((offset1*3) -1) - ((offset*(col+1))+(col+1)) )-1;
							new_indexes.push_back(val);	
						
						}else if( countFace == 5 )
						{
							val = (offset*(col))+col+1;		
							new_indexes.push_back(val);
							val = (offset*(col+1))+col+1;	
							new_indexes.push_back(val+1);						
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


