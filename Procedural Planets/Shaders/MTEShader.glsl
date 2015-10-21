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
const float radiusMoon = 5.0;

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
  return  42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) ) ;
  }



float perlinNoise_snoise(vec3 point, int octvs)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octvs-1;
	float k = 1.0;

	for(int i = 0; i < n ; i++)
	{

		float frequency = pow(2,i);
		float amplitude = 1/pow(2,i);

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

float perlinNoise_snoiseFractal(vec3 point, int octvs)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octvs-1;
	float k = 0.01;

	for(int i = 0; i < n ; i++)
	{

		float frequency = 4*pow(2,i);
		float amplitude = 2/pow(2,i);

		float add = amplitude * (snoise(point / frequency));
		
		if( i > 1)
		{
			add *= k * total;
		}	

		total += add;



	}
	return total;
}
 //***************************************************************************************************//

 float turbulence(vec3 point, float size)
{
    float value = 0.0, initialSize = size;
    
    while(size >= 1)
    {
        value += snoise(point / size) * size;
        size /= 2.0;
    }
    
    return(128.0 * value / initialSize);
}
 //***************************************************************************************************//

void main(void)
{
	vec4 modelPosition = vec4(0,0,0,1);
	
	//vec3 eyePosition  = (-V*vec4(0,0,0,1)).xyz;




	vec3 tePosition = interpolate3Dquads( tcPosition[0], tcPosition[1], tcPosition[2], tcPosition[3]);
	gl_Position.xyz = interpolate3Dquads( gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);


	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );

	float x2 =  gl_Position.x * gl_Position.x;
	float y2 =  gl_Position.y * gl_Position.y;
	float z2 =  gl_Position.z * gl_Position.z;	

	tePosition.x =  gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	tePosition.y =  gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	tePosition.z =  gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));

	vec2 pointLL = translateCoordsToLL(tePosition.x,tePosition.y,tePosition.z);

	float xPeriod = 10.0; //defines repetition of marble lines in x direction
    float yPeriod = 10.0; //defines repetition of marble lines in y direction
	float zPeriod = 10.0; //defines repetition of marble lines in z direction

	
	float turbPower = 5.0; //makes twists
    float turbSize = 3.0; //initial size of the turbulence

	 float xyzValue = 0;
     float sineValue = 0;

	//------------------------- AUXILIAR POINT FOR NORMAL --------------------------------------------//
	float offset = radians(1.0);
	vec3 pointNorh = translateLLtoCoords(pointLL.x+offset,pointLL.y,1);

	vec3 normalAux = pointNorh - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);

	xyzValue = pointNorh.x * xPeriod / (1024.0  )+ pointNorh.y * yPeriod / (768.0) + pointNorh.z * zPeriod / (1024.0*768.0) + turbPower * turbulence(pointNorh,turbSize) /256.0;
    sineValue = (sin(xyzValue * 3.14159) * -cos(xyzValue * 3.14159) );


	pointNorh = normalAux*radiusMoon + normalAux * (sineValue*0.1+0.1);


	vec3 pointSouth = translateLLtoCoords(pointLL.x-offset,pointLL.y,1);
	normalAux = pointSouth - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);

	xyzValue = pointSouth.x * xPeriod / (1024.0  )+ pointSouth.y * yPeriod / (768.0) + pointSouth.z * zPeriod / (1024.0*768.0) + turbPower * turbulence(pointSouth,turbSize) /256.0;
    sineValue = (sin(xyzValue * 3.14159) * -cos(xyzValue * 3.14159) );

	pointSouth = normalAux*radiusMoon + normalAux * (sineValue*0.1+0.1);
	

	vec3 pointEast = translateLLtoCoords(pointLL.x,pointLL.y+offset,1);
	if(pointLL.x == 0 )
	{
		pointEast = translateLLtoCoords(offset,radians(90),1);
		
	}

	normalAux = pointEast - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);

	xyzValue = pointEast.x * xPeriod / (1024.0  )+ pointEast.y * yPeriod / (768.0) + pointEast.z * zPeriod / (1024.0*768.0) + turbPower * turbulence(pointEast,turbSize) /256.0;
    sineValue = (sin(xyzValue * 3.14159) * -cos(xyzValue * 3.14159) );

	pointEast = normalAux*radiusMoon + normalAux * (sineValue*0.1+0.1);
	

	vec3 pointWest = translateLLtoCoords(pointLL.x,pointLL.y-offset,1);
	if(pointLL.x == 0 )
	{
		pointWest = translateLLtoCoords(-offset,radians(90),1);		
	}


	normalAux = pointWest - vec4(0,0,0,1).xyz;
	normalAux = normalize(normalAux);

	xyzValue = pointWest.x * xPeriod / (1024.0  )+ pointWest.y * yPeriod / (768.0) + pointWest.z * zPeriod / (1024.0*768.0) + turbPower * turbulence(pointWest,turbSize) /256.0;
    sineValue = (sin(xyzValue * 3.14159) * -cos(xyzValue * 3.14159) );
	
	pointWest = normalAux*radiusMoon + normalAux * (sineValue*0.1+0.1);
	
	
	//------------------------------------------------------------------------------------------------//



	vec3 normal = tePosition - vec4(0,0,0,1).xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;

	xyzValue = tePosition.x * xPeriod / (1024.0  )+ tePosition.y * yPeriod / (768.0) + tePosition.z * zPeriod / (1024.0*768.0) + turbPower * turbulence(tePosition,turbSize) /256.0;
    sineValue = (sin(xyzValue * 3.14159) * -cos(xyzValue * 3.14159) );

	//float perlinN = perlinNoise_3DRigidMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	tePosition = normal*(radiusMoon) + normal * (sineValue*0.1+0.1);

	vec3 vector1 = pointNorh - pointSouth;
	vec3 vector2 = pointWest - pointEast;

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

	gl_Position = vec4(tePosition, 1);
}



