#ifndef _RENDER_H_
#define _RENDER_H_
#include <functional>
#include "global.h"
#include "3dObject.h"
#include "MyText.h"

using namespace std;

extern  unsigned int SHADOW_WIDTH;
extern  unsigned int SHADOW_HEIGHT;
extern unsigned int depthMapFBO;
extern unsigned int depthMap;

typedef function<void(void)> render_function_t;
typedef unsigned long long renderID;

struct light_t
{
	vec4	position = vec4(1.0f, 1.0f, 1.0f, 0.0f);   // directional light
	vec4	ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4	diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4	specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};


struct render_function {
	render_function_t function;
	bool is_text = false;
	bool is_able = true;
	bool light_blocking = true;
	bool light_effect = true;
	render_function(render_function_t f, bool lb = true, bool le = true) {
		function = f; light_blocking = lb, light_effect = le;
	}
	bool excute() {
		if (!isAble())	return false;
		else {
			function();
			return true;
		}
	}
	void enableLightBlocking() { light_blocking = true; }
	void disableLightBlocking() { light_blocking = false; }
	void enableLightEffect() { light_effect = true; }
	void disableLightEffect() { light_effect = false; }
	bool isAble() { return is_able; }
	void disable() { is_able = false; }
	void enable() { is_able = true; }
	bool isText() { return is_text; }
};


void pushRenderFunction(render_function_t);
renderID attachRenderFunction(render_function_t render_function, bool light_blocking = true);
renderID attachTextRenderFunction(render_function_t render_function);
renderID put2render(Basic3dObject* obj, bool light_blocking = true);
renderID put2render(MyText* text);

void detachRenderFunction(renderID& renderID);

void renderAll(GLuint ID = program);

void enableLightBlocking(renderID id);
void disableLightBlocking(renderID id);
void enableLightEffect(renderID id);
void disableLightEffect(renderID id);
bool isAble(renderID id);
void enable(renderID id);
void disable(renderID id);

#endif 
