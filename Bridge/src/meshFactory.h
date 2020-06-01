#pragma once

// default box
#include <vector>
#include "glBufferManager.h"
#include "cgmath.h"
#include "myMesh.h"

using namespace std;


// Default Face Meshes

static const vector<uint>	_face_indices = { 0,1,2,0,1,3, };

static const vector<vertex> _bottom_vertices = {
		{{0,0,0},{0,0,-1},{0,0}},	
		{{1,1,0},{0,0,-1},{0,0}},
		{{1,0,0},{0,0,-1},{0,0}},	
		{{0,1,0},{0,0,-1},{0,0}},
};
static const vector<vertex> _left_vertices = {
		{{0,0,0},{0,-1,0},{0,0}},
		{{1,0,1},{0,-1,0},{0,0}},
		{{1,0,0},{0,-1,0},{0,0}},
		{{0,0,1},{0,-1,0},{0,0}},
};
static const vector<vertex> _right_vertices = {
		{{0,1,0},{0,1,0},{0,0}},
		{{1,1,1},{0,1,0},{0,0}},
		{{1,1,0},{0,1,0},{0,0}},
		{{0,1,1},{0,1,0},{0,0}},
};
static const vector<vertex> _front_vertices = {
		{{0,0,0},{-1,0,0},{0,0}},
		{{0,1,1},{-1,0,0},{0,0}},
		{{0,0,1},{-1,0,0},{0,0}},
		{{0,1,0},{-1,0,0},{0,0}},
};
static const vector<vertex> _back_vertices = {
		{{1,0,0},{1,0,0},{0,0}},
		{{1,1,1},{1,0,0},{0,0}},
		{{1,1,0},{1,0,0},{0,0}},
		{{1,0,1},{1,0,0},{0,0}},
};
static const vector<vertex> _top_vertices = {
		{{0,0,1},{0,0,1},{0,0}},
		{{1,1,1},{0,0,1},{0,0}},
		{{1,0,1},{0,0,1},{0,0}},
		{{0,1,1},{0,0,1},{0,0}},
};

inline Mesh* generateRectMesh(vec3 scale = vec3(1, 1, 1)) {
	// TODO : not completely implemented
	static const vector<vertex> _base_rect_vertices = {
		{{0,0,0},{0,0,-1},{0,0}},	
		{{1,1,0},{0,0,-1},{0,0}},
		{{1,0,0},{0,0,-1},{0,0}},	
		{{0,1,0},{0,0,-1},{0,0}},
	};
	vector<vertex> _ret_rect_vertices;
	return nullptr;
}



// Default Box Mesh 

static const vector<vertex> _box_vertices = {
		// bottom
		{{0,0,0},{0,0,-1},{0,0}},	// 0 
		{{1,1,0},{0,0,-1},{0,0}},	// 2
		{{1,0,0},{0,0,-1},{0,0}},	// 1
		{{0,1,0},{0,0,-1},{0,0}},	// 3
		// left
		{{0,0,0},{0,-1,0},{0,0}},	// 4 
		{{1,0,1},{0,-1,0},{0,0}},	// 5
		{{1,0,0},{0,-1,0},{0,0}},	// 6
		{{0,0,1},{0,-1,0},{0,0}},	// 7
		// right
		{{0,1,0},{0,1,0},{0,0}},	// 8
		{{1,1,1},{0,1,0},{0,0}},	// 9 
		{{1,1,0},{0,1,0},{0,0}},	// 10
		{{0,1,1},{0,1,0},{0,0}},	// 11
		// front
		{{0,0,0},{-1,0,0},{0,0}},	// 12 
		{{0,1,1},{-1,0,0},{0,0}},	// 13
		{{0,0,1},{-1,0,0},{0,0}},	// 14
		{{0,1,0},{-1,0,0},{0,0}},	// 15
		// back
		{{1,0,0},{1,0,0},{0,0}},	// 16
		{{1,1,1},{1,0,0},{0,0}},	// 17
		{{1,1,0},{1,0,0},{0,0}},	// 18
		{{1,0,1},{1,0,0},{0,0}},	// 19
		// top
		{{0,0,1},{0,0,1},{0,0}},	// 20
		{{1,1,1},{0,0,1},{0,0}},	// 21
		{{1,0,1},{0,0,1},{0,0}},	// 22
		{{0,1,1},{0,0,1},{0,0}},	// 23
};

static const vector<uint> _box_indices = {
	// bottom
	 0, 1, 2, 3, 1, 0,
	// left
	 6, 5, 4, 4, 5, 7,
	// right
	 8, 9,10, 11, 9,8,
	// front
	12,13,15,14,13,12,
	// back
	18,17,16,16,17,19,
	// top
	22,21,20,20,21,23,
};

inline struct Mesh* generateBoxMesh(vec3 scale=vec3(1,1,1)) {
	Mesh* newMesh = new Mesh();
	newMesh->vertices = _box_vertices;
	// scale 
	for (auto& v: newMesh->vertices) {
		vec4 t = (mat4::scale(scale) * vec4(v.pos, 1));
		v.pos = vec3(t.x, t.y, t.z);
	}
	newMesh->indices = _box_indices;
	//newMesh->update();
	return newMesh;
}


// Default Sphere Mesh 

static vector<vertex> _sphere_vertices;
static vector<uint>	  _sphere_indices;

inline struct Mesh* generateSphereMesh(vec3 scale=vec3(1,1,1),uint cN=50,uint sN=30) {
	if (_sphere_vertices.size() < cN * sN) {
		vector<vertex>& sphere_vertices = _sphere_vertices;
		vector<uint>&	sphere_indices	= _sphere_indices;
		for (uint h = 0; h <= sN; h++) {
			float t = ((float)PI / sN) * (float)h;
			for (uint k = 0; k <= cN; k++) {
				float pi = (2.0f * (float)PI / cN) * (float)k;
				vec3 norm = { sin(t) * cos(pi),sin(t) * sin(pi),cos(t) };
				sphere_vertices.push_back({ norm, norm.normalize(), vec2((float)k / cN, 1 - (float)h / sN) });
			}
		}
		for (uint h = 0; h < sN; h++) {
			for (uint k = 0; k < cN; k++) {
				int o = (cN+1) * h + k;
				sphere_indices.push_back(o);
				sphere_indices.push_back(o + cN+1);
				sphere_indices.push_back(o + cN + 2);
				sphere_indices.push_back(o);
				sphere_indices.push_back(o + cN + 2);
				sphere_indices.push_back(o + 1);
			}
		}
	}
	Mesh* newMesh = new Mesh();
	newMesh->vertices = _sphere_vertices;
	newMesh->indices = _sphere_indices;
	return newMesh;
}

