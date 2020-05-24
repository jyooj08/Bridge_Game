#pragma once

#include <functional>
#include "global.h"

using namespace std; 

typedef function<void(void)> render_function_t;
typedef unsigned long long renderID;

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

struct render_function {
	render_function_t function;
	bool is_able = true;
	render_function(render_function_t f) { function = f; }
	bool excute() {
		if (!isAble())	return false;
		else {
			function();
			return true;
		}
	}
	bool isAble()	{ return is_able; }
	void disable()	{ is_able = false; }
	void enable()	{ is_able = true; }
};


void pushRenderFunction(render_function_t);
renderID attachRenderFunction(render_function_t render_function);
void detachRenderFunction(renderID& renderID);

void renderAll();

