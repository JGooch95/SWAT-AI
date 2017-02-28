#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "MathUtils.h"

class Weapon : public Object
{
	private:
		sf::VertexArray m_AimLine;
		sf::Vector2f m_LazerIntersect;
		Util::Math Calculator;
		sf::Vector2f LazerPoint;
		sf::Clock FireRateClock;
		float FireRate;
		float Damage;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Weapon();
		float shoot();
		void aim(sf::Vector2f Location);
		sf::Vector2f calcLazerIntersect(sf::Vector2f a, sf::Vector2f b);
		void setIntersect(sf::Vector2f Vect);
		sf::Vector2f getIntersect();

};