layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model_matrix;
uniform mat4 light_padding;


void main()
{
    gl_Position = lightSpaceMatrix * model_matrix * vec4(aPos, 1.0);
}