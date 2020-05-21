#include "animator.h"
#include <iostream>
#include <queue>
#include <set>
#include "global.h"

using namespace std;

map<animatorID,animator_function_t> _animatorSet;
static animatorID nextAnimationID = 1;
queue<animatorID> animatorIDBasket;

animatorID attachAnimatorFunction(animator_function_t animator_function) {
	animatorID newAnimationID; 
	if(animatorIDBasket.empty())
		newAnimationID = nextAnimationID++;
	else {
		newAnimationID = animatorIDBasket.front();
		animatorIDBasket.pop();
	}
	_animatorSet.insert(make_pair(newAnimationID,animator_function));
	return newAnimationID;

}
void detachAnimatorFunction(animatorID& animationID) {
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
		p.second(past_time);
	}

	prev_time = cur_time;
}
