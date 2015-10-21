#version 430 core
//*********** IN VARIABLES **********************************//
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 UV;
// Notice that the "1" here equals the "1" in glVertexAttribPointer

//**********************************************************//

//*********** OUT VARIABLES **********************************//
out vec2 uv_out;


//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
//***************************************************************//
void main(){

	// Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
	gl_Position = v;

	
	// The color of each vertex will be interpolated
    // to produce the color of each fragment
	uv_out = UV;
 }