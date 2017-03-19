#pragma once
#include "Object.h"

class Button : public Object
{
	public:
		Button();
		bool clicked(sf::Vector2i pos);
};
