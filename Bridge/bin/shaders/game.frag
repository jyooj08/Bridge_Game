#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

#define PI 3.1415926538

// input from vertex shader

in vec4 epos;
in vec3 norm;
in vec3 lnorm;
in vec3 real_norm;
in vec2 tc;
in vec4 wpos;
in vec4 FragPosLightSpace;


uniform uint color_mode;

// texture
uniform sampler2D shadowMap;



// light 
uniform mat4	view_matrix;
uniform vec4	light_position, Ia, Id, Is;	// light

uniform vec4	Ka, Kd, Ks;					// material properties
uniform float	shininess;

uniform float	Kbias;


// the only output variable
out vec4 fragColor;

// random function for noise
float rand() {
	vec2 co = epos.xy;
	float r=0;
	r += fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
	return r;
}
// source : learnopengl.com
float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
	float fx = fragPosLightSpace.x;
	float fy = fragPosLightSpace.y;
	float fz = fragPosLightSpace.z;
	float fw = fragPosLightSpace.w;
		
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(real_norm);
    vec3 lightDir = normalize(light_position.xyz - wpos.xyz);

	// show light area
	if (projCoords.z >= 1.0||
		projCoords.z <= 0.0 ||
		projCoords.x <= 0.0 ||
		projCoords.x >= 1.0 ||
		projCoords.y <= 0.0 ||
		projCoords.y >= 1.0 ||
		false
		)
		return 0.0;


    float bias = max(0.5 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	
	bias = -0.0000001f;
	if (dot(normal, lightDir) <= sin(PI/10)) { bias = 0.0000003f; }
	if(true){	// default pcf
		float r = 3;
		int d = 10;
		float t = rand() * PI/2;
		float area = 0;

		for (int x = -d; x <= d; ++x)
		{
			for (int y = -d; y <= d; ++y)
			{
				//if (x * x + y * y > d * d) continue;
				float pcfDepth = 
					texture (shadowMap, projCoords.xy + 
						vec2(x, y)*r/d * texelSize ) .r;
				float k = currentDepth - pcfDepth;
				//float l = abs(x) + abs(y)+1;
				float s = 1;
				if ( k >= bias )
					shadow += s;
				area += s;
			}
		}
		shadow /= area;
		float b = 0.7;
		//if (shadow > b) {
		//	shadow = 1.0f;
		//}
		//else {
		//	shadow *= 1 / ( b);
		//}
	}
	else { // test

	}

	if (dot(normal, lightDir) <= 0.0 && shadow<0.999f ) { return 1; } // back space 
	shadow *= 0.7f;
	//if (shadow < 0.3) shadow = 0.0;

        
    return 1-shadow;
}

vec4 phong( vec3 l, vec3 n, vec3 h, vec4 Kd )
{
	vec4 Ira = Ka*Ia;									// ambient reflection
	vec4 Ird = max(Kd*dot(l,n)*Id,0.0);					// diffuse reflection
	vec4 Irs = max(Ks*pow(dot(h,n),shininess)*Is,0.0);	// specular reflection
	return Ira + (Ird + Irs)*ShadowCalculation(FragPosLightSpace);
}
vec4 attenuate(vec4 color,vec4 lpos)
{
	vec3 l = normalize(lpos.xyz - epos.xyz);
	float d = length(lpos.xyz - epos.xyz);
	d/=100;
	float a = 1.0 / (1.0 + 0.22*d + 0.019*d*d);
	return color*a;
}


void main()
{
	// light position in the eye space
	vec4 lpos = view_matrix*light_position;

	vec3 n = normalize(norm);	// norm interpolated via rasterizer should be normalized again here
	vec3 p = epos.xyz;			// 3D position of this fragment
	vec3 l = normalize(lpos.xyz-(lpos.a==0.0?vec3(0):p));	// lpos.a==0 means directional light
	vec3 v = normalize(-p);		// eye-epos = vec3(0)-epos
	vec3 h = normalize(l+v);	// the halfway vector

	//vec4 iKd = texture( TEX, tc );	// Kd from image
	if(color_mode == 0u) {
		fragColor = phong(l, n ,h,Kd);
		//fragColor = attenuate(fragColor,light_position);
		//fragColor *= ShadowCalculation(FragPosLightSpace);
    } else if(color_mode == 1u) {
		fragColor = phong(l, n ,h,Kd);
		fragColor = attenuate(fragColor,light_position);
		fragColor *= vec4(FragPosLightSpace.xyz,0);
		//fragColor = vec4(texture(shadowMap,vec2(0,0)).r);
    } else if(color_mode == 2u) {
        fragColor = vec4(tc.yyy, 1);
    }

}
