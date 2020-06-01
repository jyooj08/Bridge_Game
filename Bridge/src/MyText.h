#ifndef MY_TEXT_H
#define MY_TEXT_H
#include <string>
#include "cgmath.h"
#include "text.h"
#include "global.h"

using namespace std;

struct MyText {
	string text;
	bool is_raw = false;
	float scale = 1.0f;
	int x, y;
	vec4 color;
	MyText(string str, int _x = 50, int _y = 50, float s = 1.0f,
		vec4 col = vec4(0.5f, 0.8f, 0.2f, 1.0f), bool raw = false) {
		scale = s;
		is_raw = raw;
		text = str;
		x = _x;
		y = _y;
		color = col;
	}
	void render() {
		int _x, _y;
		if (!is_raw) {
			//int l = min(window_size.x, window_size.y);
			_x = (x * window_size.x) / 100;
			_y = (y * window_size.y) / 100;
		}
		else {
			_x = x;
			_y = y;
		}
		float dpi_scale = cg_get_dpi_scale();
		render_text(text, _x, _y, scale, color, dpi_scale);
	}
};
#endif // MY_TEXT_H
