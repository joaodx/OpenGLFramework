#version 430 core

//*********** IN VARIABLES **********************************//
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUv;
//**********************************************************//

//*********** OUT VARIABLES **********************************//
// Output data ; will be interpolated for each fragment.
out VertexData {
    vec2 texCoord;
    vec3 normal;
	vec3 fragmentColor;
} VertexOut;

out vec3 vPosition;
//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
//***************************************************************//
void main(){

	// Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
	gl_Position = v;
	vPosition = gl_Position.xyz;

	// The color of each vertex will be interpolated
    // to produce the color of each fragment
    VertexOut.fragmentColor = vec3(1,0,1);
	VertexOut.texCoord = vertexUv;
	VertexOut.normal =  vertexNormal;
 }