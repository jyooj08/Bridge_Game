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
	glChunk* defaultVertexChunk = NULL;
	glChunk* defaultIndexChunk = NULL;
	vector<vertex> boxVertices;
	vector<uint> boxIndices;
	vector<Tile> path;
};

