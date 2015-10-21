#version 430 core

#define MAX_LIGHTS 4
//*********** IN VARIABLES **********************************//
struct LightSourceParameters
    {
        vec4 ambient;              // Aclarri   
        vec4 diffuse;              // Dcli   
        vec4 specular;             // Scli   
        vec3 position;             // Ppli   
    };
//***************************************************************//

//*********** IN VARIABLES **********************************//
// Interpolated values from the vertex shaders
in VertexData {
	vec3 position;
    vec2 texCoord;
    vec3 normal;
	vec3 fragmentColor;
} FragmentIn;

//**********************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform vec3 lights [MAX_LIGHTS];
uniform LightSourceParameters lightSource [MAX_LIGHTS];
uniform mat4 V;
//***************************************************************//

//*********** OUT VARIABLES **********************************//
layout(location = 0) out vec4 color;
//**********************************************************//

 
void main(){
	vec3 eye_position = inverse(V)[3].xyz;
	vec3 world_pos = FragmentIn.position;
	vec3 world_normal = FragmentIn.normal;
	float material_shininess = 4;
	vec4 sceneColor = vec4(0,0,0,0); 

	float material_kd = 0.5;
	float material_ks = 1.0 ;
	const float screenGamma = 0.8; // Assume the monitor is calibrated to the sRGB color space
	float constantAttenuation = 0;
	float linearAttenuation = 0;
	float quadraticAttenuation = 1;

	vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		vec4 ambientColour = lightSource[i].ambient; 
		vec4 diffuseColour = lightSource[i].diffuse; 
		vec4 specularColour = lightSource[i].specular; 

		vec3 L = normalize( lightSource[i].position - world_pos);//light direction
		float D = length(L);
		vec3 V = normalize( eye_position - world_pos);//view direction
		float distIntensity = 1.0 / dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,D,D*D) );


		float LdotN =  max(0, dot(L,world_normal));
		float diffuse = material_kd * LdotN;			
		float specular = 0.0;

		if(LdotN >= 0.0)
		{
	  	 
		  //choose H or R to see the difference
		  //vec3 R = -normalize(reflect(L, world_normal));//Reflection
		  //specular = material_ks * pow(max(0, dot(R, V)), material_shininess);
      
		  //Blinn-Phong
		  vec3 H = normalize(L + V );//Halfway
		  specular = material_ks * pow(max(0, dot(H, world_normal)), material_shininess);
    
		}

		//float light = diffuse + specular;	
		finalColor += ambientColour + diffuse * diffuseColour * distIntensity + specular * specularColour * distIntensity;
		//finalColor += vec4(light,light, light,1); 
	}
	
	vec4 colorGammaCorrected = pow(finalColor, vec4(1.0/screenGamma));
	color = colorGammaCorrected; 

	//color = vec4(lightSource[0].specular,1);
	//color = lightSource[0].ambient;
	//color = vec4(eye_position.xyz ,1);
	//color = FragmentIn.fragmentColor;
	//color = FragmentIn.normal;
	//color = lightSource[0].position;
	//color = vec4(1,0,0,1);
}