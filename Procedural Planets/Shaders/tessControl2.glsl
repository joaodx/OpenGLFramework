#version 430 core

layout (vertices = 3) out;

//*********** IN VARIABLES **********************************//
in VertexData {
    vec2 texCoord;
    vec3 normal;
	vec3 fragmentColor;
} VertexIn[]; 
in vec3 vPosition[];
//**********************************************************//

//*********** OUT VARIABLES **********************************//
out VertexData {
    vec2 texCoord;
    vec3 normal;
	vec3 fragmentColor;
} VertexOut[]; 
out vec3 tcPosition[];
//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 V;
uniform mat4 M;
//***************************************************************//

void main(void)
{
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
		gl_TessLevelInner[0] = 1;
        gl_TessLevelOuter[0] = 1;
        gl_TessLevelOuter[1] = 1;
        gl_TessLevelOuter[2] = 1;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	VertexOut[gl_InvocationID].texCoord = VertexIn[gl_InvocationID].texCoord;
	VertexOut[gl_InvocationID].normal = VertexIn[gl_InvocationID].normal;
	VertexOut[gl_InvocationID].fragmentColor = VertexIn[gl_InvocationID].fragmentColor;
	
}