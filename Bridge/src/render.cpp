#include "render.h"

#include <queue>
#include <vector>
#include "global.h"

using namespace std;


queue<render_function_t> _renderQ;

void pushRenderFunction(render_function_t render_function) {
	_renderQ.push(render_function);
}

void renderAll() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram( program );
	glBindVertexArray(vertex_array);
	while (!_renderQ.empty()) {
		render_function_t f = _renderQ.front(); _renderQ.pop();
		f();
	}
	glfwSwapBuffers(window);
}

