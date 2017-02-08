#pragma once

#include <SFML\Graphics.hpp>

namespace Util
{
	class Math
	{
	public:
		float cross(sf::Vector2f a, sf::Vector2f b)
		{
			return (a.x * b.y - a.y * b.x);
		}
	};
}