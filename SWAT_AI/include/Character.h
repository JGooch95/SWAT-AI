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
		sf::VertexArray m_OrientationLine;
		sf::VertexArray m_PathLine;
		Weapon m_Weapon1;
		Bar m_HealthBar;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Character();
		void Move();
		void setPath(std::deque<Node*> NewPath);
		void lookAt(sf::Vector2f Position);
		void update();
		void setGunTexture(sf::Texture* Tex2);
		sf::Vector2f getPosition();
		std::deque<Node*> Path;
};