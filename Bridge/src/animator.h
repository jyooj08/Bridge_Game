#pragma once

#include <functional>

using namespace std;

typedef function<void(double)> animator_function_t;
typedef unsigned long long animatorID;

animatorID attachAnimatorFunction(animator_function_t animate_function);
void detachAnimatorFunction(animatorID& animationID);

void animateAll();

