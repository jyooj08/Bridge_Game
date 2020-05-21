#include "BridgeMap.h"


BridgeMap::BridgeMap() {
	// sample path // TODO : delete sample
	Tile sample_path[] = "UUULULLUURRURURURRUUULLUURRUUUULULLUURRURURURRUUULLUURRUUUULULLUURRURURURRUUULLUURRUUUULULLUURRURURURRUUULLUURRU";
	for (auto t : sample_path) {
		if (t == '\0') break;
		path.push_back(t);
	}

	// vertex & index 
	tileMesh = new myMesh(8,36);
	float h = 1.0f;
	tileMesh->vertices= {
		{{0,0,0},{0,0,0},{0,0}},	// 0 
		{{10,0,0},{0,0,0},{0,0}},	// 1
		{{10,10,0},{0,0,0},{0,0}},	// 2
		{{0,10,0},{0,0,0},{0,0}},	// 3
		{{0,0,h},{0,0,0},{0,0}},	// 4
		{{10,0,h},{0,0,0},{0,0}},	// 5
		{{10,10,h},{0,0,0},{0,0}},	// 6
		{{0,10,h},{0,0,0},{0,0}},	// 7
	};
	// texture color 
	for (auto& v : tileMesh->vertices) {
		static vec2 col = { 0.0f,0.0f };
		col += {0.1f,0.1f};
		v.tex = col;
	}

	tileMesh->indices = {
		// bottom
		0,1,2,2,3,0,
		// left
		0,1,5,5,4,0,
		// right
		3,2,6,6,7,3,
		// front
		0,3,4,4,7,3,
		// back
		1,2,6,6,5,1,
		// top
		4,7,6,6,5,4
	};
	tileMesh->update();
}

void BridgeMap::render() {
	float size = 1.0f;
	mat4 model = mat4::scale(size);
	applyModelMatrix(model);
	tileMesh->render();
	for (size_t i = 1; i < path.size(); i++) {
		switch (path[i - 1]) {
		case 'U':
			model *= mat4::translate(10.0f, 0, 0);
			break;
		case 'D':
			model *= mat4::translate(-10.0f, 0, 0);
			break;
		case 'L':
			model *= mat4::translate(0, -10.0f, 0);
			break;
		case 'R':
			model *= mat4::translate(0, +10.0f, 0);
			break;
		}
		applyModelMatrix(model);
		tileMesh->render();
	}
}
