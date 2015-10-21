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
	float noise;
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


//************************************************************************************************************//
//Noise funtions *********************************************************************************************//
//************************************************************************************************************//
float floatNoise(float x, float y, float z)
{
	int val = int(x) + int(y) + int(z) * 57;
	val = (val<<13)^val;
	return (1.0 - ( (val * (val * val * 15731 + 789221) + 1376312589 ) & 0x7fffffff ) / 1073741824.0 );
}

float floatNoise2(float x, float y, float z)
{
	int val = int(x) + int(y) + int(z) * 57;
	val = (val<<13)^val;
	return val*val*val*(val*(val*6-15)+10);
}

float smoothNoise(float x, float y, float z)
{
	float corners = (floatNoise(x-1,y-1,z-1) + floatNoise(x-1,y-1,z+1) + floatNoise(x+1,y-1,z-1) + floatNoise(x+1,y-1,z+1) + floatNoise(x-1,y+1,z-1) + floatNoise(x-1,y+1,z+1) + floatNoise(x+1,y+1,z-1) + floatNoise(x+1,y+1,z+1) ) / 32.0;
	float sides = (floatNoise(x-1,y,z) + floatNoise(x+1,y,z) + floatNoise(x,y-1,z) + floatNoise(x,y+1,z) + floatNoise(x,y,z-1) + floatNoise(x,y,z+1) ) / 16.0;
	float center = (floatNoise(x,y,z)  ) / 8.0;

	return corners + sides + center;
}

float linearInterpolation(float a, float b, float x)
{
	return x * (1-x) + b*x;
}

float cosineInterpolation(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0- cos(ft) ) *0.5f;

	return a * (1.0-f) + b*f;
}

int fastfloor(double x) {
	if(x>0)
		return int(x);
	else
		return int(x-1);
 }

float InterpolatedNoise( float x, float y, float z)
{
	float val_x =  fastfloor(x);
	float fractional_x = x - val_x;

	float val_y =  fastfloor(y);
	float fractional_y = y - val_y;

	float val_z =  fastfloor(z);
	float fractional_z = z - val_z;

	float v1 = floatNoise(val_x,	val_y,	 val_z);
	float v2 = floatNoise(val_x+1, val_y,	 val_z);
	float v3 = floatNoise(val_x,	val_y+1, val_z);
	float v4 = floatNoise(val_x+1, val_y+1, val_z);
	float v5 = floatNoise(val_x,	val_y,	 val_z+1);
	float v6 = floatNoise(val_x+1, val_y,	 val_z+1);
	float v7 = floatNoise(val_x,	val_y+1, val_z+1);
	float v8 = floatNoise(val_x+1, val_y+1, val_z+1);

	float i1 = cosineInterpolation(v1,v2,fractional_x );
	float i2 = cosineInterpolation(v3,v4,fractional_x );
	float i3 = cosineInterpolation(v5,v6,fractional_x );
	float i4 = cosineInterpolation(v7,v8,fractional_x );

	float ii1 = cosineInterpolation(i1,i2,fractional_y );
	float ii2 = cosineInterpolation(i3,i4,fractional_y );

	return cosineInterpolation(ii1,ii2,fractional_z );
}

float perlinNoise_3D(float x, float y, float z)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;

	for(int i = 0; i < n ; i++)
	{/*
		float frequency = pow(2,i);
		float amplitude = pow(p,i);*/

		//float frequency = initfrq*pow(frqgain,i);
		//float amplitude = initamp*pow(ampgain,i);

		//total += InterpolatedNoise(x * frequency, y * frequency , z * frequency) * amplitude;

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);

		total += InterpolatedNoise(x / frequency, y / frequency , z / frequency) * amplitude;
	
	}



	return total;
}

float perlinNoise_3DRigid(float x, float y, float z)
{
	float total = 0;
	float ampgainl = 0.57;
	float frqgainl = 4.93;
	float initampl = 0.701;
	float initfrql = 0.337;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrql/pow(2,i);
		float amplitude = initampl/pow(2,i);
		
		float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency) ;


		//float frequency = initfrql*pow(frqgainl,i);
		//float amplitude = initampl*pow(ampgainl,i);

		//float add = InterpolatedNoise(x * frequency, y * frequency , z * frequency) ;
		if( add > 0)
		{
			add = (-add) + 1;
		}	
		else
		{
			add = add + 1;
		}
		total += add * amplitude;

	}
	return total;
}


float perlinNoise_3DMultiFractal(float x, float y, float z)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;
	float k = 0.0001;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);
		
		float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency)* amplitude ;


		//float frequency = initfrq*pow(frqgain,i);
		//float amplitude = initamp*pow(ampgain,i);

		//float add = InterpolatedNoise(x * frequency, y * frequency , z * frequency) * amplitude;
		
		if( i > 1)
		{
			add *= k * total;
		}	

		total += add ;

	}
	return total;
}

float perlinNoise_3DRigidMultiFractal(float x, float y, float z)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;
	float k = 1.0;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);
		
		//float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency) ;


		//float frequency = initfrq*pow(frqgain,i);
		//float amplitude = initamp*pow(ampgain,i);

		float add = InterpolatedNoise(x / frequency, y / frequency , z / frequency);
		float add2 = InterpolatedNoise(x / frequency, y / frequency , z / frequency)* amplitude;

		if( add > 0)
		{
			add = (-add) + 1.0;		
		}	
		else
		{
			add = add + 1.0;
		}

		if( i > 1)
		{
			add2 *= k * total;
		}
		total += add2 * add * amplitude;

	}
	return total;
}
//********************** 3D SIMPLEX NOISE **************************************************************************************//


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
 
//************************************************************************************************//
void main(void)
{
	vec4 modelPosition = M*vec4(0,0,0,1);
	vec3 tePosition = interpolate3Dquads( tcPosition[0], tcPosition[1], tcPosition[2], tcPosition[3]);
	gl_Position.xyz = interpolate3Dquads( gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);


	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );
	VertexOut.normal = interpolate3D( VertexIn[0].normal, VertexIn[1].normal, VertexIn[2].normal );
	//VertexOut.fragmentColor = interpolate3D( VertexIn[0].fragmentColor, VertexIn[1].fragmentColor, VertexIn[2].fragmentColor );


	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;	

	tePosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	tePosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	tePosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));

	vec3 normal = tePosition - modelPosition.xyz;
	normal = normalize(normal);
	//VertexOut.normal = normal;

	float perlinN = perlinNoise_3DRigidMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	//float perlinN = SimplexPerlinNoise_3D(tePosition.x,tePosition.y,tePosition.z);
	tePosition = modelPosition.xyz + normal*radius + normal * perlinN;
	//tePosition += VertexOut.normal * abs(perlinN);

	VertexOut.noise = perlinN;
	gl_Position = vec4(tePosition, 1);
}



