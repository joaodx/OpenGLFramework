#version 430 core

layout (vertices = 12) out;

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

float tessFactor(vec3 p)
{
	float dist2cam = length(V*M*vec4(p,1));
	if(dist2cam >= 20)
		return 2;
	else if(dist2cam < 20 && dist2cam > 10 )
		return 3;
	else if(dist2cam < 10 && dist2cam > 4 )
		return 8;
	else if(dist2cam < 4 && dist2cam > 2 )
		return 10;
	else if(dist2cam < 2 && dist2cam > 0 )
		return 25;
	//float divisionTess = (20.0/(pow(dist2cam,4)))+1.0;
	//return clamp( divisionTess,1.0, 50.0 );	
}

vec3 midpoint4(vec3 p1, vec3 p2, vec3 p3,vec3 p4)
{
	return (p1+p2+p3+p4)/4.0;
}

vec3 midpoint3(vec3 p1, vec3 p2, vec3 p3)
{
	return (p1+p2+p3)/3.0;
}

vec3 midpoint2(vec3 p1, vec3 p2)
{
	return (p1+p2)/3.0;
}

void main(void)
{
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];


	float tessFactor = 3.0;
    if (gl_InvocationID == 0)
    {
		gl_TessLevelInner[0] = tessFactor;
		gl_TessLevelInner[1] = tessFactor;
        gl_TessLevelOuter[0] = tessFactor;
        gl_TessLevelOuter[1] = tessFactor;
        gl_TessLevelOuter[2] = tessFactor;
		gl_TessLevelOuter[3] = tessFactor;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	

	VertexOut[gl_InvocationID].texCoord = VertexIn[gl_InvocationID].texCoord;
	VertexOut[gl_InvocationID].normal = VertexIn[gl_InvocationID].normal;
	VertexOut[gl_InvocationID].fragmentColor = VertexIn[gl_InvocationID].fragmentColor;
	
}