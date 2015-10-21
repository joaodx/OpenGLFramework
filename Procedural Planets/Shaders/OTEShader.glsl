#version 430 core

layout(quads, equal_spacing, cw) in;

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
	vec3 scattering;
	vec3 extinction;	
	vec3 light;
} VertexOut; 

//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform float radius;

//***************************************************************//
const float radiusOcean = radius-0.02;

//******************************************** FUNCTIONS *****************************************//
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec3 interpolate3Dquads(vec3 v0, vec3 v1, vec3 v2, vec3 v3) //linear interpolation for x,y,z coords on the quad
{
    return mix(mix(v0,v1,gl_TessCoord.x),mix(v3,v2,gl_TessCoord.x),gl_TessCoord.y);
};

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
	float lon = (abs(z) == r ? 0.0 : atan(y,x) );
	//float lon = atan(y,x);	
	return vec2(lat,lon);
}
 
//************************************************************************************************//
void main(void)
{
	vec4 modelPosition = vec4(0,0,0,1);
	//vec3 eyePosition  = (-V*vec4(0,0,0,1)).xyz;


	vec3 tePosition = interpolate3Dquads( tcPosition[0], tcPosition[1], tcPosition[2], tcPosition[3]);
	gl_Position.xyz = interpolate3Dquads( gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);


	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );

	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;	

	tePosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	tePosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	tePosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));


	vec3 normal = tePosition - vec4(0,0,0,1).xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;

	//float perlinN = perlinNoise_3DRigidMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	tePosition =  normal*(radiusOcean);

	gl_Position = vec4(tePosition, 1);
}



