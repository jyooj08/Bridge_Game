#include "render.h"

#include <queue>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <iostream>
#include "global.h"
#include "text.h"

using namespace std;


queue<render_function_t> _renderQ;
unordered_map<renderID,render_function> _renderSet;
static renderID nextRenderID = 0;
queue<renderID> renderIDBasket;

void pushRenderFunction(render_function_t function) {
	_renderQ.push(function);
}
renderID attachRenderFunction(render_function_t function) {
	renderID newRenderID; 
	if(renderIDBasket.empty())
		newRenderID = nextRenderID++;
	else {
		newRenderID = renderIDBasket.front();
		renderIDBasket.pop();
	}
	_renderSet.insert(make_pair(newRenderID,render_function(function)));
	return newRenderID;
}
void detachRenderFunction(renderID& renderID) {
	if (renderID == 0) return;
	if (_renderSet.erase(renderID)>=1) {
		renderIDBasket.push(renderID);
	}
	else {
		//cout << "WARNING : Non-existing renderID!" << endl;
	}
	renderID = 0;
}



void renderAll() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// render texts
	float dpi_scale = cg_get_dpi_scale();
	render_text("Sample text", 100, 100, 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), dpi_scale);

	glUseProgram( program );
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindVertexArray(vertex_array);
	while (!_renderQ.empty()) {
		render_function_t f = _renderQ.front(); _renderQ.pop();
		f();
	}
	for (auto& p : _renderSet) {
		p.second.excute();
	}

	glfwSwapBuffers(window);
}