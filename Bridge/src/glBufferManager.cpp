#include "glBufferManager.h"
#include <iostream>

//*************************************
// OpenGL objects
GLuint	program	= 0;	// ID holder for GPU program
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
		vertex_buffer_size *= 2;
		if (vertex_buffer_size >= MAX_GL_BUFFER_SIZE) {
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

void drawIndexChunk(glChunk* chunk) {
	glDrawElementsBaseVertex(GL_TRIANGLES,
		chunk->size/sizeof(uint),
		GL_UNSIGNED_INT,
		nullptr,
		chunk->offset/sizeof(uint));
}
