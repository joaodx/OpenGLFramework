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
uniform mat4 P;
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

		
	//if(dist2cam >= 20)
	//	return 2;
	//else if(dist2cam < 20 && dist2cam > 10 )
	//	return 3;
	//else if(dist2cam < 10 && dist2cam > 4 )
	//	return 13;
	//else if(dist2cam < 4 && dist2cam > 2 )
	//	return 20;
	//else if(dist2cam < 2 && dist2cam > 0 )
	//	return 25;

	/*float divisionTess = (100.0/(pow(dist2cam,4)))+1.0;
	return clamp( divisionTess,1.0, 50.0 );*/
}

vec3 midpoint4(vec3 p1, vec3 p2, vec3 p3,vec3 p4)
{
	return (p1+p2+p3+p4)/4.0;
}

vec4 midpoint4(vec4 p1, vec4 p2, vec4 p3,vec4 p4)
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

float SurfaceAreaOnScreen(vec4 tl, vec4 bt, vec4 bl, vec4 tr, bool neighbours)
{

	vec4 topLeft = P*V*M* tl;
	vec4 bottomright = P*V*M* bt;
	vec4 bottomleft = P*V*M* bl;
	vec4 topright = P*V*M* tr;
	vec4 PatchMidPoint = midpoint4(topLeft, topright, bottomleft ,bottomright);


	topLeft.x = ( 1024.0 * ((topLeft.x/topLeft.w + 1.0) / 2.0) );
	topLeft.y = ( 768 * ((1.0 - topLeft.y/topLeft.w) / 2.0) );
	topLeft.z = 0.0;

	bottomright.x = ( 1024.0 * ((bottomright.x/bottomright.w + 1.0) / 2.0) );
	bottomright.y = ( 768 * ((1.0 - bottomright.y/bottomright.w) / 2.0) );
	bottomright.z = 0.0;

	bottomleft.x = ( 1024.0 * ((bottomleft.x/bottomleft.w + 1.0) / 2.0) );
	bottomleft.y = ( 768 * ((1.0 - bottomleft.y/bottomleft.w) / 2.0) );
	bottomleft.z = 0.0;

	topright.x = ( 1024.0 * ((topright.x/topright.w + 1.0) / 2.0) );
	topright.y = ( 768 * ((1.0 - topright.y/topright.w) / 2.0) );
	topright.z = 0.0;

	//float base = distance(bottomleft,topright);

	vec3 a = topright.xyz - topLeft.xyz;
	vec3 b = bottomleft.xyz - topLeft.xyz;
	float angle = acos(dot(normalize(a),normalize(b)));

	//float surfaceArea = abs( topLeft.x -  bottomright.x) * abs( topLeft.y -  bottomright.y);
	float surfaceArea = length(a.xyz)*length(b.xyz)*sin(angle);
	surfaceArea /= 10000.0;
	float percent = clamp(surfaceArea / 64.0f, 0.0, 1.0);
	surfaceArea = (1.0 + percent * (10.0 - 1.0));

	PatchMidPoint.x = ( 1024.0 * ((PatchMidPoint.x/PatchMidPoint.w + 1.0) / 2.0) );
	PatchMidPoint.y = ( 768 * ((1.0 - PatchMidPoint.y/PatchMidPoint.w) / 2.0) );
	PatchMidPoint.z = 0.0;

	if(!neighbours)
	if((topLeft.x > 1024 || topLeft.y > 768 || topLeft.x < 0 || topLeft.y < 0) &&
	(topright.x > 1024 || topright.y > 768 || topright.x < 0 || topright.y < 0) &&
	(bottomleft.x > 1024 || bottomleft.y > 768 || bottomleft.x < 0 || bottomleft.y < 0) &&
	(bottomright.x > 1024 || bottomright.y > 768 || bottomright.x < 0 || bottomright.y < 0) &&
	(PatchMidPoint.x > 1024 || PatchMidPoint.y > 768 || PatchMidPoint.x < 0 || PatchMidPoint.y < 0) )
		surfaceArea = -1.0;


	return surfaceArea;
}

void main(void)
{
	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
	vec4 modelPosition = vec4(0,0,0,1);

	vec3 main = midpoint4(tcPosition[0],tcPosition[1],tcPosition[2],tcPosition[3]);

	
	vec3 midZero =	midpoint4(tcPosition[3],tcPosition[0],tcPosition[10],tcPosition[11]);
	vec3 midOne  =	midpoint4(tcPosition[0],tcPosition[1],tcPosition[9],tcPosition[8]);
	vec3 midTwo =	midpoint4(tcPosition[1],tcPosition[2],tcPosition[4],tcPosition[5]);
	vec3 midThree = midpoint4(tcPosition[2],tcPosition[3],tcPosition[6],tcPosition[7]);
	float tessZero = tessFactor(midZero); 
	float tessOne = tessFactor(midOne); 
	float tessTwo = tessFactor(midTwo);
	float tessThree = tessFactor(midThree);  

	float tessMain = tessFactor(main);
	
	float MainsurfaceArea = SurfaceAreaOnScreen(vec4(tcPosition[0].xyz,1), vec4(tcPosition[1].xyz,1) ,
							vec4(tcPosition[2].xyz,1), vec4(tcPosition[3].xyz,1) ,false);

	float midZerosurfaceArea = SurfaceAreaOnScreen(vec4(tcPosition[3].xyz,1), vec4(tcPosition[0].xyz,1) ,
							vec4(tcPosition[10].xyz,1), vec4(tcPosition[11].xyz,1) , true );

	float midOnesurfaceArea = SurfaceAreaOnScreen(vec4(tcPosition[0].xyz,1), vec4(tcPosition[1].xyz,1) ,
							vec4(tcPosition[9].xyz,1), vec4(tcPosition[8].xyz,1) , true);

	float midTwosurfaceArea = SurfaceAreaOnScreen(vec4(tcPosition[1].xyz,1), vec4(tcPosition[2].xyz,1) ,
							vec4(tcPosition[4].xyz,1), vec4(tcPosition[5].xyz,1), true );

	float midThreesurfaceArea = SurfaceAreaOnScreen(vec4(tcPosition[2].xyz,1), vec4(tcPosition[3].xyz,1) ,
							vec4(tcPosition[6].xyz,1), vec4(tcPosition[7].xyz,1), true );
	
	//float tessFactor =  MainsurfaceArea;




	//if(MainsurfaceArea < 0 || midZerosurfaceArea < 0 || midOnesurfaceArea < 0 ||
	//	midTwosurfaceArea < 0 || midThreesurfaceArea < 0 )
	//{
	//	tessFactor = 1.0;
	//	tessZero = 1.0;
	//	tessOne = 1.0;
	//	tessTwo = 1.0;
	//	tessThree = 1.0;
	//}

	//	tessFactor =  tessMain;
	//	if(tessMain < 5 )
	//	{
	//		tessFactor = tessMain;
	//		midZerosurfaceArea = tessZero;
	//		midOnesurfaceArea = tessOne;
	//		midTwosurfaceArea = tessTwo;
	//		midThreesurfaceArea = tessThree;
	//	}
	//	else
	//	{
	//		tessFactor += tessMain;
	//		midZerosurfaceArea += tessZero;
	//		midOnesurfaceArea += tessOne;
	//		midTwosurfaceArea += tessTwo;
	//		midThreesurfaceArea += tessThree;
	//	}
		
		//vec4 cameraPos = inverse(-V)[3].xyzw;
		//vec3 cameraDir =  normalize( inverse(-V)[2].xyz) ;

		//vec3 vertexDir =  normalize(main.xyz).xyz;
		

		//if(dot(normalize(vertexDir.xyz),normalize(cameraDir.xyz)) > 0.2 )
		//{
		//	tessMain = 0.0;		
		//	//tessFactor = 0.0;	
		//}
		
		//float tessFactor = 1.0;
    if (gl_InvocationID == 0)
    {
	
		gl_TessLevelInner[0] = tessMain;
		gl_TessLevelInner[1] = tessMain;
        gl_TessLevelOuter[0] = min(tessZero,tessMain );
        gl_TessLevelOuter[1] = min(tessOne,tessMain );
        gl_TessLevelOuter[2] = min(tessTwo,tessMain );
		gl_TessLevelOuter[3] = min(tessThree,tessMain );
		
		//gl_TessLevelInner[0] = tessFactor;
		//gl_TessLevelInner[1] = tessFactor;
  //      gl_TessLevelOuter[0] = min(midZerosurfaceArea,tessFactor );
  //      gl_TessLevelOuter[1] = min(midOnesurfaceArea,tessFactor );
  //      gl_TessLevelOuter[2] = min(midTwosurfaceArea,tessFactor );
		//gl_TessLevelOuter[3] = min(midThreesurfaceArea,tessFactor );

		/*gl_TessLevelInner[0] = tessFactor;
		gl_TessLevelInner[1] = tessFactor;
        gl_TessLevelOuter[0] = tessFactor;
        gl_TessLevelOuter[1] = tessFactor;
        gl_TessLevelOuter[2] = tessFactor;
		gl_TessLevelOuter[3] = tessFactor;*/
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	

	VertexOut[gl_InvocationID].texCoord = VertexIn[gl_InvocationID].texCoord;
	VertexOut[gl_InvocationID].normal = VertexIn[gl_InvocationID].normal;
	VertexOut[gl_InvocationID].fragmentColor = VertexIn[gl_InvocationID].fragmentColor;

	//vec3 vector1 = midZero - midTwo;
	//vec3 vector2 = midOne - midThree;

	//vec3 vector1 = midZero - midTwo;
	//vec3 vector2 = midOne - midThree;

	//vec3 c = cross(vector1, vector2);

	//if(dot(VertexOut[gl_InvocationID].normal,c) < 0 )
	//{
	//	c = cross(vector2,vector1);		
	//}
	

	//VertexOut[gl_InvocationID].normal = normalize(c);
	
}