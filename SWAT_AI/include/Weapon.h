#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "MathUtils.h"

class Weapon : public Object
{
	private:
		sf::VertexArray m_AimLine; //Holds the lazer pointer line
		sf::Vector2f m_LazerPoint; //Where the lazer is pointed
		sf::Clock m_FireRateClock; //Holds how long its been since firing the weapon
		float m_fFireRate; //How many shots can be fired per second
		float m_fDamage; //How much damage is done per shot

		Util::Math Calculator;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Weapon(); //Constructor
		float shoot(); //Shoots the weapon if ready
		void aim(sf::Vector2f location); //Aims towards the location given
		sf::Vector2f calcLazerIntersect(sf::Vector2f a, sf::Vector2f b); //Calculates where the lazer collides
		sf::Vector2f getIntersect(); //Returns where the lazer has intersected
		void setIntersect(sf::Vector2f vect); //Sets the lines intersect

};