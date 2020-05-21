#pragma once
#include "global.h"
#include "myMesh.h"

using namespace std;

typedef char Tile;


class BridgeMap
{	
public:
	const vec3 tile_size=vec3(10.0f,10.0f,1.0f); // 10x10x1 size tile
	BridgeMap();
	void loadPath();
	void render();
private:
	myMesh* tileMesh;
	glChunk* tileVertexChunk = NULL;
	glChunk* tileIndexChunk = NULL;
	vector<vertex>	tileVertices;
	vector<uint>	tileIndices;

	vector<Tile> path;
};

