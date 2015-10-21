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

float CubivInterpolation(
   float y0,float y1,
   float y2,float y3,
   float mu)
{
   float a0,a1,a2,a3,mu2;

   mu2 = mu*mu;
   a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;

   return(a0*mu*mu2+a1*mu2+a2*mu+a3);
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

	float v1 = floatNoise(val_x, val_y, val_z);
	float v2 = floatNoise(val_x+1, val_y, val_z);
	float v3 = floatNoise(val_x, val_y+1, val_z);
	float v4 = floatNoise(val_x+1, val_y+1, val_z);
	float v5 = floatNoise(val_x, val_y, val_z+1);
	float v6 = floatNoise(val_x+1, val_y, val_z+1);
	float v7 = floatNoise(val_x, val_y+1, val_z+1);
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

		//float add = InterpolatedNoise(x * frequency, y * frequency , z * frequency);

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
	float k = 1.0;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);
		
		float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency) * amplitude;


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
		//float frequency = initfrq/pow(2,i);

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
//************************************************************************************************************//



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


		total += add ;



		//float n = abs(snoise(vPosition));
		//n += 0.5 * abs(snoise(vPosition * 2.0));
		//n += 0.25 * abs(snoise(vPosition * 4.0));
		//n += 0.125 * abs(snoise(vPosition * 8.0));
		//n += 0.0625 * abs(snoise(vPosition * 16.0));
		//n += 0.03125 * abs(snoise(vPosition * 32.0));
		//n += 0.015625 * abs(snoise(vPosition * 64.0))

	}
	return total;
}
 //***************************************************************************************************//

void main(){

	

	vec4 modelPosition = vec4(0,0,0,1);

	// Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
	//gl_Position = M * v;
	gl_Position = v;
	vPosition = v.xyz;
	
	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;

	
	vPosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	vPosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	vPosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));

	

	vec3 normal = vPosition - modelPosition.xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;

	// vPosition = (M * vec4(vPosition,1)).xyz;// XICO

	float final_perlin = 0;
	if(simplexNoise)
		final_perlin = perlinNoise_snoise(vPosition);
	else if(rigidMultiFractal)
			final_perlin = perlinNoise_3DRigidMultiFractal(vPosition.x,vPosition.y,vPosition.z);
	else if(rigid)
			final_perlin = perlinNoise_3DRigid(vPosition.x,vPosition.y,vPosition.z);
	else if(multiFractal)
			final_perlin = perlinNoise_3DMultiFractal(vPosition.x,vPosition.y,vPosition.z);
	else if(perlinNoise)
			final_perlin = perlinNoise_3D(vPosition.x,vPosition.y,vPosition.z);

	vPosition = normal*radius + normal * final_perlin;
	//vPosition = modelPosition + normal*radius + normal * n; // XICO

	//if( vPosition.z < 0 )
	//{
	//	vPosition = vec3(0,0,0);
	//	gl_Position = vec4(0,0,0,0);
	//}

	// The color of each vertex will be interpolated
    // to produce the color of each fragment
    VertexOut.fragmentColor = vertexColor;
	VertexOut.texCoord = vertexUv;
	//VertexOut.normal = vertexNormal;

 }