#include "logic.h"
#include "3dObject.h"
#include "text.h"

char direction[4] = { 'U','R','D','L' };
int di = 0, mi = 0;
extern bool game_start;
extern int stage;
int mode = 0;
string game_map;
int map_len;
double last_time, now;
double time_delta;

extern Basic3dObject box;
extern Basic3dObject lightBall;
extern camera global_cam;

void init_logic(string m, double t) {
	game_map = m;
	map_len = game_map.length();
	time_delta = t;
	mode = 0;
	di = 0;
	mi = 0;
}


void start_game() {
	// logic setting
	last_time = glfwGetTime();
	cout << "map: " << game_map << endl;
	cout << "TIME_DELTA: " << time_delta << "s" << endl << endl;
}

void turn_right() {
	di++;
	if (di > 3) di = 0;
	cout << "Turn right\n";
	//global_cam.view_matrix = global_cam.view_matrix * mat4::rotate(vec3(0, 1, 0), radians(-90));
}

void turn_left() {
	di--;
	if (di < 0) di = 3;
	cout << "Turn left\n";
}

void render_logic () {
	if (!game_start) return;
	if (mode == 1) {
		cout << "\nWIN GAME\n";
		//render_text("WIN GAME", 100, 100, 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), 1);
		stage += 1;
		game_start = false;
		return;
	}
	if (mode == 2) {
		cout << "\nLOSE GAME\n";
		//box2.translate(vec3(0, -10, 0));
		//render_text("LOSE GAME", 100, 100, 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), 1);
		stage = -1;
		game_start = false;
		return;
	}

	now = glfwGetTime();
	if (mi == map_len - 1) mode = 1;
	else if (mi < map_len - 1 && now - last_time >= time_delta) {
		//move
		switch (direction[di]) {
		case 'U':
			box.translate(vec3(0, 0, -10));
			lightBall.translate(vec3(0, 0, -10));
			global_cam.basicMove(vec3(0, 0, -10));
			break;
		case 'R':
			box.translate(vec3(10, 0, 0));
			lightBall.translate(vec3(10, 0, 0));
			global_cam.basicMove(vec3(10, 0, 0));
			break;
		case 'D':
			box.translate(vec3(0, 0, 10));
			lightBall.translate(vec3(0, 0, 10));
			global_cam.basicMove(vec3(0, 0, 10));
			break;
		case 'L':
			box.translate(vec3(-10, 0, 0));
			lightBall.translate(vec3(-10, 0, 0));
			global_cam.basicMove(vec3(-10, 0, 0));
			break;
		default:
			break;
		}
		cout << "current_move: " << direction[di] << " current_map:" << game_map[mi] << endl;
		if (game_map[mi] != direction[di]) mode = 2;
		mi++;
		last_time = now;
	}
}