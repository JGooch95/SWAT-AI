#pragma once
#include <SFML\Graphics.hpp>
#include <deque>
#include "AStar.h"
#include "Object.h"
#include "Bar.h"
#include "Weapon.h"


class Character : public Object
{
	private:
		Weapon m_Weapon1;
		Bar m_HealthBar;

		//AI
		std::deque<Node*> m_Path; //Holds the current path
		enum States { SEARCH_SWEEP };
		States m_CurrentState;  

		//Debug lines
		sf::VertexArray m_OrientationLine; //Shows the direction the unit is facing
		sf::VertexArray m_PathLine; //Shows the path the unit is following
		sf::VertexArray m_CollisionLine; //Shows the line used for shooting collisions

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Character(); //Default constructor

		void move(); //Follows the path to the next node
		void update(); //Updates the states of data
		float shoot(); //Shoots the characters gun and returns the damage dealt

		//Getters
		sf::Vector2f getPosition(); 
		float getRotation(); 
		float getHealth(); 
		std::vector<sf::Vector2f> getCollisionLine(float fAngle); //Gets the line used for shooting collisions

		//Setters
		void setHealth(float fLevel); 
		void setGunTexture(sf::Texture* tex2);
		void setPath(std::deque<Node*> newPath); //Sets a new path to follow

		//MAYBE CHANGE =============================================================
		void lookAt(sf::Vector2f position); //Looks towards the position stated.
		bool lazerChecks(std::vector<sf::Vector2f> vEdges); //Checks if the weapons lazer has collided
		// =========================================================================
};