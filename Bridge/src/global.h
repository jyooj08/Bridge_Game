#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "trackball.h"	// virtual trackball
#include "camera.h"
#include "glBufferManager.h"

//*************************************
// common structures

// EXTERN VARIABLES

//*************************************
// global constants
extern const char*	window_name;
extern const char*	vert_shader_path;
extern const char*	frag_shader_path;
extern const char*	shadow_vert_shader_path;
extern const char*	shadow_frag_shader_path;
//*************************************
// window objects
extern GLFWwindow*	window;
extern ivec2		window_size; // initial window size
//*************************************
// OpenGL objects
extern GLuint	program;	// ID holder for GPU program
extern GLuint	shadow_program;
extern GLuint vertex_buffer;
extern GLuint index_buffer;
extern GLuint vertex_array;
//*************************************
// scene objects
extern camera		global_cam;

//*************************************
// global variables
extern int		frame;				// index of rendering frames

// control options
extern bool zoom_trackin;
extern bool padding ;
extern bool tb_tracking ;
extern bool handle_tracking ;
extern bool fps_mode;
enum MouseLock { NONE=0, Y_AXIS, X_AXIS };
extern MouseLock mouse_lock;

// my options

extern bool wire_mode;
extern uint color_mode;
extern bool rotate_mode;

//************************************ 
// simple functions

void print_help();

//*************************************
// call back functions

void reshape(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse(GLFWwindow* window, int button, int action, int mods);
void motion(GLFWwindow* window, double x, double y);
#endif // _GLOBAL_H_
