#version 430 core

layout (vertices = 6) out;

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
	if(dist2cam >= 40)
		return 2;
	else if(dist2cam < 40 && dist2cam > 30 )
		return 5;
	else if(dist2cam < 30 && dist2cam > 20 )
		return 10;
	else if(dist2cam < 20 && dist2cam > 18 )
		return 15;
	else if(dist2cam < 18 && dist2cam > 17 )
		return 17;
	else if(dist2cam < 17 && dist2cam > 12 )
		return 20;
	else if(dist2cam < 12 && dist2cam > 10 )
		return 25;
	else if(dist2cam < 10 )
		return 30;
	//float divisionTess = (100.0/(pow(dist2cam,4)))+1.0;
	//return clamp( divisionTess,1.0, 50.0 );	
}

vec3 midpoint(vec3 p1, vec3 p2, vec3 p3)
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

	vec3 main = midpoint(tcPosition[0],tcPosition[1],tcPosition[2]);
	

	vec3 midZero = midpoint(tcPosition[1],tcPosition[2],tcPosition[4]);
	vec3 midOne  = midpoint(tcPosition[2],tcPosition[0],tcPosition[5]);
	vec3 midTwo = midpoint(tcPosition[0],tcPosition[1],tcPosition[3]);
	float tessZero = tessFactor(midZero); 
	float tessOne = tessFactor(midOne); 
	float tessTwo = tessFactor(midTwo); 
	float tessMain = tessFactor(main);

	//float first = tessFactor(tcPosition[0]);
	//float second = tessFactor(tcPosition[1]);
	//float third = tessFactor(tcPosition[2]);
	//float Firstthird = tessFactor(midpoint2(tcPosition[0],tcPosition[2]));
	//float FirstSecond = tessFactor(midpoint2(tcPosition[0],tcPosition[1]));
	//float Secondthird = tessFactor(midpoint2(tcPosition[1],tcPosition[2]));
	
	//tessMain = min(tessMain, min(first, min(second,third) ) );
//	tessMain = min(tessMain, min(Firstthird, min(FirstSecond,Secondthird) ) );
	// mesmo pos outros..

    if (gl_InvocationID == 0)
    {
       gl_TessLevelInner[0] = tessMain;
        gl_TessLevelOuter[0] = min(tessZero,tessMain );
        gl_TessLevelOuter[1] = min(tessOne, tessMain);
        gl_TessLevelOuter[2] = min(tessTwo, tessMain);

		 //gl_TessLevelInner[0] = 5;
   //     gl_TessLevelOuter[0] = 5;
   //     gl_TessLevelOuter[1] = 5;
   //     gl_TessLevelOuter[2] = 5;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	VertexOut[gl_InvocationID].texCoord = VertexIn[gl_InvocationID].texCoord;
	VertexOut[gl_InvocationID].normal = VertexIn[gl_InvocationID].normal;
	VertexOut[gl_InvocationID].fragmentColor = VertexIn[gl_InvocationID].fragmentColor;
	
}