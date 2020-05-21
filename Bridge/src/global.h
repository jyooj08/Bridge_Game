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
//*************************************
// window objects
extern GLFWwindow*	window;
extern ivec2		window_size; // initial window size
//*************************************
// OpenGL objects
extern GLuint	program;	// ID holder for GPU program
extern GLuint vertex_buffer;
extern GLuint index_buffer;
extern GLuint vertex_array;
//*************************************
// scene objects
extern camera		cam;
extern trackball	tb;

//*************************************
// global variables
extern int		frame;				// index of rendering frames

// global functions

#endif // _GLOBAL_H_
