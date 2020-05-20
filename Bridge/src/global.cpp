#include "global.h"

//*************************************
// global constants
const char*	window_name = "Bridge";
const char*	vert_shader_path = "../bin/shaders/game.vert";
const char*	frag_shader_path = "../bin/shaders/game.frag";
//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size
//*************************************
// global variables
int		frame = 0;				// index of rendering frames

//*************************************
// scene objects
camera		cam;
trackball	tb;
