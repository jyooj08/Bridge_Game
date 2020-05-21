#pragma once

#include "global.h"
#include <vector>
#include <iostream>
using namespace std;

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

void drawIndexChunk(glChunk* chunk,GLint baseVertex);

GLint applyModelMatrix(mat4 model_matrix);

typedef struct myMesh {
	glChunk* vertexChunk=NULL;
	glChunk* indexChunk=NULL;
	vector<vertex> vertices;
	vector<uint> indices;
	GLuint texture=0;
	myMesh(GLsizeiptr vertexSize,GLsizeiptr indexSize) {
		vertexSize *= sizeof(vertex);
		indexSize *= sizeof(uint);
		vertexChunk = newVertexChunk(vertexSize);
		indexChunk	= newIndexChunk(indexSize);
	}
	void updateVertex() {
		my_glBufferData(vertexChunk, vertices.data());
	}
	void updateIndex() {
		my_glBufferData(indexChunk, indices.data());
	}
	void update() {
		updateVertex();
		updateIndex();
	}
	void render() {
		drawIndexChunk(indexChunk,vertexChunk->offset/sizeof(vertex));
	}
}myMesh;
