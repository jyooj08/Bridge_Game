#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "cgmath.h"
#include "global.h"
#include <cstdio>

typedef struct camera
{
	vec3	eye = vec3( 100,0, 100 );
	vec3	at = vec3( 0, 0, 0 );
	vec3	up = vec3( 0, 1,0 );
	mat4	view_matrix = mat4::look_at( eye, at, up );
	void update() {
		//aspect = window_size.x/float(window_size.y);
		view_matrix = mat4::look_at(eye, at, up);
		projection_matrix = mat4::perspective( fovy, aspect, dnear, dfar );
	}
	void trackBallMove(float th, float pi=0.0f,float om=0.0f) { // pi : UD angle, th : LR angle 
		vec3 re = eye - at;
		vec4 e = { re.x,re.y,re.z, 0 };
		vec4 u = { up.x, up.y, up.z, 0 };
		vec3 tby = re.cross(up).normalize();
		vec3 tbx = tby.cross(re).normalize();
		vec3 tbz = tbx.cross(tby).normalize();
		e = mat4::rotate(tby, th) * e;
		u = mat4::rotate(tby, th) * u;
		e = mat4::rotate(tbx, -pi) * e;
		u = mat4::rotate(tbx, -pi) * u;
		e = mat4::rotate(tbz, om) * e;
		u = mat4::rotate(tbz, om) * u;

		eye = at + vec3(e.x, e.y, e.z);
		up = vec3(u.x, u.y, u.z);
	}
	void relativeMove(vec3 d) {
		vec3 f = (at - eye).normalize();
		vec3 u = up.normalize();
		vec3 r = f.cross(u).normalize();
		vec3 s(0, 0, 0);
		s = f * d.x + u * d.z + r * d.y;
		eye += s;
		at += s;
	}
	void basicMove(vec3 d) {
		eye += d;
		at += d;
	}
	void zoomMove(float d) {
		eye += (eye - at).normalize() * d;
	}
	float	fovy = PI/4.0f; // must be in radian
	float	aspect=1.0f;
	float	dnear = 0.1f;
	float	dfar = 10000.0f;
	mat4	projection_matrix;
}camera;

#endif // _CAMERA_H_
