#version 430 core

//layout(triangles, equal_spacing, cw) in;
layout(triangles, fractional_even_spacing, ccw) in;
//layout(triangles, fractional_odd_spacing, cw) in;





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
	smooth float noise;
} VertexOut; 

//************************************************************//

//*********** UNIFORM VARIABLES **********************************//
uniform mat4 M;
uniform float ampgain;
uniform float frqgain;
uniform float initamp;
uniform float initfrq;
uniform float radius;
uniform float octv;
//***************************************************************//


//************************************************************************************************************//
//Noise funtions *********************************************************************************************//
//************************************************************************************************************//
float floatNoise(float x, float y, float z)
{
	int val = int(x) + int(y) + int(z) * 57;
	val = (val<<13)^val;
	return (1.0 - ( (val * (val * val * 15731 + 789221) + 1376312589 ) & 0x7fffffff ) / 1073741824.0 );
}

float floatNoise2(float x, float y, float z)
{
	int val = int(x) + int(y) + int(z) * 57;
	val = (val<<13)^val;
	return val*val*val*(val*(val*6-15)+10);
}

float smoothNoise(float x, float y, float z)
{
	float corners = (floatNoise(x-1,y-1,z-1) + floatNoise(x-1,y-1,z+1) + floatNoise(x+1,y-1,z-1) + floatNoise(x+1,y-1,z+1) + floatNoise(x-1,y+1,z-1) + floatNoise(x-1,y+1,z+1) + floatNoise(x+1,y+1,z-1) + floatNoise(x+1,y+1,z+1) ) / 32.0;
	float sides = (floatNoise(x-1,y,z) + floatNoise(x+1,y,z) + floatNoise(x,y-1,z) + floatNoise(x,y+1,z) + floatNoise(x,y,z-1) + floatNoise(x,y,z+1) ) / 16.0;
	float center = (floatNoise(x,y,z)  ) / 8.0;

	return corners + sides + center;
}

float linearInterpolation(float a, float b, float x)
{
	return x * (1-x) + b*x;
}

float cosineInterpolation(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0- cos(ft) ) *0.5f;

	return a * (1.0-f) + b*f;
}

int fastfloor(double x) {
	if(x>0)
		return int(x);
	else
		return int(x-1);
 }

float InterpolatedNoise( float x, float y, float z)
{
	float val_x =  fastfloor(x);
	float fractional_x = x - val_x;

	float val_y =  fastfloor(y);
	float fractional_y = y - val_y;

	float val_z =  fastfloor(z);
	float fractional_z = z - val_z;

	float v1 = floatNoise(val_x,	val_y,	 val_z);
	float v2 = floatNoise(val_x+1, val_y,	 val_z);
	float v3 = floatNoise(val_x,	val_y+1, val_z);
	float v4 = floatNoise(val_x+1, val_y+1, val_z);
	float v5 = floatNoise(val_x,	val_y,	 val_z+1);
	float v6 = floatNoise(val_x+1, val_y,	 val_z+1);
	float v7 = floatNoise(val_x,	val_y+1, val_z+1);
	float v8 = floatNoise(val_x+1, val_y+1, val_z+1);

	float i1 = cosineInterpolation(v1,v2,fractional_x );
	float i2 = cosineInterpolation(v3,v4,fractional_x );
	float i3 = cosineInterpolation(v5,v6,fractional_x );
	float i4 = cosineInterpolation(v7,v8,fractional_x );

	float ii1 = cosineInterpolation(i1,i2,fractional_y );
	float ii2 = cosineInterpolation(i3,i4,fractional_y );

	return cosineInterpolation(ii1,ii2,fractional_z );
}

float perlinNoise_3D(float x, float y, float z)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;

	for(int i = 0; i < n ; i++)
	{/*
		float frequency = pow(2,i);
		float amplitude = pow(p,i);*/

		//float frequency = initfrq*pow(frqgain,i);
		//float amplitude = initamp*pow(ampgain,i);

		//total += InterpolatedNoise(x * frequency, y * frequency , z * frequency) * amplitude;

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);

		total += InterpolatedNoise(x / frequency, y / frequency , z / frequency) * amplitude;
	
	}



	return total;
}

float perlinNoise_3DRigid(float x, float y, float z)
{
	float total = 0;
	float ampgainl = 0.57;
	float frqgainl = 4.93;
	float initampl = 0.701;
	float initfrql = 0.337;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrql/pow(2,i);
		float amplitude = initampl/pow(2,i);
		
		float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency) ;


		//float frequency = initfrql*pow(frqgainl,i);
		//float amplitude = initampl*pow(ampgainl,i);

		//float add = InterpolatedNoise(x * frequency, y * frequency , z * frequency) ;
		if( add > 0)
		{
			add = (-add) + 1;
		}	
		else
		{
			add = add + 1;
		}
		total += add * amplitude;

	}
	return total;
}


float perlinNoise_3DMultiFractal(float x, float y, float z)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;
	float k = 0.0001;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);
		
		float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency)* amplitude ;


		//float frequency = initfrq*pow(frqgain,i);
		//float amplitude = initamp*pow(ampgain,i);

		//float add = InterpolatedNoise(x * frequency, y * frequency , z * frequency) * amplitude;
		
		if( i > 1)
		{
			add *= k * total;
		}	

		total += add ;

	}
	return total;
}

float perlinNoise_3DRigidMultiFractal(float x, float y, float z)
{
	float total = 0;
	//float ampgain = 0.5;
	//float frqgain = 2.0;
	//float initamp = 1.0;
	//float initfrq = 1.0;
	float p = 1;
	//float p = 0.7;
	float n = octv-1;
	float k = 1.0;

	for(int i = 0; i < n ; i++)
	{

		float frequency = initfrq/pow(2,i);
		float amplitude = initamp/pow(2,i);
		
		//float add  = InterpolatedNoise(x / frequency, y / frequency , z / frequency) ;


		//float frequency = initfrq*pow(frqgain,i);
		//float amplitude = initamp*pow(ampgain,i);

		float add = InterpolatedNoise(x / frequency, y / frequency , z / frequency);
		float add2 = InterpolatedNoise(x / frequency, y / frequency , z / frequency)* amplitude;

		if( add > 0)
		{
			add = (-add) + 1.0;		
		}	
		else
		{
			add = add + 1.0;
		}

		if( i > 1)
		{
			add2 *= k * total;
		}
		total += add2 * add * amplitude;

	}
	return total;
}
//********************** 3D SIMPLEX NOISE **************************************************************************************//


float smoothSimplexNoise(float x, float y, float z)
{
	float corners = (floatNoise(x,y,z+1) + floatNoise(x,y,z-1) + floatNoise(x,y,z-1) + floatNoise(x,y,z-1)  ) / 36.0;
	float sides = (floatNoise(x-1,y, z-1) + floatNoise(x+1,y,z-1) + floatNoise(x,y,z+1) + floatNoise(x,y-1,z)  ) / 18.0;
	float center = (floatNoise(x,y,z)  ) / 9.0;

	return corners + sides  + center;
}

float InterpolatedSimplexNoise( float xin, float yin, float zin)
{
	float n0, n1, n2, n3; // Noise contributions from the four corners
	
	// Skew the input space to determine which simplex cell we're in
	float F3 = 1.0/3.0;
	float s = (xin+yin+zin)*F3; // Very nice and simple skew factor for 3D
	int i = fastfloor(xin+s);
	int j = fastfloor(yin+s);
	int k = fastfloor(zin+s);

	float G3 = 1.0/6.0; // Very nice and simple unskew factor, too
	float t = (i+j+k)*G3; 
	float X0 = i-t; // Unskew the cell origin back to (x,y,z) space
	float Y0 = j-t;
	float Z0 = k-t;
	float x0 = xin-X0; // The x,y,z distances from the cell origin
	float y0 = yin-Y0;
	float z0 = zin-Z0;

	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	// Determine which simplex we are in.
	int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
	int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

	if(x0>=y0) 
	{
		if(y0>=z0)
		{ 
			i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; 
		} // X Y Z order
		else if(x0>=z0) 
		{ 
			i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; 
		} // X Z Y order
		else 
		{ 
			i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; 
		} // Z X Y order
	}
	else 
	{ // x0<y0
		if(y0<z0) 
		{ 
			i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; 
		} // Z Y X order
		else if(x0<z0) 
			{ 
				i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; 
			} // Y Z X order
		else 
		{ 
			i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; 
		} // Y X Z order
	}

	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	// c = 1/6.

	float x1 = x0 - i1 + G3;			// Offsets for second corner in (x,y,z) coords
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0*G3;		// Offsets for third corner in (x,y,z) coords
	float y2 = y0 - j2 + 2.0*G3;
	float z2 = z0 - k2 + 2.0*G3;
	float x3 = x0 - 1.0 + 3.0*G3;		// Offsets for last corner in (x,y,z) coords
	float y3 = y0 - 1.0 + 3.0*G3;
	float z3 = z0 - 1.0 + 3.0*G3;

	 // Work out the hashed gradient indices of the four simplex corners
	 int ii = i & 255;
	 int jj = j & 255;
	 int kk = k & 255;
	 int gi0 = int(floatNoise(ii,jj,kk));			// perm[ii+perm[jj+perm[kk]]] % 12;
	 int gi1 = int(floatNoise(ii+i1,jj+j1,kk+k1));	//perm[ii+i1+perm[jj+j1+perm[kk+k1]]] % 12;
	 int gi2 = int(floatNoise(ii+i2,jj+j2,kk+k2));	//perm[ii+i2+perm[jj+j2+perm[kk+k2]]] % 12;
	 int gi3 = int(floatNoise(ii+1,jj+1,kk+1));		//perm[ii+1+perm[jj+1+perm[kk+1]]] % 12;

	 // Calculate the contribution from the four corners
	 float t0 = 0.5 - x0*x0 - y0*y0 - z0*z0;
	 if(t0<0) 
		n0 = 0.0;
	 else {
	 t0 *= t0;
	 n0 = t0 * t0 * smoothSimplexNoise(x0,y0,z0);  //dot(grad3[gi0], x0, y0, z0);
	 }
	 float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
	 if(t1<0) 
		n1 = 0.0;
	 else {
	 t1 *= t1;
	 n1 = t1 * t1 * smoothSimplexNoise(x1+i1,y1+j1,z1+k1); //dot(grad3[gi1], x1, y1, z1);
	 }
	 float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
	 if(t2<0) 
		n2 = 0.0;
	 else {
	 t2 *= t2;
	 n2 = t2 * t2 * smoothSimplexNoise(x2+i2,y2+j2,z2+k2); //dot(grad3[gi2], x2, y2, z2);
	 }
	 float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
	 if(t3<0) 
		n3 = 0.0;
	 else {
	 t3 *= t3;
	 n3 = t3 * t3 * smoothSimplexNoise(x3+1,y3+1,z3+1); //dot(grad3[gi3], x3, y3, z3);
	 }
	 // Add contributions from each corner to get the final noise value.
	 // The result is scaled to stay just in
	 return 32.0*(n0 + n1 + n2 + n3);
}


// 3D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float scaled_raw_noise_3d( const float loBound, const float hiBound, const float x, const float y, const float z ) {
    return InterpolatedSimplexNoise(x, y, z) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}


float SimplexPerlinNoise_3D(float x, float y, float z)
{
	float total = 0;
	float p = 0.4;
	//float p = 0.7;
	float n = 15-1;

	for(int i = 0; i < n ; i++)
	{
		float frequency = pow(2,i);
		float amplitude = pow(p,i);

		total += InterpolatedSimplexNoise(x * frequency, y * frequency , z * frequency) * amplitude;
	
	}

	return total;
}


//************************************************************************************************************//



//************************************************************************************************************//




//******************************************** FUNCTIONS *****************************************//
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}
 
//************************************************************************************************//


void main(void)
{

	vec4 modelPosition = M*vec4(0,0,0,1);
	

	vec3 tePosition  = (gl_TessCoord.x * tcPosition[0]) +
                  (gl_TessCoord.y * tcPosition[1]) +
                  (gl_TessCoord.z * tcPosition[2]);
	//vec3 tePosition2  = (gl_TessCoord.x * tcPosition[1]) +
 //                 (gl_TessCoord.y * tcPosition[2]) +
 //                 (gl_TessCoord.z * tcPosition[3]);
	//vec3 tePosition3  = (gl_TessCoord.x * tcPosition[2]) +
 //                 (gl_TessCoord.y * tcPosition[0]) +
 //                 (gl_TessCoord.z * tcPosition[5]);
	//vec3 tePosition4  = (gl_TessCoord.x * tcPosition[0]) +
 //                 (gl_TessCoord.y * tcPosition[1]) +
 //                 (gl_TessCoord.z * tcPosition[3]);


	gl_Position  = (gl_TessCoord.x * gl_in[0].gl_Position) +
    (gl_TessCoord.y * gl_in[1].gl_Position) +
    (gl_TessCoord.z * gl_in[2].gl_Position);

	VertexOut.texCoord = interpolate2D( VertexIn[0].texCoord, VertexIn[1].texCoord, VertexIn[2].texCoord );
	VertexOut.normal = interpolate3D( VertexIn[0].normal, VertexIn[1].normal, VertexIn[2].normal );
	//VertexOut.fragmentColor = interpolate3D( VertexIn[0].fragmentColor, VertexIn[1].fragmentColor, VertexIn[2].fragmentColor );

	float x2 = gl_Position.x * gl_Position.x;
	float y2 = gl_Position.y * gl_Position.y;
	float z2 = gl_Position.z * gl_Position.z;	

	tePosition.x = gl_Position.x* (sqrt(1-(y2/2)-(z2/2)+((y2*z2)/3)));
	tePosition.y = gl_Position.y* (sqrt(1-(z2/2)-(x2/2)+((z2*x2)/3)));
	tePosition.z = gl_Position.z* (sqrt(1-(x2/2)-(y2/2)+((x2*y2)/3)));

	vec3 normal = tePosition - modelPosition.xyz;
	normal = normalize(normal);
	VertexOut.normal = normal;

	float perlinN = perlinNoise_3DRigidMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	//float perlinN = SimplexPerlinNoise_3D(tePosition.x,tePosition.y,tePosition.z);
	tePosition = modelPosition.xyz + normal*radius + normal * perlinN;
	//tePosition += VertexOut.normal * abs(perlinN);

	//vec3 average = (tePosition2+tePosition3+tePosition4)/3;
	//vec3 vector1 = gl_TessCoord.x * tcPosition[0] - tePosition;
	//vec3 vector2 = gl_TessCoord.x * tcPosition[1] - tePosition;
	//vec3 vector3 = gl_TessCoord.x * tcPosition[2] - tePosition;

	//vec3 vector4 = cross(vector2,vector1);
	//if(dot(VertexOut.normal,vector4) < 0 )
	//	vector4 = cross(vector1,vector2);
	//vec3 vector5 = cross(vector3,vector2);
	//if(dot(VertexOut.normal,vector5) < 0 )
	//	vector5 = cross(vector2,vector3);
	//vec3 vector6 = cross(vector3,vector1);
	//if(dot(VertexOut.normal,vector6) < 0 )
	//	vector6 = cross(vector1,vector3);
	//vec3 average = (vector4+vector5+vector6)/3;
	//VertexOut.normal = normalize(average);
	//perlinN = perlinNoise_3DMultiFractal(tePosition.x,tePosition.y,tePosition.z);
	//tePosition = modelPosition.xyz + normal*radius + normal * perlinN;


	//perlinN = perlinNoise_3DRigidMultiFractal(tePosition2.x,tePosition2.y,tePosition2.z);
	//tePosition2 = modelPosition.xyz + teNormal2*radius + teNormal2 * perlinN;
	//vec3 vector1 = tePosition2 - tePosition;
	//perlinN = perlinNoise_3DRigidMultiFractal(tePosition3.x,tePosition3.y,tePosition3.z);
	//tePosition3 = modelPosition.xyz + teNormal3*radius + teNormal3 * perlinN;
	//vec3 vector2 = tePosition3 - tePosition;
	//perlinN = perlinNoise_3DRigidMultiFractal(tePosition4.x,tePosition4.y,tePosition4.z);
	//tePosition4 = modelPosition.xyz + teNormal4*radius + teNormal4 * perlinN;
	//vec3 vector3 = tePosition4 - tePosition;

	//vec3 vector4 = tePosition3 - tePosition2;
	//vec3 vector5 = tePosition2 - tePosition3;

	//vec3 c = cross(vector1, vector2);
	//vec3 c2 = cross(vector1, vector3);
	//vec3 c3 = (c2-c)/2;

	//if(dot(VertexOut.normal,c) < 0 )
	//{
	//	c = cross(vector2,vector1);
	//	if(dot(normal,c) < 0 )
	//	{	
	//		c = cross(vector1,vector3);
	//		if(dot(normal,c) < 0 )
	//		{
	//			c = cross(vector3,vector1);
	//			vec3 c2 = cross(vector2, c);
	//			if(dot(normal,c2) < 0 )
	//				c = cross(c2,vector2);
	//		}
		
	//	}
	//	else
	//	{
	//		vec3 c2 = cross(vector3, c);
	//			if(dot(normal,c2) < 0 )
	//				c = cross(c2,vector3);
	//	}
		
			
	//}
	

	//VertexOut.normal = normalize(c3);


	//TEST
	//tePosition.x += perlinN;
	//tePosition.y += perlinN * VertexOut.normal.y;
	//tePosition.z += perlinN/4;
	//tePosition.x = 2 * cos(gl_TessCoord.x*9.42) * sin(gl_TessCoord.y * 9.42); 
	//tePosition.y = 2 * sin(gl_TessCoord.x*9.42) * sin(gl_TessCoord.y *9.42); 
	//tePosition.z = 2 * cos(gl_TessCoord.y *9.42);
	
	VertexOut.noise = perlinN;
	gl_Position = vec4(tePosition, 1);

	
	
	//float m_distance = length(tePosition - modelPosition.xyz);
	//if(m_distance >= (radius + 0.25))
	//{
	//	VertexOut.fragmentColor = vec3(0.545,0.514,0.471);// + vec3(0.545,0.514,0.471)*perlinN/2;
	//}
	//else if(m_distance >= (radius + 0.15) && m_distance < (radius + 0.25))
	//{
	//	VertexOut.fragmentColor = vec3(0.745,0.745,0.745);// + vec3(0.745,0.745,0.745) *perlinN/2;
	//}
	//else if(m_distance >= (radius + 0.01) && m_distance < (radius + 0.15))
	//{
	//	VertexOut.fragmentColor = vec3(0.419,0.557,0.138);// + vec3(0.419,0.557,0.138)*perlinN/2;
	//}
	//else if(m_distance >= (radius - 0.01) && m_distance < (radius + 0.01))
	//{		
	//	VertexOut.fragmentColor = vec3(0.109,0.525,0.933);// + vec3(0.109,0.525,0.933)*perlinN/2;
		
	//}
	//else 
	//{		
	//	VertexOut.fragmentColor = vec3(0.109,0.425,0.933);// + vec3(0.109,0.525,0.933)*perlinN/2;
		
	//}
	
	
}



