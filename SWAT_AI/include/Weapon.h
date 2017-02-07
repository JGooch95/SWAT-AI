#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"

class Weapon : public Object
{
	private:
		sf::VertexArray m_AimLine;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Weapon();
		void shoot();
		void aim(sf::Vector2f Location);
		float LazerLength(sf::FloatRect ColliderRect);
};