#include "BridgeMap.h"
#include "meshFactory.h"



BridgeMap::BridgeMap() {
	// sample path // TODO : delete sample
	Tile sample_path[] = "UULLUUURR";
	for (auto t : sample_path) {
		if (t == '\0') break;
		path.push_back(t);
	}

	// vertex & index 
	tileMesh = generateBoxMesh(tile_size);
	// texture color 
	for (auto& v : tileMesh->vertices) {
		static vec2 col = { 0.0f,0.0f };
		col += {0.1f,0.1f};
		v.tex = col;
	}
}

void BridgeMap::render() {
	float size = 1.0f;
	mat4 model = mat4::scale(size);
	applyModelMatrix(model);
	for (size_t i = 0; i < path.size(); i++) {
		applyModelMatrix(model);
		tileMesh->render();

		// make model matrix for next tile
		while (path[i] == 'J') {
			model *= mat4::translate(0,0,5.0f);
			i++;
		}
		switch (path[i]) {
		case 'U':
			model *= mat4::translate(tile_size.x, 0, 0);
			break;
		case 'D':
			model *= mat4::translate(-tile_size.x, 0, 0);
			break;
		case 'L':
			model *= mat4::translate(0, -tile_size.y, 0);
			break;
		case 'R':
			model *= mat4::translate(0, +tile_size.y, 0);
			break;
		}
	}
}
