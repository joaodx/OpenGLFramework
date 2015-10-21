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


//******** Atmosphere Constants and functions *************************************************************//

const float radiusAtm = radius*1.2;
const vec3 vWaveLength = vec3( 0.650f, 0.570f, 0.475f );
const float Kr = 0.015f; // Kr is the Rayleigh scattering constant
const float Km = 0.000035f; //Km is the Mie scattering constant
const float ESun = 10.1f; // ESun is the brightness of the sun
const float fSkydomeRadius = (6.378e6f) + 200000;
//const vec3 lightDir = vec3(0, -1, 0);
const float fScaleDepth = 200.0;

const vec3 Br = vec3(6.95e-6, 1.18e-5, 2.44e-5);
const vec3 Bm = vec3(4e-7, 6e-7, 2.4e-6);
const vec3 Brm = Br+Bm;
const vec4 Esun = vec4(1.0, 1.0, 1.0, 15.0);
const float g = 0.0;
float log2_e = 1.0/log(2.0);
const float e = 2.71828;
const float PI = 3.141592654;

vec3 Fex(float s)
{
    vec3 ex = exp(Brm * -s);
    return ex;
}
 
vec3 rayleigh(float theta)
{
    float pi316 = 3.0/(16.0*PI);
 
    float phase = 1.0 + (theta*theta);
     
    vec3 ret = pi316*phase*Br;
    return ret;
}
 
vec3 mie(float theta)
{
    float pi14 = 1.0/(4.0*PI);
 
    float g1 = (1.0-g)*(1.0-g);
    float g2 = 1.0+(g*g);
    float g3 = 2.0*g;
 
    float phase = g1 / pow(g2 - g3*theta, 1.5);
     
    vec3 ret = pi14*phase*Bm;
    return ret;
}
 
vec3 inScatter(float s, float theta)
{
    vec3 num = rayleigh(theta) + mie(theta);
    num *= Esun.w;
    num *= 1.0-Fex(s);
 
    vec3 ret = num/Brm;
    return ret;
}
//http://www.gamedev.net/topic/584256-atmospheric-scattering-and-dark-sky/
float ExpScale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}



// Returns the near intersection point of a line and a sphere 
float getNearIntersection(vec3 v3Pos, vec3 v3Ray, float fDistance2, float fRadius2) 
{ 
    float B = 2.0 * dot(v3Pos, v3Ray); 
    float C = fDistance2 - fRadius2; 
    float fDet = max(0.0, B*B - 4.0 * C); 
    return 0.5 * (-B - sqrt(fDet)); 
} 
 
// Returns the far intersection point of a line and a sphere 
float getFarIntersection(vec3 v3Pos, vec3 v3Ray, float fDistance2, float fRadius2) 
{ 
    float B = 2.0 * dot(v3Pos, v3Ray); 
    float C = fDistance2 - fRadius2; 
    float fDet = max(0.0, B*B - 4.0 * C); 
    return 0.5 * (-B + sqrt(fDet)); 
} 
//*********************************************************************************//

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
void main(void)
{
	vec4 modelPosition = vec4(0,0,0,1);
	//vec3 eyePosition  = (-V*vec4(0,0,0,1)).xyz;


	vec3 tePosition = interpolate3Dquads( tcPosition[0], tcPosition[1], tcPosition[2], tcPosition[3]);
	gl_Position.xyz = interpolate3Dquads( gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);


	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );

	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;	

	tePosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	tePosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	tePosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));


	vec3 normal = tePosition - vec4(0,0,0,1).xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;

	tePosition = normal*(radiusAtm);

	//****************************** scattering atmosphere gem 2 ************************************//
	vec3 v3CameraPos  = inverse(-V)[3].xyz;
	vec4 pos = vec4(tePosition, 1);
	vec3 v3LightPos = vec3(0,0,2000);
	vec3 lightDir = v3LightPos - pos.xyz ;
	vec3 v3LightDir =  normalize(lightDir);  // Direction vector to the light source
	//vec3 v3InvWavelength = vec3(1,1,1) * (1 / pow(wavelength, 4)); // for RGB
	vec3 v3InvWavelength = vec3( 1.0 / pow( vWaveLength.x, 4 ), 1.0 / pow( vWaveLength.y, 4 ), 1.0 / pow( vWaveLength.z, 4 ) );
	float fCameraHeight = length( pos.xyz - v3CameraPos.xyz );    // The camera's current height
	float fCameraHeight2 = pow(fCameraHeight,2);   // fCameraHeight^2
	float fOuterRadius = radiusAtm;     // The outer (atmosphere) radius
	float fOuterRadius2 = pow(fOuterRadius,2);    // fOuterRadius^2
	float fInnerRadius = radius;     // The inner (planetary) radius
	float fInnerRadius2 = pow(fInnerRadius,2);     // fInnerRadius^2
	float fKrESun = Kr*ESun;          // Kr * ESun
	float fKmESun = Km * ESun;          // Km * ESun
	float fKr4PI = Kr * 4 * PI;           // Kr * 4 * PI
	float fKm4PI = Km * 4 * PI;           // Km * 4 * PI
	float fScale = 1.0 / (fOuterRadius - fInnerRadius);            //
	float fScaleOverScaleDepth = fScale / fScaleDepth ;			//
	float fInvScaleDepth = 1/fScaleDepth;
	float fSamples = 200.0;
	float nSamples = 100.0;
	
	// Get the ray from the camera to the vertex and its length (which
	// is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = vec4(M*vec4(pos.xyz,1)).xyz;// / fOuterRadius;	
	//v3Pos.y += fInnerRadius;
	//v3Pos.x += fInnerRadius;
	//v3Pos.z += fInnerRadius;

	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /=  fFar;

	vec3 v3FrontColor = vec3(0.225,0.225,0.5);
	//*********************************************** space from atmosphere ******************************//


	//// Calculate the farther intersection of the ray with the outer atmosphere (which is the far point of the ray passing through the atmosphere)
	//float B = 2.0 * dot(v3CameraPos, v3Ray);
	//float C = fCameraHeight2 - fOuterRadius2;
	//float fDet = max(0.0, B*B - 4.0 * C);
	//fFar = 0.5 * (-B + sqrt(fDet));

	//// Calculate attenuation from the camera to the top of the atmosphere toward the vertex
	//vec3 v3Start = v3CameraPos;
	//float fHeight = length(v3Start);
	//float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	//float fAngle = dot(v3Ray, v3Start) / fHeight;
	//float fScatter = fDepth*ExpScale(fAngle);

	
	//VertexOut.scattering = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
	//VertexOut.extinction = v3FrontColor * fKmESun;
	//VertexOut.light = v3CameraPos - v3Pos;
	//************************************************************************************************************************//
	
	//*********************************************** sky from atmosphere ******************************//

	//// Calculate the ray's starting position, then calculate its scattering offset
	//vec3 v3Start = v3CameraPos;
	//float fHeight = length(v3Start);
	//float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	//float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	//float fStartOffset = fDepth*ExpScale(fStartAngle);

	//// Initialize the scattering loop variables
	////gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	//float fSampleLength = fFar / fSamples;
	//float fScaledLength = fSampleLength * fScale;
	//vec3 v3SampleRay = v3Ray * fSampleLength;
	//vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	//// Now loop through the sample rays
	//for(int i=0; i<nSamples; i++)
	//{
	//	float fHeight = length(v3SamplePoint);
	//	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
	//	float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
	//	float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
	//	float fScatter = (fStartOffset + fDepth*(ExpScale(fLightAngle) - ExpScale(fCameraAngle)));
	//	vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
	//	v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
	//	v3SamplePoint += v3SampleRay;
	//}
	
	//VertexOut.scattering = v3FrontColor * (v3InvWavelength * fKrESun);
	//VertexOut.extinction = v3FrontColor * fKmESun;
	//VertexOut.light = v3CameraPos - v3Pos;

	//************************************************************************************************************************//

		
	//*********************************************** ground from atmosphere ******************************//

	// Calculate the ray's starting position, then calculate its scattering offset
	//vec3 v3Start = v3CameraPos;
	//float fDepth = exp((fInnerRadius - fCameraHeight) / fScaleDepth);
	//float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
	//float fLightAngle = dot(v3LightPos, v3Pos) / length(v3Pos);
	//float fCameraScale = ExpScale(fCameraAngle);
	//float fLightScale = ExpScale(fLightAngle);
	//float fCameraOffset = fDepth*fCameraScale;
	//float fTemp = (fLightScale + fCameraScale);

	//// Initialize the scattering loop variables
	//float fSampleLength = fFar / fSamples;
	//float fScaledLength = fSampleLength * fScale;
	//vec3 v3SampleRay = v3Ray * fSampleLength;
	//vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	//// Now loop through the sample rays
	//vec3 v3Attenuate = vec3(1,1,1);
	//for(int i=0; i<nSamples; i++)
	//{
	//	float fHeight = length(v3SamplePoint);
	//	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
	//	float fScatter = fDepth*fTemp - fCameraOffset;
	//	v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
	//	v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
	//	v3SamplePoint += v3SampleRay;
	//}
	
	//VertexOut.scattering = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);
	//VertexOut.extinction = v3Attenuate;
	//VertexOut.light = v3CameraPos - v3Pos;

	//************************************************************************************************************************//
	//http://www.vis.uni-stuttgart.de/~schafhts/HomePage/pubs/wscg07-schafhitzel.pdf
	//http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html
	//http://http.download.nvidia.com/developer/GPU_Gems_2/CD/Index.html
	//http://www.gamedev.net/topic/461747-atmospheric-scattering-sean-oneill---gpu-gems2/
	//************************************************************************************************************************//


	//vec3 v3Start = -v3CameraPos;
	//float fHeight = length(v3Start);
	//float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	//float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	//float fStartOffset = fDepth * ExpScale(fStartAngle);
			
	//// Init loop variables
	//float fSampleLength = fFar / fSamples;
	//float fScaledLength = fSampleLength * fScale;
	//vec3 v3SampleRay = v3Ray * fSampleLength;
	//vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5f;
	
	//// Loop the ray
	//vec3 color = vec3(0.0,0.0,0.58);
	//for (int i = 0; i < nSamples; i++)
	//{
	//	float fHeight = length(v3SamplePoint);
	//	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius-fHeight));
		
	//	float fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
	//	float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		
	//	float fScatter = (fStartOffset + fDepth*(ExpScale(fLightAngle) - ExpScale(fCameraAngle)));
	//	vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		
	//	// Accumulate color
	//	v3Attenuate *= (fDepth * fScaledLength);
	//	color += v3Attenuate;
		
	//	// Next sample point
	//	v3SamplePoint += v3SampleRay;
	//}

	//VertexOut.scattering.xyz = color * (v3InvWavelength * fKrESun);
	//VertexOut.extinction.xyz = color * fKmESun;
	//VertexOut.light = v3CameraPos - v3Pos;

	//************************************************************************************************************************//
	
	//http://gamedev.stackexchange.com/questions/58334/glsl-atmospheric-scattering-issue
	//************************************************************************************************************************//

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float B = 2.0 * dot(v3CameraPos, v3Ray);
	float C = fCameraHeight2 - fOuterRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	float fNear = 0.5 * (-B - sqrt(fDet));


	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = v3CameraPos + v3Ray * fNear;
	fFar -= fNear;
	float fDepth = exp((fInnerRadius - fOuterRadius) / fScaleDepth);
	float fCameraAngle = dot(-v3Ray, v3Pos) / fFar;
	float fLightAngle = dot(v3LightPos, v3Pos) / fFar;
	float fCameraScale = ExpScale(fCameraAngle);
	float fLightScale = ExpScale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	vec3 v3Attenuate = vec3(0,0,0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	VertexOut.scattering.xyz = v3FrontColor * (v3InvWavelength * fKrESun   * fKmESun);
	VertexOut.extinction.xyz = v3Attenuate;
	VertexOut.light = v3CameraPos - v3Pos;
	//************************************************************************************************************************//


	//// Calculate the closest intersection of the ray with
	//// the outer atmosphere (point A in Figure 16-3)
	//float fNear = getNearIntersection(v3CameraPos, v3Ray, fCameraHeight2, fOuterRadius2);

	//// Calculate the ray's start and end positions in the atmosphere,
	//// then calculate its scattering offset
	//vec3 v3Start = v3CameraPos + v3Ray * fNear;
	//fFar -= fNear;
	//float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
	//float fStartDepth = exp(fInvScaleDepth);
	//float fStartOffset = fStartDepth * ExpScale(fStartAngle);

	// // Initialize the scattering loop variables
	//float fSampleLength = fFar / fSamples;
	//float fScaledLength = fSampleLength * fScale;
	//vec3 v3SampleRay = v3Ray * fSampleLength;
	//vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	
	//// Now loop through the sample points
	//for(int i=0; i < nSamples; i++) 
	//{
	//	float fHeight = length(v3SamplePoint);
	//	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
	//	float fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
	//	float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
	//	float fScatter = (fStartOffset + fDepth * (ExpScale(fLightAngle) - ExpScale(fCameraAngle)));

	//	vec3 v3Attenuate = exp(-fScatter * ((v3InvWavelength * fKr4PI) + fKm4PI));

	//	v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
	//	v3SamplePoint += v3SampleRay;

	//}

	//VertexOut.scattering = v3FrontColor * (v3InvWavelength * fKrESun);
	//VertexOut.extinction = v3FrontColor * fKmESun;
	//VertexOut.light = v3CameraPos - v3Pos;
	//************************************************************************************************//

	 

	//VertexOut.light = normalize(lightDir);
 
	//VertexOut.normal = normalize(normal.xyz);
 
 //   vec3 wDir = normalize(pos.xyz - eyePosition);
	//float dist = length(pos.xyz - eyePosition );
 //   float cosTheta = dot(normal, VertexOut.light);
 
 //   VertexOut.scattering = inScatter(dist, cosTheta);
 //   VertexOut.extinction = Fex(dist);
 
 //   VertexOut.light = lightDir;


	gl_Position = vec4(tePosition, 1);
}



