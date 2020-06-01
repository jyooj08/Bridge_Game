#ifndef _MY_MESH_H_
#define _MY_MESH_H_

#include "glBufferManager.h"

//typedef struct myMesh myMesh;

struct Mesh {
	glChunk* vertexChunk = NULL;
	glChunk* indexChunk = NULL;
	vector<vertex> vertices;
	vector<uint> indices;
	GLuint texture = 0;
	bool update_flag = false; // check inital udpate
	bool auto_free = false;
	Mesh(GLsizeiptr vertexSize = 0, GLsizeiptr indexSize = 0) {
		vertexSize *= sizeof(vertex);
		indexSize *= sizeof(uint);
		if (vertexSize > 0 || indexSize > 0) {
			vertexChunk = newVertexChunk(vertexSize);
			indexChunk = newIndexChunk(indexSize);
		}
	}
	~Mesh() {
		if (auto_free) {
			// TODO free chunk
		}
		if (vertexChunk != NULL) {
			cout << "vertexChunk[" << vertexChunk << "]is not free!" << endl;
		}
		if (vertexChunk != NULL) {
			cout << "indexChunk[" << indexChunk << "]is not free!" << endl;
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
	void paint(vec4 color) {
		return;
		for (auto& v : vertices) {
			v.norm = vec3(color.x, color.y,color.z);
		}
	}
	void flipNormal() {
		for (auto& v : vertices) {
			v.norm *= -1.0f;
		}
	}
	void newPaint() {
		paint(vec4(0.5f));
	}

	// convenient functions
	void paint(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
		paint(vec4(r, g, b, a)); }
};

inline Mesh* mergeMesh(Mesh* ma, Mesh* mb) {
	Mesh* newMesh = new Mesh();
	newMesh->vertices = ma->vertices;
	newMesh->vertices.insert(newMesh->vertices.end(),mb->vertices.begin(), mb->vertices.end());
	newMesh->indices = ma->indices;
	for (auto i : mb->indices) {
		newMesh->indices.push_back(i + ma->vertices.size());
	}
	return newMesh;
}

inline void paintMesh(Mesh* m, vec4 color) {
	for (auto& v : m->vertices) {
		v.tex = vec2(color.x, color.y);
	}
}

#endif