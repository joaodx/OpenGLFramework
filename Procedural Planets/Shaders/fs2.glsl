#version 430 core


//*********** IN VARIABLES **********************************//
// Interpolated values from the vertex shaders
in VertexData {
    vec3 position;
    vec2 texCoord;
    vec3 normal;
} FragmentIn;

//**********************************************************//

//*********** OUT VARIABLES **********************************//
out vec3 position;
//**********************************************************//

//*********** UNIFORM VARIABLES **********************************//

//***************************************************************//

 
void main(){


	//position = vec3(1,0,0);
	position = FragmentIn.position;
}