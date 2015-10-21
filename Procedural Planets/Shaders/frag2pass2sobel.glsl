#version 430 core
layout(binding = 0)  uniform sampler2D renderedTexture;
//layout(location = 1) in vec2 UV;
layout(location = 1) in vec2 uv_out;


//*********** IN VARIABLES **********************************//


//**********************************************************//

//*********** OUT VARIABLES **********************************//
out vec3 color;
//**********************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 V;
//***************************************************************//

 
void main(){

//gl_fragcoord
	vec4 pos_height = texture( renderedTexture, uv_out);
	vec3 position = pos_height.xyz;
	float height = pos_height.w;

	

	vec3 water_color =  vec3(0.109,0.425,0.933);
	vec3 water2_color = vec3(0.109,0.525,0.933);
	vec3 grass_color =  vec3(0.419,0.557,0.138);
	vec3 sand_color =   vec3(0.745,0.745,0.745);
	vec3 rock_color =   vec3(0.65,0.65,0.65);
	vec3 rock_color2 =   vec3(0.45,0.55,0.55);
	vec3 rock_color3 =   vec3(0.45,0.45,0.45);
	vec3 snow_color =     vec3(0.9,0.9,0.9);



	vec3 fragmentColor = mix(water_color,water2_color,smoothstep(-1.0,-0.5,height));
	fragmentColor = mix(fragmentColor,water2_color,smoothstep(-0.5,-0.1,height));
	fragmentColor      = mix(fragmentColor,sand_color,smoothstep(-0.1,0.0,height));
	fragmentColor      = mix(fragmentColor,rock_color,smoothstep(-0.05,0.1,height));
	fragmentColor      = mix(fragmentColor,grass_color,smoothstep(0.0,0.2,height));
	fragmentColor      = mix(fragmentColor,rock_color,smoothstep(0.19,0.3,height));
	fragmentColor      = mix(fragmentColor,rock_color2,smoothstep(0.1,0.4,height));
	fragmentColor      = mix(fragmentColor,rock_color3,smoothstep(0.05,0.6,height));
	fragmentColor      = mix(fragmentColor,snow_color,smoothstep(0.9,1.0,height));

	//fragmentColor = height > 1.5 ? vec3(1,0,0) : vec3(1,1,1);

	float constantAttenuation = 0.7;
	float linearAttenuation = 0.0007;
	float quadraticAttenuation = 0.0000014;

	vec3 ambientlight = vec3(0.07,0.07,0.07);

	vec3 lightPos = vec3(0,0,20);
	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = lightPos - position ;
	float dist = length(lightDir);
	float distIntensity = 1.0 /dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,dist,dist*dist) );

	
	float inc = 8.0;
	
	float upperLeft =		texture( renderedTexture, (position  + vec3(-1.0, 1.0, 1.0)).xy	).w ;
	float upperCenter =		texture( renderedTexture, (position  + vec3(0.0, 1.0, 1.0)).xy	).w ;
	float upperRight =		texture( renderedTexture, (position  + vec3(1.0, 1.0, 1.0)).xy	).w ;

	float left =			texture( renderedTexture, (position  + vec3(-1.0, 0.0, 1.0)).xy  ).w ;
	float right =			texture( renderedTexture, (position  + vec3(1.0, 0.0, 1.0)).xy   ).w ;

	float lowerLeft =		texture( renderedTexture, (position  + vec3(-1.0, -1.0, 1.0)).xy ).w ;
	float lowerCenter =		texture( renderedTexture, (position  + vec3(0.0, -1.0, 1.0)).xy  ).w ;
	float lowerRight =		texture( renderedTexture, (position  + vec3(1.0, -1.0, 1.0)).xy  ).w ;

	float x = upperRight +(2.0 * right) + lowerRight - upperLeft - (2.0 * left) - lowerLeft;
	float y = lowerLeft +(2.0 * lowerCenter) + lowerRight - upperLeft - (2.0 * upperCenter) - upperRight;
	vec3 new_normal_coord =  vec3(-x, y, 1.0) ;

	if(length(position) <= 0)
		discard;





	vec3 normal = new_normal_coord;


	vec3 N = normalize(normal); 
	vec3 L = normalize(lightDir);
	vec3 m_v = normalize(V*vec4(position,1)).xyz;
	vec3 R = reflect(m_v,N);

	//float lightIntensity = clamp( dot(FragmentIn.normal, L), 5.0, 20.0  );
	//float diffuseIntensity = clamp(dot(N,L),0,1);
	float diffuseIntensity = max(dot(L,N),0);
	float specular =  pow(max(dot(R, L), 0.0), 1.0);
	//
	color = (/*ambientlight  + specular + */lightColor * diffuseIntensity * distIntensity) * fragmentColor;//;


	color = N;//*0.5+0.5;
	//color = new_normal_coord;//*0.5+0.5;
	//color = vec3(height,height,height);
	//color = position;
	//color = vec3(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);
				
}