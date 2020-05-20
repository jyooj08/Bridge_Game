#include"cgmath.h"
#include"cgut.h"
#include<iostream>
using namespace std;
#define TIME_DELTA 1

ivec2 window_size = cg_default_window_size();
char current_move;
//up -> right -> down -> left
char direction[] = { 'u','r','d','l' };
int di = 0;

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

	string map = "uuuurruu";
	double last_time = glfwGetTime();
	int idx = 0;
	int map_len = map.length();
	current_move = map[0];

	cout << "map: " << map << endl;
	cout << "TIME_DELTA: " << TIME_DELTA << "s" << endl << endl;

	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		current_move = direction[di];
		//cout << "now: " << now << endl;
		if (idx < map_len && now - last_time >= TIME_DELTA) {
			cout << "current map: " << map[idx] << " current_move: " << current_move << endl;
			if (map[idx] != current_move) {
				cout << "Lose Game...." << endl;
				idx = map_len;
			}
			idx++;
			last_time = now;
		}
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
			di++;
			if (di > 3) di = 0;
			cout << "direction change: " << direction[di] << endl;
		}
		else if (key == GLFW_KEY_LEFT) {
			di--;
			if (di < 0) di = 3;
			cout << "direction change: " << direction[di] << endl;
		}
	}
}