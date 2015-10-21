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
layout(location = 0) out vec4 pos_height;
layout(location = 1) out vec4 nrm_pad;
//**********************************************************//

//*********** UNIFORM VARIABLES **********************************//

//***************************************************************//

 
void main(){

	vec3 water_color =  vec3(0.109,0.425,0.933);
	vec3 water2_color = vec3(0.109,0.525,0.933);
	vec3 grass_color =  vec3(0.419,0.557,0.138);
	vec3 sand_color =   vec3(0.745,0.745,0.745);
	vec3 rock_color =   vec3(0.65,0.65,0.65);
	vec3 rock_color2 =   vec3(0.45,0.55,0.55);
	vec3 rock_color3 =   vec3(0.45,0.45,0.45);
	vec3 snow_color =     vec3(0.9,0.9,0.9);



	vec3 fragmentColor = mix(water_color,water2_color,smoothstep(-1.0,-0.5,FragmentIn.noise));
	fragmentColor = mix(fragmentColor,water2_color,smoothstep(-0.5,-0.1,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,sand_color,smoothstep(-0.1,0.0,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,rock_color,smoothstep(-0.05,0.1,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,grass_color,smoothstep(0.0,0.2,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,rock_color,smoothstep(0.19,0.3,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,rock_color2,smoothstep(0.1,0.4,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,rock_color3,smoothstep(0.05,0.6,FragmentIn.noise));
	fragmentColor      = mix(fragmentColor,snow_color,smoothstep(0.9,1.0,FragmentIn.noise));

	//fragmentColor = FragmentIn.noise > 1.5 ? vec3(1,0,0) : vec3(1,1,1);

	float constantAttenuation = 0.7;
	float linearAttenuation = 0.0007;
	float quadraticAttenuation = 0.0000014;

	vec3 ambientlight = vec3(0.07,0.07,0.07);

	vec3 lightPos = vec3(0,50,0);
	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = lightPos - FragmentIn.position ;
	float dist = length(lightDir);
	//float distIntensity = 1.0 / constantAttenuation + linearAttenuation *dist +  quadraticAttenuation *dist*dist;
	//float distIntensity = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));
	float distIntensity = 1.0 /dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,dist,dist*dist) );

	
	//color = FragmentIn.fragmentColor * lightIntensity;
	
	vec3 N = normalize(FragmentIn.normal);
	//vec3 N = normalize(-cross(dFdy(FragmentIn.position),dFdx(FragmentIn.position)));
	vec3 L = normalize(lightDir);

	//float lightIntensity = clamp( dot(FragmentIn.normal, L), 5.0, 20.0  );
	//float diffuseIntensity = clamp(dot(N,L),0,1);
	float diffuseIntensity = max(dot(L,N),0);
	//pos_height.xyz = (/*ambientlight  + */lightColor * diffuseIntensity * distIntensity) * fragmentColor;
	
	nrm_pad = vec4(FragmentIn.normal.xyz,1.0);
	pos_height = vec4(FragmentIn.position,FragmentIn.noise*0.5+0.5);
	
	//pos_height = vec4(FragmentIn.position,FragmentIn.noise);
	//pos_height = vec4(1,1,1,1)*(FragmentIn.noise*0.5+0.5);//pos_height = vec4(1,1,1,1)*FragmentIn.noise;
	//pos_height = vec4(N,1.0);
	//color = FragmentIn.fragmentColor;
	//color = vec3(1,0,0);
	
}