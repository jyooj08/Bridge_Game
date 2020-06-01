#include "global.h"
#include "animator.h"
#include "3dObject.h"
#include "logic.h"

//*************************************
// global constants
const char*	window_name = "Bridge";
const char*	vert_shader_path = "../bin/shaders/game.vert";
const char*	frag_shader_path = "../bin/shaders/game.frag";
const char*	shadow_vert_shader_path = "../bin/shaders/shadow_depth.vert";
const char*	shadow_frag_shader_path = "../bin/shaders/shadow_depth.frag";
//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size
//*************************************
// global variables
int		frame = 0;				// index of rendering frames

//*************************************
// scene objects
camera		global_cam;

// control options
bool zoom_tracking = false;
bool padding = false;
bool tb_tracking = false;
bool handle_tracking = false;
bool fps_mode = false;
MouseLock mouse_lock;

// my options

bool wire_mode = false;
uint color_mode = 0;
bool rotate_mode = false;
extern Basic3dObject box2;
extern Basic3dObject lightBall;

//************************************ 
// simple functions

void print_help()
{
	printf("\n\n=== U S A G E ===\n\n");
	printf("ESC or q to close window\n");
	printf("Use mouse to resize window\n");
	printf("F : Toggle FrameMode(WireMode)\n");
	printf("C : Change Color\n");
	printf("Middle Button : zooming\n");
	printf("Left Button : trackball\n");
	printf("Right Button : shift\n");
	printf("WASD SPACE SHIFT+SPACE : camera move");
	printf("\n\n=================\n\n");
}


//*************************************
// call back functions


void reshape( GLFWwindow* window, int width, int height )
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	static double prev_time = glfwGetTime();
	static float camera_speed = 30.0f;
	static animatorID cF=0, cB=0, cR=0, cL=0, cU=0, cD=0;
	double past_time = glfwGetTime() - prev_time;
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_ESCAPE||key==GLFW_KEY_Q)	glfwSetWindowShouldClose( window, GL_TRUE );
		else if(key==GLFW_KEY_H||key==GLFW_KEY_F1)	print_help();
		else if(key==GLFW_KEY_HOME)					global_cam=camera();
		else if (key == GLFW_KEY_C) {
			color_mode++;
			color_mode %= 3;
		}
		else if (key == GLFW_KEY_F) {
			wire_mode = !wire_mode;
			if (wire_mode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		else if (key == GLFW_KEY_R) {
			rotate_mode = !rotate_mode;
		}
		else if (key == GLFW_KEY_1) {
			fps_mode = !fps_mode;
			if (fps_mode) {
				global_cam.startControl();
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
		/*else if (key == GLFW_KEY_UP) {
			box2.translate(vec3(0, 0, -10));
			lightBall.translate(vec3(0, 0, -10));
			global_cam.basicMove(vec3(0, 0, -10));
		}
		else if (key == GLFW_KEY_DOWN) {
			box2.translate(vec3(0, 0, 10));
			lightBall.translate(vec3(0, 0, 10));
			global_cam.basicMove(vec3(0, 0, 10));
		}
		else if (key == GLFW_KEY_RIGHT) {
			box2.translate(vec3(10, 0, 0));
			lightBall.translate(vec3(10, 0, 0));
			global_cam.basicMove(vec3(10, 0, 0));
		}
		else if (key == GLFW_KEY_LEFT) {
			box2.translate(vec3(-10, 0, 0));
			lightBall.translate(vec3(-10, 0, 0));
			global_cam.basicMove(vec3(-10, 0, 0));
		}*/
		else if (key == GLFW_KEY_RIGHT) {
			turn_right();
			global_cam.turn_right();
			lightBall.rotate(vec3(0, 1, 0), -90);
		}
		else if (key == GLFW_KEY_LEFT) {
			turn_left();
			global_cam.turn_left();
			lightBall.rotate(vec3(0, 1, 0), 90);
		}
		else {
			if (key == GLFW_KEY_W && cF == 0) 
				cF = attachAnimator([](double t) {global_cam.relativeMove(vec3(1, 0, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_S && cB == 0) 
				cB = attachAnimator([](double t) {global_cam.relativeMove(vec3(-1, 0, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_A && cL == 0) 
				cL = attachAnimator([](double t) {global_cam.relativeMove(vec3(0, -1, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_D && cR == 0) 
				cR = attachAnimator([](double t) {global_cam.relativeMove(vec3(0, 1, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_SPACE && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)!=GLFW_PRESS && cU == 0) 
				cU = attachAnimator([](double t) {global_cam.relativeMove(vec3(0, 0, 1) * (float)t * camera_speed); });
			if (key == GLFW_KEY_SPACE && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS && cD == 0) 
				cD = attachAnimator([](double t) {global_cam.relativeMove(vec3(0, 0, -1) * (float)t * camera_speed); });
		}
	}
	else if (action == GLFW_RELEASE){
		if (key == GLFW_KEY_W) detachAnimator(cF);
		if (key == GLFW_KEY_S) detachAnimator(cB);
		if (key == GLFW_KEY_A) detachAnimator(cL);
		if (key == GLFW_KEY_D) detachAnimator(cR);
		if (key == GLFW_KEY_SPACE) detachAnimator(cU);
		if (key == GLFW_KEY_SPACE) detachAnimator(cD);
	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	dvec2 pos; glfwGetCursorPos(window,&pos.x,&pos.y);
	bool is_shift_pressed =
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS
		|| glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	bool is_ctrl_pressed =
		glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS
		|| glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

	// zoom 
	if((button==GLFW_MOUSE_BUTTON_LEFT&&!is_shift_pressed&&is_ctrl_pressed)
		||button==GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS) {
			zoom_tracking = true;
		}
	}
	// padding on RIGHT, SHIFT-LEFT
	else if (button == GLFW_MOUSE_BUTTON_LEFT &&is_shift_pressed) {
		if (action == GLFW_PRESS) { padding = true; }
	}
	// track ball LEFT
	else if (button == GLFW_MOUSE_BUTTON_LEFT) {
		global_cam.startControl();
		tb_tracking = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		handle_tracking = true;
	}
	if (action == GLFW_RELEASE) {
		zoom_tracking = false;
		tb_tracking = false;
		handle_tracking = false;
		padding = false;
		mouse_lock = NONE;
	}
}

void motion( GLFWwindow* window, double x, double y ) // y : down , x : right
{
	static dvec2 prev_coord(x, y);
	float dx = float(x - prev_coord.x);
	float dy = float(y - prev_coord.y);
	if (tb_tracking) {
		float s = 0.002f; // sensitivity
		global_cam.trackBallMove(-dx * s,-dy*s, 0);
	}
	if (handle_tracking) {
		float s = 0.002f; // sensitivity
		global_cam.trackBallMove(0,0,((dx>window_size.y/2)?dy:-dy)*s);
	}
	if (zoom_tracking) {
		float ratio = 0.5f;
		vec3 re = global_cam.eye - global_cam.at;
		if (re.length() < 5.0f && dy< 0) {
			// do nothing eye is too close to the object
		}
		else {
			global_cam.zoomMove(float(dy*ratio));
		}
	}
	if (padding) {
		float ratio = 0.5f* 0.001f * (global_cam.eye -global_cam.at).length();
		global_cam.relativeMove(vec3(0,-dx,dy)*ratio);
	}
	if (fps_mode) {
		float s = 0.0002f; // sensitivity
		global_cam.fpsMove(dx * s, 0);
		global_cam.fpsMove(0, dy * s);
	}
	prev_coord = dvec2(x, y);
}
