#include "animator.h"
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include "global.h"

using namespace std;

unordered_map<animatorID,animation> _animatorSet;
static animatorID nextAnimationID = 1;
queue<animatorID> animatorIDBasket;

animatorID attachAnimator(animator_function_t animator_function, double start_time,double run_time) {
	animatorID newAnimationID; 
	if(animatorIDBasket.empty())
		newAnimationID = nextAnimationID++;
	else {
		newAnimationID = animatorIDBasket.front();
		animatorIDBasket.pop();
	}
	_animatorSet.insert(make_pair(newAnimationID,animation(animator_function,start_time,run_time)));
	return newAnimationID;
}

void detachAnimator(animatorID& animationID) {
	if (animationID == 0) return;
	if (_animatorSet.erase(animationID) >= 1) {
		animatorIDBasket.push(animationID);
	}
	else {
		//cout << "WARNING : Non-existing animationID!" << endl;
	}
	animationID = 0;
}

void animateAll() {
	static double prev_time = glfwGetTime();
	double cur_time = glfwGetTime();
	double past_time = cur_time - prev_time;
	for (auto& p : _animatorSet) {
		if (p.second.execute(past_time) == -1) {
			auto fid = p.first;
			cout << "detach animation ! aid : " << fid << endl;
			detachAnimator(fid);
		}
	}
	prev_time = cur_time;
}
