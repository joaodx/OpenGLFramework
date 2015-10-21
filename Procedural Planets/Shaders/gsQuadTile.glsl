#version 430 core
 
layout(triangles) in;
layout (triangle_strip, max_vertices=4) out;

//*********** IN VARIABLES **********************************//
in VertexData {
    vec2 texCoord;
    vec3 normal;
	float noise;
} VertexIn[3];
 
//**********************************************************//

//*********** OUT VARIABLES **********************************//
out VertexData {
	smooth vec3 position;
    vec2 texCoord;
    vec3 normal;
	float noise;
} VertexOut;
 
//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
//uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
//***************************************************************//

void main()
{

	
	for(int i = 0; i < gl_in.length(); i++)
  {
     // copy attributes
    gl_Position = gl_in[i].gl_Position;
    VertexOut.normal = VertexIn[i].normal;
	VertexOut.position = (M*gl_in[i].gl_Position).xyz;
    VertexOut.texCoord = VertexIn[i].texCoord;
	VertexOut.noise = VertexIn[i].noise;
	gl_Position = P * V * M * gl_Position;
    // done with the vertex
    EmitVertex();

  }	
  EndPrimitive();
}