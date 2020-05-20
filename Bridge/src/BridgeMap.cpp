#include "BridgeMap.h"


BridgeMap::BridgeMap() {
	// sample path // TODO : delete sample
	Tile sample_path[] = "UUULULLRRURURU";
	for (auto t : sample_path) {
		if (t == '\0') break;
		path.push_back(t);
	}

	// vertex & index // TODO change vertices. It should be a cube vertices
	boxVertices = {
		{{0,0,0},{0,0,0},{0,0}},
		{{10,0,0},{0,0,0},{0,0}},
		{{0,0,0},{0,0,0},{0,0}},
		{{0,0,0},{0,0,0},{0,0}},
		{{0,0,0},{0,0,0},{0,0}},
		{{0,0,0},{0,0,0},{0,0}},
		{{0,0,0},{0,0,0},{0,0}},
		{{0,0,0},{0,0,0},{0,0}},
	};
	// TODO : add box indices
	boxIndices = {};

	// buffer resource for tile
	// TODO : fix this
	//glChunk* defaultVertexChunk = newVertexChunk();
}

void BridgeMap::render() {

}
