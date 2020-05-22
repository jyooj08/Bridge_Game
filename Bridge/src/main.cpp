#include "global.h"
#include "render.h"
#include "animator.h"
#include "meshFactory.h"
#include "glBufferManager.h"
#include "BridgeMap.h"
#include "camera.h"
#include "myMesh.h"
#include <vector>
#include <iostream>
using namespace std;

bool zoom_tracking = false;
bool padding = false;
bool tb_tracking = false;
bool handle_tracking = false;
enum{NONE,Y_AXIS,X_AXIS} mouse_lock;

//*************************************

myMesh* sphereMesh;
BridgeMap* bridgeMap;
myMesh* directorMesh;

Basic3dObject* room;

bool wire_mode = false;
uint color_mode = 0;
bool rotate_mode = false;



//*************************************
void update()
{
	// update projection matrix
	setUniformVariable("color_mode", color_mode);

	applyCamera(cam);
}

class Tmp {

public:
void render(int a)
{
	for (int i = a; i < a+1; i++) {
		mat4 model_matrix = mat4::scale(1.0f) *
			mat4::translate(cam.at);
		applyModelMatrix(model_matrix);
		sphereMesh->render();
	}
}
};


void reshape( GLFWwindow* window, int width, int height )
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

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
		else if(key==GLFW_KEY_HOME)					cam=camera();
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
		else {
			if (key == GLFW_KEY_W && cF == 0) 
				cF = attachAnimatorFunction([](double t) {cam.relativeMove(vec3(1, 0, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_S && cB == 0) 
				cB = attachAnimatorFunction([](double t) {cam.relativeMove(vec3(-1, 0, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_A && cL == 0) 
				cL = attachAnimatorFunction([](double t) {cam.relativeMove(vec3(0, -1, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_D && cR == 0) 
				cR = attachAnimatorFunction([](double t) {cam.relativeMove(vec3(0, 1, 0) * (float)t * camera_speed); });
			if (key == GLFW_KEY_SPACE && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)!=GLFW_PRESS && cU == 0) 
				cU = attachAnimatorFunction([](double t) {cam.relativeMove(vec3(0, 0, 1) * (float)t * camera_speed); });
			if (key == GLFW_KEY_SPACE && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS && cD == 0) 
				cD = attachAnimatorFunction([](double t) {cam.relativeMove(vec3(0, 0, -1) * (float)t * camera_speed); });
		}
	}
	else if (action == GLFW_RELEASE){
		if (key == GLFW_KEY_W) detachAnimatorFunction(cF);
		if (key == GLFW_KEY_S) detachAnimatorFunction(cB);
		if (key == GLFW_KEY_A) detachAnimatorFunction(cL);
		if (key == GLFW_KEY_D) detachAnimatorFunction(cR);
		if (key == GLFW_KEY_SPACE) detachAnimatorFunction(cU);
		if (key == GLFW_KEY_SPACE) detachAnimatorFunction(cD);
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

void motion( GLFWwindow* window, double x, double y )
{
	static dvec2 padding_past(x, y);
	float dx = float(x - padding_past.x);
	float dy = float(y - padding_past.y);
	if (tb_tracking) {
		float s = 0.002f; // sensitivity
		if (mouse_lock == NONE || abs(dy/dx) >10 || abs(dx/dy) >10 ) {
			if (abs(dy) > abs(dx)) {
				mouse_lock = Y_AXIS;
			}
			else {
				mouse_lock = X_AXIS;
			}
		}
		if(mouse_lock == Y_AXIS)
			cam.trackBallMove(dy * s, 0);
		else if(mouse_lock == X_AXIS)
			cam.trackBallMove(0, dx*s);
	}
	if (handle_tracking) {
		float s = 0.002f; // sensitivity
		cam.trackBallMove(0,0,((dx>window_size.y/2)?dy:-dy)*s);
	}
	if (zoom_tracking) {
		float ratio = 0.5f;
		vec3 re = cam.eye - cam.at;
		if (re.length() < 5.0f && dy< 0) {
			// do nothing eye is too close to the object
		}
		else {
			cam.zoomMove(float(dy*ratio));
		}
	}
	if (padding) {
		float ratio = 0.5f* 0.001f * (cam.eye -cam.at).length();
		cam.relativeMove(vec3(0,-dx,dy)*ratio);
	}
	padding_past = dvec2(x, y);
}

bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	//glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests

	// generate vertex_array vertex_bufer vertex_indices
	my_glBufferInit();
	sphereMesh = generateSphereMesh();
	bridgeMap = new BridgeMap();

	// generate director mesh
	myMesh* x = generateBoxMesh(vec3(10,0.5,0.5));
	myMesh* y = generateBoxMesh(vec3(0.5,10,0.5));
	myMesh* z = generateBoxMesh(vec3(0.5,0.5,10));
	x->paint(1,0,0);
	y->paint(0,1,0);
	z->paint(0,0,1);
	directorMesh = mergeMesh(mergeMesh(x, y),z);
	attachRenderFunction([]() {								// render camera director
		mat4 model_matrix = mat4::translate(cam.at) *
			mat4::scale((cam.at - cam.eye).length()/200);
		applyModelMatrix(model_matrix);
		directorMesh->render();});

	// generate room mesh & object
	static myMesh* roomMesh = generateBoxMesh(vec3(100));
	attachAnimatorFunction([](double t) {
		float ct = float(glfwGetTime());
		ct = ct - int(ct / 1);
		roomMesh->gPaint(.3f,.2f,.2f);
		roomMesh->update();
	});
	roomMesh->gPaint(1.0f,.5f);
	room = new Basic3dObject(roomMesh);
	room->setOrigin(50, 50, 50);
	room->scale(30);
	attachRenderFunction([]() {room->render(); });

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// register render functions
	attachRenderFunction(bind(&BridgeMap::render, bridgeMap));


	// attach frame counter
	attachAnimatorFunction([](double t) {
		static int past_frame = frame;
		static double fps = double((frame - past_frame) / t);
		const double a = 0.006f;
		fps = (frame - past_frame) / t * a + fps * (1 - a);
		printf("\r fps : %-4d frame_count : %d", (int)fps,frame);
		past_frame = frame;
	});

	// attach bridgeMap animation
	attachAnimatorFunction([](double t) {bridgeMap->animate(t); });

	return true;
}

void user_finalize()
{
}

int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }						// user initialization


	// enters rendering/event loop
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events

		// NOTICE : update() and animateAll();
		// object which is ATTACHED on animated object should update position after animateAll() !!!
		animateAll();
		update();			

		renderAll();			// per-frame render
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}
