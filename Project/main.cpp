#include"cgmath.h"
#include"cgut.h"
#include<iostream>
using namespace std;
#define TIME_DELTA 0.5

ivec2 window_size = cg_default_window_size();

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

	void update() {
		if (game_mode == 0) return;
		if (game_mode == 2) {
			cout << "WIN GAME" << endl;
			game_mode = 0;
			return;
		}
		if (game_mode == 3) {
			cout << "LOSE GAME" << endl;
			game_mode = 0;
			return;
		}

		now = glfwGetTime();
		if (mi == map_len) game_mode = 2;
		else if (mi < map_len && now - last_time >= TIME_DELTA) {
			cout << "current map: " << map[mi] << " current_move: " << direction[di] << endl;
			if (map[mi] != direction[di]) game_mode = 3;
			mi++;
			last_time = now;
		}
	}
};

Logic logic("uuuurruu");

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void reshape(GLFWwindow* window, int width, int height);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetKeyCallback(window, keyboard);

	// logic setting
	logic.last_time = glfwGetTime();
	cout << "map: " << logic.map << endl;
	cout << "TIME_DELTA: " << TIME_DELTA << "s" << endl << endl;
	logic.game_mode = 1;

	while (!glfwWindowShouldClose(window))
	{
		logic.update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void reshape(GLFWwindow* window, int width, int height) {
	window_size.x = width; window_size.y = height;
	glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
			glfwSetWindowShouldClose(window, true);
		else if (key == GLFW_KEY_RIGHT) {
			logic.di++;
			if (logic.di > 3) logic.di = 0;
			cout << "Turn right" << endl;
		}
		else if (key == GLFW_KEY_LEFT) {
			logic.di--;
			if (logic.di < 0) logic.di = 3;
			cout << "Turn left" << endl;
		}
	}
}