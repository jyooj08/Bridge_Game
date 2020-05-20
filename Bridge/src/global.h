#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "trackball.h"	// virtual trackball

//*************************************
// common structures
struct camera
{
	vec3	eye = vec3( 0, 0, 1500 );
	vec3	at = vec3( 0, 0, 0 );
	vec3	up = vec3( 0, 1, 0 );
	mat4	view_matrix = mat4::look_at( eye, at, up );

	float	fovy = PI/4.0f; // must be in radian
	float	aspect=1.0f;
	float	dnear = 0.1f;
	float	dfar = 10000.0f;
	mat4	projection_matrix;
};

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


#endif // _GLOBAL_H_
