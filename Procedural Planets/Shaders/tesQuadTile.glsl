#version 430 core

layout(quads, fractional_even_spacing, cw) in;

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

uniform bool simplexNoise;
uniform bool rigidMultiFractal;
uniform bool rigid;
uniform bool multiFractal;
uniform bool perlinNoise;
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



//https://github.com/OpenGLInsights/OpenGLInsightsCode/tree/master/Chapter%2007%20Procedural%20Textures%20in%20GLSL/demo1
//***************************************************************************************************//


vec4 permute(vec4 x)
{
  return mod(((x*34.0)+1.0)*x, 289.0);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //  x0 = x0 - 0. + 0.0 * C 
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

// Permutations
  i = mod(i, 289.0 ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients
// ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }


 float perlinNoise_snoise(vec3 point)
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

		float frequency = initfrq*pow(2,i);
		float amplitude = initamp/pow(2,i);

		float add = amplitude * (snoise(point * frequency));

		total += add;


	}
	return total;
}
 //***************************************************************************************************//

void main(void)
{
	vec4 modelPosition = vec4(0,0,0,1);
	vec3 tePosition = interpolate3Dquads( tcPosition[0], tcPosition[1], tcPosition[2], tcPosition[3]);
	gl_Position.xyz = interpolate3Dquads( gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);

	//if(length(tePosition) == 0)
	//{
	//	gl_Position = vec4(0,0,0,0);
	//	return;
	//}

	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );
	VertexOut.normal = interpolate3D( VertexIn[0].normal, VertexIn[1].normal, VertexIn[2].normal );
	//VertexOut.fragmentColor = interpolate3D( VertexIn[0].fragmentColor, VertexIn[1].fragmentColor, VertexIn[2].fragmentColor );


	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;	

	tePosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	tePosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	tePosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));
	
	vec2 pointLL = translateCoordsToLL(tePosition.x,tePosition.y,tePosition.z);

	// tePosition = (M * vec4(tePosition,1)).xyz; XICO

	//------------------------- AUXILIAR POINT FOR NORMAL --------------------------------------------//
	float offset = radians(1.0);
	vec3 pointNorh = translateLLtoCoords(pointLL.x+offset,pointLL.y,1);
	vec3 normalAux = pointNorh - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);

	// pointNorh = (M * vec4(pointNorh,1)).xyz; XICO 

	//float n = perlinNoise_snoise(pointNorh);
	//float perlinN = perlinNoise_3DRigidMultiFractal(pointNorh.x,pointNorh.y,pointNorh.z);
	//float perlinN2 = perlinNoise_3DRigid(pointNorh.x,pointNorh.y,pointNorh.z);
	//float perlinN3 = perlinNoise_3DMultiFractal(pointNorh.x,pointNorh.y,pointNorh.z);
	//float perlinN4 =perlinNoise_3D(pointNorh.x,pointNorh.y,pointNorh.z);
	float final_perlin = 0;
	if(simplexNoise)
		final_perlin = perlinNoise_snoise(pointNorh);
	else if(rigidMultiFractal)
			final_perlin = perlinNoise_3DRigidMultiFractal(pointNorh.x,pointNorh.y,pointNorh.z);
	else if(rigid)
			final_perlin = perlinNoise_3DRigid(pointNorh.x,pointNorh.y,pointNorh.z);
	else if(multiFractal)
			final_perlin = perlinNoise_3DMultiFractal(pointNorh.x,pointNorh.y,pointNorh.z);
	else if(perlinNoise)
			final_perlin = perlinNoise_3D(pointNorh.x,pointNorh.y,pointNorh.z);


	pointNorh =  normalAux*radius + normalAux * final_perlin;
	// pointNorh =  modelPosition.xyz + normalAux*radius + normalAux * n; XICO

	vec3 pointSouth = translateLLtoCoords(pointLL.x-offset,pointLL.y,1);
	normalAux = pointSouth - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);

	// pointSouth = (M * vec4(pointSouth,1)).xyz; XICO 

	 //n = perlinNoise_snoise(pointSouth);
	 //perlinN = perlinNoise_3DRigidMultiFractal(pointSouth.x,pointSouth.y,pointSouth.z);
	 //perlinN2 = perlinNoise_3DRigid(pointSouth.x,pointSouth.y,pointSouth.z);
	 //perlinN3 = perlinNoise_3DMultiFractal(pointSouth.x,pointSouth.y,pointSouth.z);
	 //perlinN4 =perlinNoise_3D(pointSouth.x,pointSouth.y,pointSouth.z);
	 final_perlin = 0;
	if(simplexNoise)
		final_perlin = perlinNoise_snoise(pointSouth);
	else if(rigidMultiFractal)
			final_perlin = perlinNoise_3DRigidMultiFractal(pointSouth.x,pointSouth.y,pointSouth.z);
	else if(rigid)
			final_perlin = perlinNoise_3DRigid(pointSouth.x,pointSouth.y,pointSouth.z);
	else if(multiFractal)
			final_perlin = perlinNoise_3DMultiFractal(pointSouth.x,pointSouth.y,pointSouth.z);
	else if(perlinNoise)
			final_perlin = perlinNoise_3D(pointSouth.x,pointSouth.y,pointSouth.z);

	pointSouth =  normalAux*radius + normalAux * final_perlin;
	// pointSouth =  modelPosition.xyz + normalAux*radius + normalAux * n; XICO
	

	vec3 pointEast = translateLLtoCoords(pointLL.x,pointLL.y+offset,1);
	if(pointLL.x == 0 )
	{
		pointEast = translateLLtoCoords(offset,radians(90),1);
		
	}
	// pointEast = (M * vec4(pointEast,1)).xyz; XICO 
	//n = perlinNoise_snoise(pointEast);
	//perlinN = perlinNoise_3DRigidMultiFractal(pointEast.x,pointEast.y,pointEast.z);
	//perlinN2 = perlinNoise_3DRigid(pointEast.x,pointEast.y,pointEast.z);
	//perlinN3 = perlinNoise_3DMultiFractal(pointEast.x,pointEast.y,pointEast.z);
	//perlinN4 =perlinNoise_3D(pointEast.x,pointEast.y,pointEast.z);
	final_perlin = 0;
	if(simplexNoise)
		final_perlin = perlinNoise_snoise(pointEast);
	else if(rigidMultiFractal)
			final_perlin = perlinNoise_3DRigidMultiFractal(pointEast.x,pointEast.y,pointEast.z);
	else if(rigid)
			final_perlin = perlinNoise_3DRigid(pointEast.x,pointEast.y,pointEast.z);
	else if(multiFractal)
			final_perlin = perlinNoise_3DMultiFractal(pointEast.x,pointEast.y,pointEast.z);
	else if(perlinNoise)
			final_perlin = perlinNoise_3D(pointEast.x,pointEast.y,pointEast.z);

	normalAux = pointEast - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);
	pointEast =  normalAux*radius + normalAux * final_perlin;
	// pointEast =  modelPosition.xyz + normalAux*radius + normalAux * n; XICO
	

	vec3 pointWest = translateLLtoCoords(pointLL.x,pointLL.y-offset,1);
	if(pointLL.x == 0 )
	{
		pointWest = translateLLtoCoords(-offset,radians(90),1);		
	}

	// pointWest = (M * vec4(pointWest,1)).xyz; XICO 
	//n = perlinNoise_snoise(pointWest);
	//perlinN = perlinNoise_3DRigidMultiFractal(pointWest.x,pointWest.y,pointWest.z);
	//perlinN2 = perlinNoise_3DRigid(pointWest.x,pointWest.y,pointWest.z);
	//perlinN3 = perlinNoise_3DMultiFractal(pointWest.x,pointWest.y,pointWest.z);
	//perlinN4 =perlinNoise_3D(pointEast.x,pointWest.y,pointWest.z);
	final_perlin = 0;
	if(simplexNoise)
		final_perlin = perlinNoise_snoise(pointWest);
	else if(rigidMultiFractal)
			final_perlin = perlinNoise_3DRigidMultiFractal(pointWest.x,pointWest.y,pointWest.z);
	else if(rigid)
			final_perlin = perlinNoise_3DRigid(pointWest.x,pointWest.y,pointWest.z);
	else if(multiFractal)
			final_perlin = perlinNoise_3DMultiFractal(pointWest.x,pointWest.y,pointWest.z);
	else if(perlinNoise)
			final_perlin = perlinNoise_3D(pointWest.x,pointWest.y,pointWest.z);

	normalAux = pointWest - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);
	pointWest =  normalAux*radius + normalAux * final_perlin;
	// pointWest =  modelPosition.xyz + normalAux*radius + normalAux * n; XICO
	
	
	//tePosition = translateLLtoCoords(pointLL.x,pointLL.y,1);
	//------------------------------------------------------------------------------------------------//


	//vec3 normal = tePosition - (M*vec4(0,0,0,1)).xyz;
	vec3 normal = tePosition - vec4(0,0,0,1).xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;
	
	//n = perlinNoise_snoise(tePosition);
	//perlinN = perlinNoise_3DRigidMultiFractal(pointWest.x,pointWest.y,pointWest.z);
	//perlinN2 = perlinNoise_3DRigid(pointWest.x,pointWest.y,pointWest.z);
	//perlinN3 = perlinNoise_3DMultiFractal(pointWest.x,pointWest.y,pointWest.z);
	//perlinN4 =perlinNoise_3D(pointEast.x,pointWest.y,pointWest.z);
	final_perlin = 0;
	if(simplexNoise)
		final_perlin = perlinNoise_snoise(tePosition);
	else if(rigidMultiFractal)
			final_perlin = perlinNoise_3DRigidMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	else if(rigid)
			final_perlin = perlinNoise_3DRigid(tePosition.x,tePosition.y,tePosition.z);
	else if(multiFractal)
			final_perlin = perlinNoise_3DMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	else if(perlinNoise)
			final_perlin = perlinNoise_3D(tePosition.x,tePosition.y,tePosition.z);
  
	 
	// tePosition = modelPosition.xyz + normal*radius + normal * n; XICO
	tePosition = normal*radius + normal * final_perlin;
	//tePosition += VertexOut.normal * abs(perlinN);
	
	vec3 vector1 = pointNorh - pointSouth;
	vec3 vector2 = pointWest - pointEast;
	
	//vec3 vector1 = pointNorh - tePosition;
	//vec3 vector2 = pointWest - tePosition;

	

	vec3 c = cross(vector1, vector2);

	if(dot(VertexOut.normal,c) < 0 )
	{
		c = cross(vector2,vector1);		
	}

	if(normalize(tePosition).x > -0.001 &&
	normalize(tePosition).x < 0.001  && 
	normalize(tePosition).y > -0.001 &&
	normalize(tePosition).y < 0.001  && normalize(tePosition).z > 0)
	{
		c = vec3(0,0,1);	
	}
	else if(normalize(tePosition).x > -0.001 &&
	normalize(tePosition).x < 0.001  && 
	normalize(tePosition).y > -0.001 &&
	normalize(tePosition).y < 0.001  && 
	normalize(tePosition).z < 0)
	{
		c = vec3(0,0,-1);	
	}
		
	
	VertexOut.normal = normalize(c);

	
	VertexOut.noise = final_perlin;
	gl_Position = vec4(tePosition, 1);


	
}



