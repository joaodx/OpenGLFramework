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


//Scharr operator constants combined with luminance weights
const vec3 Sobel1 = vec3(0.2990, 0.5870, 0.1140) * vec3(3.0);
const vec3 Sobel2 = vec3(0.2990, 0.5870, 0.1140) * vec3(10.0);

//Luminance weights, scaled by the average of 3x3 normalised kernel weights (including zeros)
const vec3 Lum = vec3(0.2990, 0.5870, 0.1140) * vec3(0.355556);

//Blur level (mip map LOD bias)
const float Blur  = 0.5;
 
void main(){
//texcoord = uv 
//gl_fragcoord
	vec2 Coord[3], d;
   vec4 Texel[6];
   vec3 Normal;

	
	//3x3 kernel offsets
   d = vec2(dFdx(uv_out.s), dFdy(uv_out.t));
   Coord[0] = uv_out.st - d;
   Coord[1] = uv_out.st;
   Coord[2] = uv_out.st + d;

   //Sobel operator, U direction
   Texel[0] = texture2D(renderedTexture, vec2(Coord[2].s, Coord[0].t), Blur) - texture2D(renderedTexture, vec2(Coord[0].s, Coord[0].t), Blur);
   Texel[1] = texture2D(renderedTexture, vec2(Coord[2].s, Coord[1].t), Blur) - texture2D(renderedTexture, vec2(Coord[0].s, Coord[1].t), Blur);
   Texel[2] = texture2D(renderedTexture, vec2(Coord[2].s, Coord[2].t), Blur) - texture2D(renderedTexture, vec2(Coord[0].s, Coord[2].t), Blur);

   //Sobel operator, V direction
   Texel[3] = texture2D(renderedTexture, vec2(Coord[0].s, Coord[0].t), Blur) - texture2D(renderedTexture, vec2(Coord[0].s, Coord[2].t), Blur);
   Texel[4] = texture2D(renderedTexture, vec2(Coord[1].s, Coord[0].t), Blur) - texture2D(renderedTexture, vec2(Coord[1].s, Coord[2].t), Blur);
   Texel[5] = texture2D(renderedTexture, vec2(Coord[2].s, Coord[0].t), Blur) - texture2D(renderedTexture, vec2(Coord[2].s, Coord[2].t), Blur);

   //Compute luminance from each texel, apply kernel weights, and sum them all
   Normal.x  = dot(Texel[0].xyz, Sobel1);
   Normal.x += dot(Texel[1].xyz, Sobel2);
   Normal.x += dot(Texel[2].xyz, Sobel1);

   Normal.y  = dot(Texel[3].xyz, Sobel1);
   Normal.y += dot(Texel[4].xyz, Sobel2);
   Normal.y += dot(Texel[5].xyz, Sobel1);

   Normal.z = dot(texture2D(renderedTexture, Coord[1], Blur).xyz, Lum);

  // Normal.y = -Normal.y;

   color = vec4(vec3(0.5) + normalize(Normal) * 0.5, 1.0).xyz;

   
   vec4 pos_height = texture2D(renderedTexture, Coord[1], Blur);//texture( renderedTexture, uv_out);
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

	vec3 lightPos = vec3(0,20,0);
	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = lightPos - position ;
	float dist = length(lightDir);
	//float distIntensity = 1.0 / constantAttenuation + linearAttenuation *dist +  quadraticAttenuation *dist*dist;
	//float distIntensity = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));
	float distIntensity = 1.0 /dot(vec3(constantAttenuation,linearAttenuation,quadraticAttenuation), vec3(1.0,dist,dist*dist) );

	
	//vec3 posN = texture( renderedTexture, uv_out+vec2(0.0,0.001)).xyz;
	//vec3 posS = texture( renderedTexture, uv_out+vec2(0.0,-0.001)).xyz;
	//vec3 posE = texture( renderedTexture, uv_out+vec2(0.001,0.0)).xyz;
	//vec3 posW = texture( renderedTexture, uv_out+vec2(-0.001,0.0)).xyz;

	//vec3 c = cross(posN-posS,posE-posW);

	//vec3 normal = position;
	////vec3 c = cross(dFdy(position),dFdx(position));
	//if(dot(c,normalize(normal)) < 0)
	//	c = -c;

	
	vec3 N = normalize(Normal); 
	vec3 L = normalize(lightDir);
	vec3 m_v = normalize(V*vec4(position,1)).xyz;
	vec3 R = reflect(m_v,N);

	//float lightIntensity = clamp( dot(FragmentIn.normal, L), 5.0, 20.0  );
	//float diffuseIntensity = clamp(dot(N,L),0,1);
	float diffuseIntensity = max(dot(L,N),0);
	float specular =  pow(max(dot(R, L), 0.0), 1.0);
	//color = (/*ambientlight  + specular + */lightColor * diffuseIntensity * distIntensity) * fragmentColor;//;
	//color = vec3(height,height,height);
	//color = position;
	//color = vec3(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);



}