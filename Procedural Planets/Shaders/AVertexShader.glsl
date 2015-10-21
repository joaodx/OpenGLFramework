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
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float radius;
//***************************************************************//




void main(){

	

	vec4 modelPosition = vec4(0,0,0,1);

	// Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
	//gl_Position = M * v;
	gl_Position = v;
	//vPosition = v.xyz;

	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;

	
	vPosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	vPosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	vPosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));

	

	vec3 normal = vPosition - modelPosition.xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;		

	vPosition = modelPosition.xyz + normal*radius ;

	// The color of each vertex will be interpolated
    // to produce the color of each fragment
    VertexOut.fragmentColor = vertexColor;
	VertexOut.texCoord = vertexUv;


 }