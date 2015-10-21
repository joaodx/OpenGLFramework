#version 430 core


//*********** IN VARIABLES **********************************//
// Interpolated values from the vertex shaders
in VertexData {
    smooth vec3 position;
    vec2 texCoord;
    vec3 normal;
	smooth float noise;
} FragmentIn;

//**********************************************************//

//*********** OUT VARIABLES **********************************//
out vec4 pos_height;
//**********************************************************//

//*********** UNIFORM VARIABLES **********************************//

//***************************************************************//

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

//ftp://gaia.ecs.csus.edu/usr/clevengr/155/old/fall11/LectureNotes/Fall11/14ProceduralTexturingLectureNotes.pdf
//***************************************************************************************************//
const vec3 skyColor = vec3(0.0, 0.8, 0.0); //blue sky
const vec3 cloudColor = vec3(0.8, 0.8, 0.8); //white clouds

float rand(vec3 co){
return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
 //***************************************************************************************************//

 //http://lodev.org/cgtutor/randomnoise.html
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

void main(){

	vec3 ambientlight = vec3(0.09,0.09,0.09);
	float constantAttenuation = 0.7;
	float linearAttenuation = 0.00000007;
	float quadraticAttenuation = 0.00000000000014;

	vec3 water_color =  vec3(0.109,0.425,0.933);
	vec3 water2_color = vec3(0.109,0.525,0.933);
	vec3 grass_color =  vec3(0.419,0.557,0.138);
	vec3 sand_color =   vec3(0.745,0.745,0.745);
	vec3 rock_color =   vec3(0.65,0.65,0.65);
	vec3 rock_color2 =   vec3(0.45,0.55,0.55);
	vec3 rock_color3 =   vec3(0.45,0.45,0.45);
	vec3 snow_color =     vec3(0.9,0.9,0.9);



	//float mixFactor = rand(FragmentIn.position);
	////use the intensity to mix sky and cloud colors, modulated by the light intensity
	//vec4 proceduralGrass = vec4( mix(grass_color, sand_color, mixFactor) * (ambientlight).xyz, 1) ; 
	//vec4 proceduralwater = vec4( mix(water_color, water2_color, mixFactor) * (ambientlight).xyz, 1) ; 
	//vec4 proceduralSand = vec4( mix(sand_color, snow_color, mixFactor) * (ambientlight).xyz, 1) ; 
	//vec4 proceduralRock = vec4( mix(rock_color, rock_color2, mixFactor) * (ambientlight).xyz, 1) ; 
	//vec4 proceduralRock2 = vec4( mix(rock_color2, rock_color3, mixFactor) * (ambientlight).xyz, 1) ; 
	//vec4 proceduralSnow = vec4( mix(snow_color, rock_color3, mixFactor) * (ambientlight).xyz, 1) ; 


	float n = abs(perlinNoise_snoise(FragmentIn.position,3));
	float n3 =  abs(perlinNoise_snoiseFractal(FragmentIn.position,9));

		
	float xPeriod = 1.0; //defines repetition of marble lines in x direction
    float yPeriod = 5.0; //defines repetition of marble lines in y direction
	float zPeriod = 10.0; //defines repetition of marble lines in z direction

	
	float turbPower = 3.0; //makes twists
    float turbSize = 200.0; //initial size of the turbulence

	 float xyzValue = FragmentIn.position.x * xPeriod / (1024.0*768.0  )+ FragmentIn.position.y * yPeriod / (1024.0*768.0) + FragmentIn.position.z * zPeriod / (1024.0*768.0) + turbPower * turbulence(FragmentIn.position,turbSize) /256.0;
     float sineValue = abs(sin(xyzValue * 3.14159) * -cos(xyzValue * 3.14159) );

	

  
	 vec4 proceduralSun = vec4(vec3(1.5-(n*0.1+0.1), 1.0-(n*0.1+0.1), 0.5-(n*0.1+0.1)), 1.0);
	 vec4 proceduralGrass = vec4(vec3(0.419-(n3*0.1+0.1), 0.557-(n3*0.1+0.1), 0.138-(n3*0.1+0.1)), 1.0);
	 vec4 proceduralSand = vec4(vec3(1.0-(sineValue*0.1+0.1), 0.9-(sineValue*0.1+0.1), 0.4-sineValue), 1.0);
	 vec4 proceduralWater = vec4(vec3(0.109-(n*0.1+0.1), 0.425-(n*0.1+0.1), 0.933), 1.0);
	 vec4 proceduralWater2 = vec4(vec3(0.109-(n*0.1+0.1), 0.525-(n*0.1+0.1), 0.933), 1.0);
	 vec4 proceduralRock = vec4(vec3(0.65-(n*0.1+0.1), 0.65-(n*0.1+0.1), 0.65-(n*0.1+0.1)), 1.0);
	 vec4 proceduralRock2 = vec4(vec3(0.45+(sineValue*0.001+0.01), 0.55+(sineValue*0.001+0.01), 0.55+(sineValue*0.001+0.01)), 1.0);
	 vec4 proceduralRock3 = vec4(vec3(0.45-(n*0.1+0.1), 0.45-(n*0.1+0.1), 0.45-(n*0.1+0.1)), 1.0);
	 vec4 proceduralSnow = vec4(vec3(0.8-(n3*0.1+0.1), 0.8-(n3*0.1+0.1), 0.8-(n3*0.1+0.1)), 1.0);

	vec3 fragmentColor = mix(proceduralWater.xyz,proceduralWater2.xyz,smoothstep(-1.0,-0.5,FragmentIn.noise));
	fragmentColor = mix(fragmentColor,water2_color.xyz,smoothstep(-0.5,-0.1,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,proceduralSand.xyz,smoothstep(-0.1,0.1,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,proceduralGrass.xyz,smoothstep(0.1,0.5,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,proceduralRock.xyz,smoothstep(0.6,0.7,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,proceduralRock2.xyz,smoothstep(0.7,0.9,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,proceduralRock3.xyz,smoothstep(0.6,1.8,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,proceduralSnow.xyz,smoothstep(1.2,2.0,FragmentIn.noise));

	//fragmentColor = proceduralGrass.xyz;
	//fragmentColor = FragmentIn.noise > 1.5 ? vec3(1,0,0) : vec3(1,1,1);



	

	vec3 lightPos = vec3(0,2000,0);
	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = lightPos - FragmentIn.position ;
	float dist = length(lightDir);
	//float distIntensity = 1.0 / constantAttenuation + linearAttenuation *dist +  quadraticAttenuation *dist*dist;
	//float distIntensity = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));
	float distIntensity = 1.0 /dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,dist,dist*dist) );

	
	//color = FragmentIn.fragmentColor * lightIntensity;
	
	vec3 N = normalize(FragmentIn.normal);
	vec3 L = normalize(lightDir);

	//float lightIntensity = clamp( dot(FragmentIn.normal, L), 5.0, 20.0  );
	//float diffuseIntensity = clamp(dot(N,L),0,1);
	float diffuseIntensity = max(dot(L,N),0);
	//pos_height.xyz = (ambientlight  + lightColor * diffuseIntensity * distIntensity) * fragmentColor;
	pos_height.xyz = (ambientlight  + lightColor * diffuseIntensity * distIntensity) * fragmentColor;
	
	//pos_height = vec4(FragmentIn.position,FragmentIn.noise*0.5+0.5);
	//pos_height = vec4(FragmentIn.position,FragmentIn.noise);
	//pos_height = vec4(1,1,1,1)*(FragmentIn.noise*0.5+0.5);//pos_height = vec4(1,1,1,1)*FragmentIn.noise;
	//pos_height = vec4(N.xyz,FragmentIn.noise);
	//pos_height = FragmentIn.fragmentColor;
	//pos_height = vec4(1,0,0,1);

	//float n = snoise(vec3(pos_height.x,pos_height.y,pos_height.z));
	//pos_height = vec4(0.5*n.xxx + 0.5, 1.0);

	//float n = snoise(vec3(5.0*pos_height.st,50.0));
	//  n += 0.5*snoise(vec3(10.0*pos_height.st,60.0));
	//  n += 0.25*snoise(vec3(20.0*pos_height.st,70.0));
	//  n += 0.125*snoise(vec3(40.0*pos_height.st,80.0));
	//  n += 0.0625*snoise(vec3(80.0*pos_height.st,90.0));
	//  pos_height = vec4(0.4*vec3(n) + 0.5, 1.0);

	 // vec2 F = cellular(pos_height.st*10.0);
  //float distmap = F.x;
  //float pattern = distmap;
  //pos_height = vec4(vec3(pattern), 1.0);


	 //generate a random value based on the fragment position
	//vec4 noiseVec = noise4(FragmentIn.position); //noiseX() not implemented!
	//sum the 4 octaves of noise obtaining a mixing coefficient
	//float mixFactor = (noiseVec[0] + noiseVec[1] + noiseVec[2] + noiseVec[3] + 0.03125 );
	//float mixFactor = rand(FragmentIn.position);
	//use the intensity to mix sky and cloud colors, modulated by the light intensity
	//pos_height = vec4( mix(skyColor, cloudColor, mixFactor) * (ambientlight  + lightColor * diffuseIntensity * distIntensity).xyz, 1) ; 
	
	/*
	if(FragmentIn.position.z < 0)
		pos_height = vec4(1,0,0,1);
	else if(FragmentIn.position.z > 0)
		pos_height = vec4(0,0,1,1);
	*/
}