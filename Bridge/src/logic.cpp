#include "logic.h"
#include "3dObject.h"
#include "text.h"
#include "render.h"
#include "BridgeMap.h"
#include "animator.h"
#include <string>

char direction[4] = { 'U','R','D','L' };
int di = 0, mi = 0;
extern bool game_start;
extern int stage;
int mode = 0;
string game_map;
int map_len;
double last_time, now, start_time;
double time_delta;
bool init = false;

extern Basic3dObject box;
extern Basic3dObject lightBall;
extern BridgeMap* bridgeMap;
extern Basic3dObject box;
extern camera global_cam;
extern renderID mapID;
extern renderID boxID;
double TT = 0.2f;

MyText stageText("this is sample text relative position", 25, 30, 1.5f, vec4(1, 1, 1, 1));
renderID textID;



void init_logic(string m, double t) {
	game_map = m;
	map_len = game_map.length();
	time_delta = t;
	mode = 0;
	di = 0;
	mi = 0;
	string text = "Stage " + to_string(stage + 1);
	stageText = MyText(text, 25, 30, 1.5f, vec4(1, 1, 1, 1));
	init = false;
}


void start_game() {
	// logic setting
	last_time = glfwGetTime();
	start_time = last_time;
	//cout << "map: " << game_map << endl;
	//cout << "TIME_DELTA: " << time_delta << "s" << endl << endl;
	textID = put2render(&stageText);
}

void turn_right() {
	di++;
	if (di > 3) di = 0;
	attachAnimator([](double dt) {global_cam.trackBallMove(-PI / 2 * float(dt / TT)); }, 0, TT);
	//cout << "Turn right\n";
	//global_cam.view_matrix = global_cam.view_matrix * mat4::rotate(vec3(0, 1, 0), radians(-90));
}

void turn_left() {
	di--;
	if (di < 0) di = 3;
	//cout << "Turn left\n";
	attachAnimator([](double dt) {global_cam.trackBallMove(PI / 2 * float(dt / TT)); }, 0, TT);

}

void render_logic () {
	TT = time_delta / 4;

	vec3 ov = -global_cam.at + global_cam.eye;
	global_cam.at = box.getPosition();
	global_cam.eye = global_cam.at + ov;

	if (!game_start) return;
	if (mode == 1) {
		//cout << "\nWIN GAME\n";
		printf("WIN GAME\n");
		//render_text("WIN GAME", 100, 100, 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), 1);
		stage += 1;
		game_start = false;
		return;
	}
	if (mode == 2) {
		//cout << "\nLOSE GAME\n";
		printf("LOSE GAME\n");
		//box2.translate(vec3(0, -10, 0));
		//render_text("LOSE GAME", 100, 100, 1.0f, vec4(0.5f, 0.8f, 0.2f, 1.0f), 1);
		stage = -1;
		game_start = false;
		return;
	}

	now = glfwGetTime();
	if (mi == 0 && now - start_time < 2) return;
	if (!init) {
		detachRenderFunction(textID);
		mapID = attachRenderFunction(bind(&BridgeMap::render, bridgeMap));
		boxID = put2render(&box);
		last_time = now;
		init = true;
	}

	if (mi == map_len - 1) mode = 1;
	else if (mi < map_len - 1 && now - last_time >= time_delta) {
		//move
		switch (direction[di]) {
			/*
		case 'U':
			//attachAnimator([](double dt) {
			//	box.rotate(vec3(1,0,0),-PI/2*float(dt/time_delta*4),true);
			//},0,time_delta/4);
			attachAnimator([](double dt) {
				box.translate(vec3(0, 0, -10 * float(dt / (time_delta / 4))));
			}, 0, time_delta / 4);
			lightBall.translate(vec3(0, 0, -10));
			//global_cam.basicMove(vec3(0, 0, -10));
			break;
		case 'R':
			attachAnimator([](double dt) {
				box.translate(vec3(10 * float(dt / (time_delta / 4)), 0, 0));
			}, 0, time_delta / 4);
			//box.translate(vec3(10, 0, 0));
			lightBall.translate(vec3(10, 0, 0));
			//global_cam.basicMove(vec3(10, 0, 0));
			break;
		case 'D':
			attachAnimator([](double dt) {
				box.translate(vec3(0, 0, 10 * float(dt / (time_delta / 4))));
			}, 0, time_delta / 4);
			//box.translate(vec3(0, 0, 10));
			lightBall.translate(vec3(0, 0, 10));
			//global_cam.basicMove(vec3(0, 0, 10));
			break;
		case 'L':
			attachAnimator([](double dt) {
				box.translate(vec3(-10 * float(dt / (time_delta / 4)), 0, 0));
			}, 0, time_delta / 4);
			//box.translate(vec3(-10, 0, 0));
			//lightBall.translate(vec3(-10, 0, 0));
			//global_cam.basicMove(vec3(-10, 0, 0));
			break;

			*/
		case 'U':
			box.translate(vec3(0, 0, -10));
			lightBall.translate(vec3(0, 0, -10));
			break;
		case 'R':
			box.translate(vec3(10, 0, 0));
			lightBall.translate(vec3(10, 0, 0));
			break;
		case 'D':
			box.translate(vec3(0, 0, 10));
			lightBall.translate(vec3(0, 0, 10));
			break;
		case 'L':
			box.translate(vec3(-10, 0, 0));
			lightBall.translate(vec3(-10, 0, 0));
			break;
		default:
			break;
		}
		//cout << "current_move: " << direction[di] << " current_map:" << game_map[mi] << endl;
		if (game_map[mi] != direction[di]) mode = 2;
		mi++;
		last_time = now;
	}
}