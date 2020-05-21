#ifndef _MY_MESH_H_
#define _MY_MESH_H_

#include "glBufferManager.h"

//typedef struct myMesh myMesh;

struct myMesh {
	glChunk* vertexChunk = NULL;
	glChunk* indexChunk = NULL;
	vector<vertex> vertices;
	vector<uint> indices;
	GLuint texture = 0;
	bool update_flag = false; // check inital udpate
	myMesh(GLsizeiptr vertexSize = 0, GLsizeiptr indexSize = 0) {
		vertexSize *= sizeof(vertex);
		indexSize *= sizeof(uint);
		if (vertexSize > 0 || indexSize > 0) {
			vertexChunk = newVertexChunk(vertexSize);
			indexChunk = newIndexChunk(indexSize);
		}
	}
	void genChunk() {
		vertexChunk = newVertexChunk(vertices.size() * sizeof(vertex));
		indexChunk = newIndexChunk(indices.size() * sizeof(uint));
	}
	void updateVertex() {
		update_flag = true;
		if (vertexChunk == NULL || indexChunk == NULL) genChunk();
		my_glBufferData(vertexChunk, vertices.data());
	}
	void updateIndex() {
		update_flag = true;
		if (vertexChunk == NULL || indexChunk == NULL) genChunk();
		my_glBufferData(indexChunk, indices.data());
	}
	void update() {
		updateVertex();
		updateIndex();
	}
	void render() {
		if (update_flag == false) update();
		drawIndexChunk(indexChunk, vertexChunk->offset / sizeof(vertex));
	}
};

inline myMesh* mergeMesh(myMesh* ma, myMesh* mb) {
	myMesh* newMesh = new myMesh();
	newMesh->vertices = ma->vertices;
	newMesh->vertices.insert(newMesh->vertices.end(),mb->vertices.begin(), mb->vertices.end());
	newMesh->indices = ma->indices;
	for (auto i : mb->indices) {
		newMesh->indices.push_back(i + ma->vertices.size());
	}
	return newMesh;
}

#endif