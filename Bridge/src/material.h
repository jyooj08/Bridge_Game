#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "glBufferManager.h"
#include <string>
#include <unordered_map>

struct Material
{
	vec4	ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4	diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4	specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float	shininess = 1000.0f;
};

// default samples

const static Material _emerald = { vec4(0.0215f,0.1745f,0.0215f,1.0f), vec4(0.07568f,0.61424f,0.07568f,1.0f), vec4(0.633f,0.727811f,0.633f,1.0f), 0.6f };
const static Material _jade = { vec4(0.135f,	0.2225f,	0.1575f,1.0f), vec4(0.54f,	0.89f,	0.63f,1.0f), vec4(0.316228f,	0.316228f,	0.316228f,1.0f), 0.1f };
const static Material _obsidian = { vec4(0.05375f,	0.05f,	0.06625f,1.0f), vec4(0.18275f,	0.17f,	0.22525f,1.0f), vec4(0.332741f,	0.328634f,	0.346435f,1.0f), 0.3f };
const static Material _pearl = { vec4(0.25f,	0.20725f,	0.20725f,1.0f), vec4(1.0f,	0.829f,	0.829f,1.0f), vec4(0.296648f,	0.296648f,	0.296648f,1.0f), 0.088f };
const static Material _ruby = { vec4(0.1745f,	0.01175f,	0.01175f,1.0f), vec4(0.61424f,	0.04136f,	0.04136f,1.0f), vec4(0.727811f,	0.626959f,	0.626959f,1.0f), 0.6f };
const static Material _turquoise = { vec4(0.1f,	0.18725f,	0.1745f,1.0f), vec4(0.396f,	0.74151f,	0.69102f,1.0f), vec4(0.297254f,	0.30829f,	0.306678f,1.0f), 0.1f };
const static Material _brass = { vec4(0.329412f,	0.223529f,	0.027451f,1.0f), vec4(0.780392f,	0.568627f,	0.113725f,1.0f), vec4(0.992157f,	0.941176f,	0.807843f,1.0f), 0.21794872f };
const static Material _bronze = { vec4(0.2125f,	0.1275f,	0.054f,1.0f), vec4(0.714f,	0.4284f,	0.18144f,1.0f), vec4(0.393548f,	0.271906f,	0.166721f,1.0f), 0.2f };
const static Material _chrome = { vec4(0.25f,	0.25f,	0.25f,1.0f), vec4(0.4f,	0.4f,	0.4f,1.0f), vec4(0.774597f,	0.774597f,	0.774597f,1.0f), 0.6f };
const static Material _copper = { vec4(0.19125f,	0.0735f,	0.0225f,1.0f), vec4(0.7038f,	0.27048f,	0.0828f,1.0f), vec4(0.256777f,	0.137622f,	0.086014f,1.0f), 0.1f };
const static Material _gold = { vec4(0.24725f,	0.1995f,	0.0745f,1.0f), vec4(0.75164f,	0.60648f,	0.22648f,1.0f), vec4(0.628281f,	0.555802f,	0.366065f,1.0f), 0.4f };
const static Material _silver = { vec4(0.19225f,	0.19225f,	0.19225f,1.0f), vec4(0.50754f,	0.50754f,	0.50754f,1.0f), vec4(0.508273f,	0.508273f,	0.508273f,1.0f), 0.4f };
const static Material _black_plastic = { vec4(0.0f,	0.0f,	0.0f,1.0f), vec4(0.01f,	0.01f,	0.01f,1.0f), vec4(0.50f,	0.50f,	0.50f,1.0f), .25f };
const static Material _cyan_plastic = { vec4(0.0f,	0.1f,	0.06f,1.0f), vec4(0.0f,	0.50980392f,	0.50980392f,1.0f), vec4(0.50196078f,	0.50196078f,	0.50196078f,1.0f), .25f };
const static Material _green_plastic = { vec4(0.0f,	0.0f,	0.0f,1.0f), vec4(0.1f,	0.35f,	0.1f,1.0f), vec4(0.45f,	0.55f,	0.45f,1.0f), .25f };
const static Material _red_plastic = { vec4(0.0f,	0.0f,	0.0f,1.0f), vec4(0.5f,	0.0f,	0.0f,1.0f), vec4(0.7f,	0.6f,	0.6f,1.0f), .25f };
const static Material _white_plastic = { vec4(0.0f,	0.0f,	0.0f,1.0f), vec4(0.55f,	0.55f,	0.55f,1.0f), vec4(0.70f,	0.70f,	0.70f,1.0f), .25f };
const static Material _yellow_plastic = { vec4(0.0f,	0.0f,	0.0f,1.0f), vec4(0.5f,	0.5f,	0.0f,1.0f), vec4(0.60f,	0.60f,	0.50f,1.0f), .25f };
const static Material _black_rubber = { vec4(0.02f,	0.02f,	0.02f,1.0f), vec4(0.01f,	0.01f,	0.01f,1.0f), vec4(0.4f,	0.4f,	0.4f,1.0f), .078125f };
const static Material _cyan_rubber = { vec4(0.0f,	0.05f,	0.05f,1.0f), vec4(0.4f,	0.5f,	0.5f,1.0f), vec4(0.04f,	0.7f,	0.7f,1.0f), .078125f };
const static Material _green_rubber = { vec4(0.0f,	0.05f,	0.0f,1.0f), vec4(0.4f,	0.5f,	0.4f,1.0f), vec4(0.04f,	0.7f,	0.04f,1.0f), .078125f };
const static Material _red_rubber = { vec4(0.05f,	0.0f,	0.0f,1.0f), vec4(0.5f,	0.4f,	0.4f,1.0f), vec4(0.7f,	0.04f,	0.04f,1.0f), .078125f };
const static Material _white_rubber = { vec4(0.05f,	0.05f,	0.05f,1.0f), vec4(0.5f,	0.5f,	0.5f,1.0f), vec4(0.7f,	0.7f,	0.7f,1.0f), .078125f };
const static Material _yellow_rubber = { vec4(0.05f,	0.05f,	0.0f,1.0f), vec4(0.5f,	0.5f,	0.4f,1.0f), vec4(0.7f,	0.7f,	0.04f,1.0f), .078125f };

static void applyMaterial(Material material) {
	glUniform4fv(glGetUniformLocation(program, "Ka"), 1, material.ambient);
	glUniform4fv(glGetUniformLocation(program, "Kd"), 1, material.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Ks"), 1, material.specular);
	glUniform1f (glGetUniformLocation(program, "shininess"), material.shininess);
}


#endif 