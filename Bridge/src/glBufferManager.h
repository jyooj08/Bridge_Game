#pragma once

#include "global.h"

#define MAX_GL_BUFFER_SIZE		((size_t)1000000000)	//100MB
#define DEFAULT_GL_BUFFER_SIZE	((size_t)10000000  )	//1MB

typedef struct glChunk {
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
}glChunk;

void my_glBufferInit();

void my_glBufferData(glChunk* chunk,const void* data);

glChunk* newVertexChunk(GLsizeiptr size);
glChunk* newIndexChunk(GLsizeiptr size);
void deleteGLChunk(glChunk* chunk);

void drawIndexChunk(glChunk* chunk);
