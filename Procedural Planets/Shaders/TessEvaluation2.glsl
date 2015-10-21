#version 430 core

layout(triangles, equal_spacing, ccw) in;

//*********** IN VARIABLES **********************************//
in VertexData {
    vec2 texCoord;
    vec3 normal;
	vec3 fragmentColor;
} VertexIn[]; 
in vec3 tcPosition[];
//**********************************************************//

//*********** OUT VARIABLES **********************************//
out VertexData {
    vec2 texCoord;
    vec3 normal;
	vec3 fragmentColor;
} VertexOut; 

//************************************************************//

//******************************************** FUNCTIONS *****************************************//
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}
 
//************************************************************************************************//
void main(void)
{
	vec3 tePosition  = (gl_TessCoord.x * tcPosition[0]) +
                  (gl_TessCoord.y * tcPosition[1]) +
                  (gl_TessCoord.z * tcPosition[2]);
	gl_Position = vec4(tePosition, 1);


	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );
	VertexOut.normal = interpolate3D( VertexIn[0].normal, VertexIn[1].normal, VertexIn[2].normal );
	VertexOut.fragmentColor = interpolate3D( VertexIn[0].fragmentColor, VertexIn[1].fragmentColor, VertexIn[2].fragmentColor );
	
}



