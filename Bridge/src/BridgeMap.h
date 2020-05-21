#pragma once
#include "global.h"
#include "glBufferManager.h"

using namespace std;

typedef char Tile;

class BridgeMap
{	
public:
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

