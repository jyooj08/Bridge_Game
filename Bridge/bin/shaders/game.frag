#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec3 norm;
in vec2 tc;

uniform uint color_mode;

// the only output variable
out vec4 fragColor;

void main()
{
	if(color_mode == 0u) {
        fragColor = vec4(tc.xy, 0, 1);
    } else if(color_mode == 1u) {
        fragColor = vec4(tc.xxx, 1);
    } else if(color_mode == 2u) {
        fragColor = vec4(tc.yyy, 1);
    }

	//fragColor = vec4(normalize(norm), 1.0);
}
