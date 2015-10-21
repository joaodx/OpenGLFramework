#version 430 core

layout(triangles, equal_spacing, cw) in;
//layout(triangles, fractional_even_spacing, ccw) in;
//layout(triangles, fractional_odd_spacing, cw) in;





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
} VertexOut; 

//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 M;
uniform float ampgain;
uniform float frqgain;
uniform float initamp;
uniform float initfrq;
uniform float radius;
uniform float octv;
//***************************************************************//




//******************************************** FUNCTIONS *****************************************//
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec3 translateLLtoCoords(float lat, float lon, float R)
{


	float x =  R*sin(lat)*cos(lon);
	float y =  R*sin(lat)*sin(lon);
	float z =  R*cos(lat);

	
	return vec3(x,y,z);
}

vec2 translateCoordsToLL(float x, float y, float z)
{
	float r = length(vec3(x,y,z));
	float lat = acos(z/r);
	float lon = (abs(z) == r ? 0 : atan(y,x));	
	
	return vec2(lat,lon);
}
 
//************************************************************************************************//


void main(void)
{

	vec4 modelPosition = M*vec4(0,0,0,1);
	

	vec3 tePosition  = (gl_TessCoord.x * tcPosition[0]) +
                  (gl_TessCoord.y * tcPosition[1]) +
                  (gl_TessCoord.z * tcPosition[2]);

	

	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );
	VertexOut.normal = interpolate3D( VertexIn[0].normal, VertexIn[1].normal, VertexIn[2].normal );



	vec3 normal = tePosition - modelPosition.xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;

	
	gl_Position = vec4(tePosition, 1);
}



