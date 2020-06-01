#ifndef _CREATURE_H_
#define _CREATURE_H_

#include "3dObject.h"
#include "camera.h"

class Creature : public Basic3dObject{
private:
	camera camera;
public:
	void move(vec3 direction) {
		
	}
	int b;
};

#endif // _CREATURE_H_