#pragma once
#include "global.h"
#include "myMesh.h"
#include "3dObject.h"
#include<iostream>

using namespace std;

typedef char Tile;

class BridgeMap
{	
public:
	const vec3 tile_size=vec3(10.0f,1.0f,10.0f)*0.9f; // 10x1x10 size tile
	vec3 position = vec3(0, 0, 0);
	BridgeMap(string map);
	void loadPath();
	void render();
	void animate(double t);
private:
	Basic3dObject tile;

	vector<Tile> path;
};

