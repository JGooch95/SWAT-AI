#pragma once

#include "Object.h"

class Collider
{
public:
	bool AABBBoxCollision(Object* Object1, Object* Object2);
};