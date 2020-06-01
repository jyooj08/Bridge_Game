#include "render.h"
#include "text.h"
#include <queue>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <iostream>
#include <functional>
#include "global.h"
#include "model.h"
#include "logic.h"

using namespace std;

unsigned int SHADOW_WIDTH  = 1024;
unsigned int SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;
mat4 lightSpaceMatrix;

queue<render_function_t> _renderQ;
unordered_map<renderID,render_function> _renderSet;
static renderID nextRenderID = 0;
queue<renderID> renderIDBasket;

void pushRenderFunction(render_function_t function) {
	_renderQ.push(function);
}
renderID attachRenderFunction(render_function_t function,bool light_blocking ) {
	renderID newRenderID; 
	if(renderIDBasket.empty())
		newRenderID = nextRenderID++;
	else {
		newRenderID = renderIDBasket.front();
		renderIDBasket.pop();
	}
	_renderSet.insert(make_pair(newRenderID,render_function(function,light_blocking)));
	return newRenderID;
}
renderID put2render(Basic3dObject* obj,bool light_blocking) {
	return attachRenderFunction([obj]() { obj->render(); });
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


void shadowMapRendering() {
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("something wrong with FBO attachment\n");
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadow_program);
	extern light_t light;
	vec3 lightPos = vec3(light.position.x, light.position.y, light.position.z);

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	mat4 lightProjection, lightView;
	mat4 light_padding;
	float near_plane = 0.1f, far_plane = 1000.0f;
	lightProjection = mat4::perspective(PI*4/16, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	//lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = mat4::look_at(lightPos, global_cam.at, vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	light_padding = mat4::scale(1.0f);
	// render scene from light's point of view

	GLint uloc;
	uloc = glGetUniformLocation(shadow_program, "lightSpaceMatrix");
	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, lightSpaceMatrix);
	uloc = glGetUniformLocation(shadow_program, "light_padding");
	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, light_padding);
	
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT );
	GLuint past_program = program;
	program = shadow_program;
	// TODO : bind texture before shadow mapping

	glUniform1f (glGetUniformLocation(program, "Kbias"), 0.0000006f); // recommend 0.0000006f
	for (auto& p : _renderSet) {
		if (p.second.light_blocking == false) continue;
		//p.second.excute();
	}

	glUniform1f (glGetUniformLocation(program, "Kbias"), 0.000000f);
	glCullFace(GL_FRONT);
	// rendering backface culling off
	for (auto& p : _renderSet) {
		if (p.second.light_blocking == false) continue;
		p.second.excute();
	}
	glCullFace(GL_BACK);
	program = past_program;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void renderAll(GLuint ID) {
	shadowMapRendering();
	glViewport(0, 0, window_size.x, window_size.y);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// render texts
	//float dpi_scale = cg_get_dpi_scale();
	//render_text("Sample text", 100, 100, 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), dpi_scale);

	glUseProgram( ID );
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindVertexArray(vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	

	GLint uloc = glGetUniformLocation(ID, "lightSpaceMatrix");
	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, lightSpaceMatrix);
	while (!_renderQ.empty()) {
		render_function_t f = _renderQ.front(); _renderQ.pop();
		f();
	}
	for (auto& p : _renderSet) {
		p.second.excute();
	}

	render_logic();
	glfwSwapBuffers(window);
}



void enableLightBlocking(renderID id) { _renderSet.find(id)->second.enableLightBlocking(); }
void disableLightBlocking(renderID id) { _renderSet.find(id)->second.disableLightBlocking(); }
void enableLightEffect(renderID id) { _renderSet.find(id)->second.enableLightEffect(); }
void disableLightEffect(renderID id) { _renderSet.find(id)->second.disableLightEffect(); }
bool isAble(renderID id) { return _renderSet.find(id)->second.isAble(); }
void enable(renderID id)	{_renderSet.find(id)->second.enable();}
void disable(renderID id)	{_renderSet.find(id)->second.disable();}

