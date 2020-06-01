#pragma once
#include"cgmath.h"
#include"cgut.h"
#include<iostream>
using namespace std;
#define TIME_DELTA 0.5

void init_logic(string m);
void start_game();
void turn_right();
void turn_left();
void render_logic();