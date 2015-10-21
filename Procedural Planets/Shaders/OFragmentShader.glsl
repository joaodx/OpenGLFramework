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



void main(){




	float constantAttenuation = 2;
	float linearAttenuation = 0.00000007;
	float quadraticAttenuation = 0.000000000014;

	vec3 ambientlight = vec3(0.0,0.0,0.5);

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
	
	//pos_height.xyz = vec3(0.0,0.0,0.5);
	pos_height.a = 0.5;

	//pos_height = vec4(FragmentIn.position,FragmentIn.noise*0.5+0.5);
	//pos_height = vec4(FragmentIn.position,FragmentIn.noise);
	//pos_height = vec4(1,1,1,1)*(FragmentIn.noise*0.5+0.5);//pos_height = vec4(1,1,1,1)*FragmentIn.noise;
	//pos_height = vec4(N.xyz,FragmentIn.noise);
	//pos_height = vec4(1,0,0,1);

	//float Idiff = dot(normalize(FragmentIn.normal), normalize(FragmentIn.light));
 //   vec4 surfaceColour = vec4(0.0, 0.0, 0.3, 1.0); //colourless atmosphere
 
 //   vec4 final = (surfaceColour*vec4(FragmentIn.extinction, 1.0)) + vec4(FragmentIn.scattering, 1.0);
	//pos_height = final;

}