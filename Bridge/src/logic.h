#pragma once
#include"cgmath.h"
#include"cgut.h"
#include<iostream>
using namespace std;
#define TIME_DELTA 1

class Logic {
public:
	char direction[4];
	int di = 0, mi = 0;
	int game_mode = 0; // 0: before start, 1: during game, 2: win game, 3: lose game
	string map;
	int map_len;
	double last_time, now;

	Logic(string m) {
		direction[0] = 'u'; direction[1] = 'r'; direction[2] = 'd'; direction[3] = 'l';
		map = m;
		map_len = map.length();
	}

	void start_game() {
		// logic setting
		last_time = glfwGetTime();
		cout << "map: " << map << endl;
		cout << "TIME_DELTA: " << TIME_DELTA << "s" << endl << endl;
		game_mode = 1;
	}

	void turn_right() {
		di++;
		if (di > 3) di = 0;
		cout << "\nTurn right\n";
	}

	void turn_left() {
		di--;
		if (di < 0) di = 3;
		cout << "\nTurn left\n";
	}

	void update() {
		if (game_mode == 0) return;
		if (game_mode == 2) {
			cout << "\nWIN GAME\n";
			game_mode = 0;
			return;
		}
		if (game_mode == 3) {
			cout << "\nLOSE GAME\n";
			game_mode = 0;
			return;
		}

		now = glfwGetTime();
		if (mi == map_len) game_mode = 2;
		else if (mi < map_len && now - last_time >= TIME_DELTA) {
			cout << "\ncurrent map: " << map[mi] << " current_move: " << direction[di] << endl;
			if (map[mi] != direction[di]) game_mode = 3;
			mi++;
			last_time = now;
		}
	}
};