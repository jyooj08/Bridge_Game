#pragma once

#include <functional>

using namespace std;

typedef function<void(void)> render_function_t;

void pushRenderFunction(render_function_t);

void renderAll();
