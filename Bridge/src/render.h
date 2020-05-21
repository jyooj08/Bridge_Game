#pragma once

#include <functional>

using namespace std;

typedef function<void(void)> render_function_t;
typedef unsigned long long renderID;

void pushRenderFunction(render_function_t);
renderID attachRenderFunction(render_function_t render_function);
void detachRenderFunction(renderID renderID);

void renderAll();

