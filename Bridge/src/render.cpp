#include "render.h"

#include <queue>
#include <map>
#include <tuple>
#include <vector>
#include <iostream>
#include "global.h"

using namespace std;


queue<render_function_t> _renderQ;
map<renderID,render_function_t> _renderSet;
static renderID nextRenderID = 0;
queue<renderID> renderIDBasket;

void pushRenderFunction(render_function_t render_function) {
	_renderQ.push(render_function);
}
renderID attachRenderFunction(render_function_t render_function) {
	renderID newRenderID; 
	if(renderIDBasket.empty())
		newRenderID = nextRenderID++;
	else {
		newRenderID = renderIDBasket.front();
		renderIDBasket.pop();
	}
	_renderSet.insert(make_pair(newRenderID,render_function));
	return newRenderID;
}
void detachRenderFunction(renderID renderID) {
	if (_renderSet.erase(renderID)>=1) {
		renderIDBasket.push(renderID);
	}
	else {
		//cout << "WARNING : Non-existing renderID!" << endl;
	}
}



void renderAll() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram( program );
	glBindVertexArray(vertex_array);
	while (!_renderQ.empty()) {
		render_function_t f = _renderQ.front(); _renderQ.pop();
		f();
	}
	for (auto& p : _renderSet) {
		p.second();
	}
	glfwSwapBuffers(window);
}


