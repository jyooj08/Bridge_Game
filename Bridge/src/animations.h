#ifndef ANIMATIONOS_H
#define ANIMATIONOS_H
#include "cgmath.h"

vec2 jump_f(float l, float h, float t) {
	float x = l * t;
	float y = h - (x - l / 2) * (x - l / 2);
	return vec2(x, y);
}
vec2 jump_df(float l, float h, double t, double dt) {
	return jump_f(l, h, float(t)) - jump_f(l, h, float(t - dt));
}

#endif