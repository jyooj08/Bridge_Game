#ifndef _GL_BUFFER_MANAGER_H_
#define _GL_BUFFER_MANAGER_H_

#include "global.h"
#include <vector>
#include <iostream>
using namespace std;

#define MAX_GL_BUFFER_SIZE		((size_t)5000000000)	//500MB
#define DEFAULT_GL_BUFFER_SIZE	((size_t)100000000 )	//10MB

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
GLint applyCamera(camera arg_cam);
GLint setUniformVariable(std::string var_name, uint value);


#endif
