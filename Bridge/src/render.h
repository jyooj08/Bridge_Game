#pragma once

#include <functional>
#include "global.h"

using namespace std; 

struct light_t
{
	vec4	position = vec4(1.0f, 1.0f, 1.0f, 0.0f);   // directional light
	vec4	ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4	diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4	specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct material_t
{
	vec4	ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4	diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4	specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float	shininess = 1000.0f;
};

typedef function<void(void)> render_function_t;
typedef unsigned long long renderID;

void pushRenderFunction(render_function_t);
renderID attachRenderFunction(render_function_t render_function);
void detachRenderFunction(renderID& renderID);

void renderAll();

