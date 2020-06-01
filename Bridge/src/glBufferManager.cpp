#include "glBufferManager.h"
#include <set>
#include <iostream>

//*************************************
// OpenGL objects

GLuint	program	= 0;	// ID holder for GPU program
GLuint  shadow_program = 0;
GLuint vertex_buffer = 0;
GLuint index_buffer = 0;
GLuint vertex_array = 0;

// OpenGL simple linear buffer allocation

GLsizeiptr vertex_buffer_size= DEFAULT_GL_BUFFER_SIZE;
GLintptr vertex_buffer_offset=0;
GLsizeiptr index_buffer_size= DEFAULT_GL_BUFFER_SIZE;
GLintptr index_buffer_offset=0;

void my_glBufferInit() {
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	// allocate default buffer size
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, NULL, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, NULL, GL_STATIC_DRAW);
	vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
}

void my_glBufferData(glChunk* chunk,const void* data) {
	glBufferSubData(chunk->target, chunk->offset, chunk->size, data);
}

glChunk* newVertexChunk(GLsizeiptr size) {
	glChunk* newChunk = new glChunk;
	if (vertex_buffer_offset + size >= vertex_buffer_size) {
		vertex_buffer_size *= 2;
		if (vertex_buffer_size >= MAX_GL_BUFFER_SIZE) {
			std::cout << "WARNING! : openGL buffer size is too large!!";
		}
		/* TODO reallocate vertex buffer*/
		std::cout << "WARNING! : openGL buffer reallocation is not implemented";
	}
	newChunk->target = GL_ARRAY_BUFFER;
	newChunk->offset = vertex_buffer_offset; 
	newChunk->size = size;

	vertex_buffer_offset += size;
	return  newChunk;
}

glChunk* newIndexChunk(GLsizeiptr size) {
	glChunk* newChunk = new glChunk;
	if (index_buffer_offset + size >= index_buffer_size) {
		index_buffer_size *= 2;
		if (index_buffer_size >= MAX_GL_BUFFER_SIZE) {
			std::cout << "WARNING! : openGL buffer size is too large!!";
		}
		/* TODO reallocate index buffer*/
		std::cout << "WARNING! : openGL buffer reallocation is not implemented";
	}
	newChunk->target = GL_ELEMENT_ARRAY_BUFFER;
	newChunk->offset = index_buffer_offset; 
	newChunk->size = size;

	index_buffer_offset += size;
	return  newChunk;
}

void deleteGLChunk(glChunk* chunk) {
	
}

void drawIndexChunk(glChunk* chunk, GLint baseVertex) {
	glDrawElementsBaseVertex(GL_TRIANGLES,
		chunk->size/sizeof(uint),
		GL_UNSIGNED_INT,
		(void*)chunk->offset,
		baseVertex);
}
GLint applyModelMatrix(mat4 model_matrix) {
	GLint uloc = glGetUniformLocation(program, "model_matrix");
	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, model_matrix);
	return uloc;
}

GLint applyCamera(camera arg_cam) { // auto aspect
	arg_cam.aspect = window_size.x / float(window_size.y);
	arg_cam.update();
	GLint uloc;
	uloc = glGetUniformLocation(program, "view_matrix");
	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, arg_cam.view_matrix);
	uloc = glGetUniformLocation(program, "projection_matrix");
	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, arg_cam.projection_matrix);
	return uloc;
}

GLint setUniformVariable(std::string var_name, uint value) {
	GLint uloc;
	uloc = glGetUniformLocation(program, var_name.c_str());
	if (uloc > -1) glUniform1ui(uloc, value);
	else {
		// one time warning
		static set<string> err_var_name_set;
		if (err_var_name_set.find(var_name) == err_var_name_set.end()) {
			cout << "WARNING : GLSL uniform var '" << var_name << "' does not exist!" << endl;
			err_var_name_set.insert(var_name);
		}
	}
	return uloc;
}
