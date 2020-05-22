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

	void gPaint(float r = 0.0f, float g = 0.0f, float b = 0.0f,float c = 0.5f) {
		float d = 0.5f / vertices.size();
		float o = 0;
		for (auto& v : vertices) {
			if (o > c && d > 0.0f) d = -d;
			v.norm = vec3(r+o,g+o,b+o);
			o += d;
		}
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

inline void paintMesh(myMesh* m, vec4 color) {
	for (auto& v : m->vertices) {
		v.tex = vec2(color.x, color.y);
	}
}

#endif