#pragma once

#include <functional>
#include "global.h"

using namespace std;

typedef function<void(double)> animator_function_t;
typedef function<void(double,double)> animator_function_t2; // 2args

struct animation {
	animation(animator_function_t f,double st=0,double rt=0) {
		function = f;
		start_time = st;
		run_time = rt;
	}
	animation(animator_function_t2 f,double st=0,double rt=0) {
		function2 = f;
		start_time = st;
		run_time = rt;
	}
	animator_function_t function=NULL;
	animator_function_t2 function2=NULL;

	void stop()  { state = STOP; }
	void restart() { state = RUN; }
	void start() { state = RUN; start_time = 0; }
	int execute(double dt) { // delta time
		if (run_time < 0.0) {
			return -1;		// expire animation
		}
		if (state == STOP) {
			return 0;
		}
		else if (state == RUN) {
			if (start_time == 0 || glfwGetTime()>= start_time) {
				// excute function
				pt += dt;
				if (function != NULL) function(dt);
				else if (function2 != NULL) function2(dt, pt); // past_time argument
				else cout << "[ERR] animation::excute() : animator funciton is NULL!" << endl;
			}
			else {
				return 0;		// wait more
			}
			if (run_time > 0.0) {
				run_time -= dt;	// do not reduce run_time when run_time == 0( runtime is infinite)
				if (run_time == 0) run_time = -12345.0f; // some times run_time can be 0
			}
		}
		return 1;
	}

	enum { RUN, STOP } state=RUN;
	double start_time=0;
	double run_time  =0;
	double pt = 0;
};

typedef unsigned long long animatorID;

animatorID attachAnimator(animator_function_t animate_function,double start_time =0,double run_time =0);
void detachAnimator(animatorID& animationID);

void animateAll();

