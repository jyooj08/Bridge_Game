#include "BridgeMap.h"
#include "meshFactory.h"



BridgeMap::BridgeMap() {
	// sample path // TODO : delete sample
	Tile sample_path[] = "UULLJUUUJRRUULULJLUURRURJJURRRURJJLJLU";
	for (auto t : sample_path) {
		if (t == '\0') break;
		path.push_back(t);
	}

	// 3D Object setting
	tile.setMesh(generateBoxMesh(tile_size));
	tile.getMesh()->paint(0, 1, 0);
	//tile.getMesh()->gPaint(1,0,0);
	tile.setOrigin(vec3(5,0.5f,5));
}

void BridgeMap::render() {
	tile.setPosition(position);
	for (size_t i = 0; i < path.size(); i++) {
		tile.render();

		while (path[i] == 'J') {
			tile.translate(vec3(0,5.0f,0));
			i++;
		}
		switch (path[i]) {
		case 'U':
			tile.translate(vec3(0,0,10.0f));
			break;
		case 'D':
			tile.translate(vec3(0,0,-10.0f));
			break;
		case 'L':
			tile.translate(vec3(-10.0f,0,0));
			break;
		case 'R':
			tile.translate(vec3(10.0f,0,0));
			break;
		}
	}
}

void BridgeMap::animate(double t) {
	tile.rotate((float)t);
}
