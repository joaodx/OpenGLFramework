#version 430 core
layout(binding = 0)  uniform sampler2D renderedTexture;
layout(binding = 1)  uniform sampler2D renderedTexture2;
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
	//Create a vector where it will be add the positions that can be use for getting the normals
	 vec3 auxArray [4];
	 int countElem = 0;

	//gl_fragcoord
	vec4 pos_height = texture( renderedTexture, uv_out);
	vec4 norm_pad = texture( renderedTexture2, uv_out);
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

	float scale = (height - 0.5)/0.5;

	vec3 fragmentColor = mix(water_color,water2_color,smoothstep(-1.0,-0.5,scale));
	fragmentColor = mix(fragmentColor,water2_color,smoothstep(-0.5,-0.1,scale));
	fragmentColor      = mix(fragmentColor,sand_color,smoothstep(-0.1,0.0,scale));
	fragmentColor      = mix(fragmentColor,rock_color,smoothstep(-0.05,0.1,scale));
	fragmentColor      = mix(fragmentColor,grass_color,smoothstep(0.0,0.2,scale));
	fragmentColor      = mix(fragmentColor,rock_color,smoothstep(0.19,0.3,scale));
	fragmentColor      = mix(fragmentColor,rock_color2,smoothstep(0.1,0.4,scale));
	fragmentColor      = mix(fragmentColor,rock_color3,smoothstep(0.05,0.6,scale));
	fragmentColor      = mix(fragmentColor,snow_color,smoothstep(0.9,1.0,scale));

	//fragmentColor = height > 1.5 ? vec3(1,0,0) : vec3(1,1,1);

	float constantAttenuation = 0.7;
	float linearAttenuation = 0.0007;
	float quadraticAttenuation = 0.0000014;

	vec3 ambientlight = vec3(0.07,0.07,0.07);

	vec3 lightPos = vec3(0,200,0);
	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = lightPos - position ;
	float dist = length(lightDir);
	//float distIntensity = 1.0 / constantAttenuation + linearAttenuation *dist +  quadraticAttenuation *dist*dist;
	//float distIntensity = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));
	float distIntensity = 1.0 /dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,dist,dist*dist) );

	
	//vec3 posN = texture( renderedTexture, uv_out+vec2(0.0,0.0001)).xyz;
	//vec3 posS = texture( renderedTexture, uv_out+vec2(0.0,-0.0001)).xyz;
	//vec3 posE = texture( renderedTexture, uv_out+vec2(0.0001,0.0)).xyz;
	//vec3 posW = texture( renderedTexture, uv_out+vec2(-0.0001,0.0)).xyz;

	float inc = 4.0;
	vec3 posN = texture( renderedTexture, vec2(gl_FragCoord.x/1024.0,(gl_FragCoord.y+inc)/768.0) ).xyz ;
	vec3 posS = texture( renderedTexture, vec2(gl_FragCoord.x/1024.0,(gl_FragCoord.y-inc)/768.0) ).xyz ;
	vec3 posE = texture( renderedTexture, vec2((gl_FragCoord.x+inc)/1024.0,(gl_FragCoord.y)/768.0) ).xyz  ;
	vec3 posW = texture( renderedTexture, vec2((gl_FragCoord.x-inc)/1024.0,(gl_FragCoord.y)/768.0) ).xyz  ;
	
	if(length(position) <= 0)
		discard;

	vec3 c;
	//float distAux =  distance(vec3(0,0,0),position);
	//if(distance(posN,position) == distAux)
	//	c = cross(position.xyz-posS.xyz,posE.xyz-posW.xyz);
	//else if(distance(posS,position) == distAux)
	//	c = cross(posN.xyz-position.xyz,posE.xyz-posW.xyz);
	//else if(distance(posE,position) == distAux)
	//	c = cross(posN.xyz-posS.xyz,position.xyz-posW.xyz);
	//else if(distance(posW,position) == distAux)
	//	c = cross(posN.xyz-posS.xyz,posE.xyz-position.xyz);
	//else
	
		//c = cross(posN.xyz-posS.xyz,posE.xyz-posW.xyz);
		if(distance(position,posE) <= 1.0)
		{
			//c = cross(posN.xyz-posS.xyz,posW.xyz-position.xyz);
			auxArray[countElem] = posE;
			countElem++;
		}
		if(distance(position,posW) <= 1.0)
		{
			auxArray[countElem] = posW;
			countElem++;
		}
		if(distance(position,posN) <= 1.0)
		{
			auxArray[countElem] = posN;
			countElem++;
		}
		if(distance(position,posS) <= 1.0)
		{
			auxArray[countElem] = posS;
			countElem++;
		}

		if(countElem == 4 )
		{
			c = cross(posS.xyz-posN.xyz,posW.xyz-posE.xyz);
		}
		else if(countElem == 3 )
		{
			c = cross(auxArray[0].xyz-auxArray[1].xyz,auxArray[0].xyz-auxArray[2].xyz);
		}
		else if(countElem == 2 )
		{
			c = cross(position.xyz-auxArray[0].xyz,position.xyz-auxArray[1].xyz);
		}
		else
		{
			 c = normalize(position);
		}



		//else if(distance(position,posW) <= 0.5)
		//	c = cross(posN.xyz-posS.xyz,posE.xyz-position.xyz);
		//else if(distance(position,posN) <= 0.5)
		//	c = cross(posS.xyz-position.xyz,posE.xyz-posW.xyz);
		//else if(distance(position,posS) <= 0.5)
		//	c = cross(posN.xyz-position.xyz,posW.xyz-posE.xyz);
		//else
		//	c = cross(posS.xyz-posN.xyz,posW.xyz-posE.xyz);


	vec3 normal = position;
	//vec3 c = cross(dFdy(position),dFdx(position));
	
	if(dot(c,normalize(normal)) < 0)
		c = -c;

	if(pos_height.w < -0.3)
		c = normal;

	vec3 N = normalize(c); 
	vec3 L = normalize(lightDir);
	vec3 m_v = normalize(V*vec4(position,1)).xyz;
	vec3 R = reflect(m_v,N);

	//float lightIntensity = clamp( dot(FragmentIn.normal, L), 5.0, 20.0  );
	//float diffuseIntensity = clamp(dot(N,L),0,1);
	float diffuseIntensity = max(dot(L,N),0);
	float specular =  pow(max(dot(R, L), 0.0), 1.0);
	color = (/*ambientlight  + specular + */lightColor * diffuseIntensity * distIntensity) * fragmentColor;
	//color = N;//*0.5+0.5;
	//color = vec3(height,height,height);
	//color = position;
	//color = vec3(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);
				
}