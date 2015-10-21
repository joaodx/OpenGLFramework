#version 430 core


//*********** IN VARIABLES **********************************//
// Interpolated values from the vertex shaders
in VertexData {
    smooth vec3 position;
    vec2 texCoord;
    vec3 normal;
	vec3 scattering;
	vec3 extinction;	
	vec3 light;
} FragmentIn;

//**********************************************************//

//*********** OUT VARIABLES **********************************//
out vec4 pos_height;
//**********************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 V;
//***************************************************************//

const vec3 Br = vec3(6.95e-6, 1.18e-5, 2.44e-5);
const vec3 Bm = vec3(4e-7, 6e-7, 2.4e-6);
const vec3 Brm = Br+Bm;
const vec4 Esun = vec4(1.0, 1.0, 1.0, 15.0);
const float g = -0.91f;
const float g2 = g *g;
float log2_e = 1.0/log(2.0);
const float e = 2.71828;
const float PI = 3.141592654;
const float fExposure = 4.0;

float GetRayleighPhase( float fCos2 )
{
	return 0.75 * (1.0 + fCos2);
}

float GetMiePhase( float fCos, float fCos2 )
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}
 

 // Mie phase function
float getMiePhase2(float fCos, float fCos2, float g, float g2)
{
   return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);   
}

// Rayleigh phase function
float getRayleighPhase2(float fCos2)
{
   //return 0.75 + 0.75 * fCos2;
   return 0.75 * (2.0 + 0.5 * fCos2);
   
}

void main(){




	float constantAttenuation = 2;
	float linearAttenuation = 0.00000007;
	float quadraticAttenuation = 0.000000000014;

	vec3 ambientlight = vec3(0.0,0.0,0.2);

	vec3 lightPos = vec3(0,200,0);
	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = lightPos - FragmentIn.position ;
	float dist = length(lightDir);
	//float distIntensity = 1.0 / constantAttenuation + linearAttenuation *dist +  quadraticAttenuation *dist*dist;
	//float distIntensity = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));
	float distIntensity = 1.0 /dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,dist,dist*dist) );

	

	
	vec3 N = normalize(FragmentIn.normal);
	vec3 L = normalize(lightDir);
	vec3 m_v = normalize(V*vec4(FragmentIn.position,1)).xyz;
	vec3 R = reflect(m_v,N);

	//float lightIntensity = clamp( dot(FragmentIn.normal, L), 5.0, 20.0  );
	//float diffuseIntensity = clamp(dot(N,L),0,1);
	float diffuseIntensity = max(dot(L,N),0);
	float specular =  pow(max(dot(R, L), 0.3), 0.5);
	pos_height.xyz = (ambientlight + (specular * diffuseIntensity * distIntensity)  + lightColor  * diffuseIntensity * distIntensity );
	pos_height.a = 0.3;

	//pos_height = vec4(FragmentIn.position,FragmentIn.noise*0.5+0.5);
	//pos_height = vec4(FragmentIn.position,FragmentIn.noise);
	//pos_height = vec4(1,1,1,1)*(FragmentIn.noise*0.5+0.5);//pos_height = vec4(1,1,1,1)*FragmentIn.noise;
	//pos_height = vec4(N.xyz,FragmentIn.noise);
	//pos_height = vec4(1,0,0,1);

	//float Idiff = dot(normalize(FragmentIn.normal), normalize(FragmentIn.light));
 //   vec4 surfaceColour = vec4(0.0, 0.0, 0.3, 1.0); //colourless atmosphere
 
 //   vec4 final = (surfaceColour*vec4(FragmentIn.extinction, 1.0)) + vec4(FragmentIn.scattering, 1.0);
	//pos_height = final;



	float fCos = dot(-lightDir, FragmentIn.light) / length(FragmentIn.light);
	float fCos2 = fCos * fCos;
	float fRayleighPhase = getRayleighPhase2( fCos2  );
	float fMiePhase = getMiePhase2( fCos, fCos2, g, g2 );
	vec4 color = vec4(1-exp(-fExposure  * (fRayleighPhase * FragmentIn.scattering) + (fMiePhase * FragmentIn.extinction) ), 1.0f); 
	color.a = color.b;
	//pos_height =vec4(  getRayleighPhase2(fCos2) * FragmentIn.scattering + getMiePhase2(fCos, fCos2, g, g2) * FragmentIn.extinction , 1.0 );
	//pos_height.a = max(max(pos_height.r, pos_height.g), pos_height.b);
	//pos_height = 1.0 - exp(-0.1 * pos_height);
	pos_height = color;
	//pos_height = vec4(( FragmentIn.scattering +  vec3(0.25,0.25,0.25) * FragmentIn.extinction), 1.0);
	//pos_height = vec4( FragmentIn.scattering + 0.25 * FragmentIn.extinction,1);
	//pos_height = vec4(1,1,0,1);
}