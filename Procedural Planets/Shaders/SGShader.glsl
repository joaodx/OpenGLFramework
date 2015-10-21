#version 430 core
 
layout(triangles) in;
layout (triangle_strip, max_vertices=64) out;

//*********** IN VARIABLES **********************************//
in VertexData {
    vec2 texCoord;
    vec3 normal;
	vec3 scattering;
	vec3 extinction;	
	vec3 light;
} VertexIn[3];
 
//**********************************************************//

//*********** OUT VARIABLES **********************************//
out VertexData {
	smooth vec3 position;
    vec2 texCoord;
    vec3 normal;
	vec3 scattering;
	vec3 extinction;	
	vec3 light;
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
	VertexOut.scattering = VertexIn[i].scattering;
	VertexOut.extinction = VertexIn[i].extinction;
	VertexOut.light = VertexIn[i].light;
	gl_Position = P * V * M * gl_Position;
    // done with the vertex
    EmitVertex();

  }	
  EndPrimitive();
}