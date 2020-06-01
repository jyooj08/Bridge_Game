// vertex attributes
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

// matrices
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform mat4 lightSpaceMatrix;



// out
out vec4 epos;
out vec3 norm;
out vec3 real_norm;
out vec2 tc;
out vec4 wpos;
out vec3 lnorm;

out vec4 FragPosLightSpace;

void main()
{
	wpos = model_matrix * vec4(position,1);
	epos = view_matrix * wpos;

	gl_Position = projection_matrix * epos;

	// pass eye-coordinate normal to fragment shader
	norm = normalize(mat3(view_matrix*model_matrix)*normal);
	real_norm = normalize(mat3(model_matrix)*normal);

	// texture
	tc = texcoord;

	FragPosLightSpace = lightSpaceMatrix * vec4(wpos.xyz,1);
	lnorm = (lightSpaceMatrix * vec4(real_norm,1)).xyz;
}
