#pragma once
#include <SFML\Graphics.hpp>
#include "Map.h"

class Collidable : public sf::Drawable
{
	protected:
		sf::Sprite m_MainSprite;
		sf::Texture* m_CurrentTexture;
		Map* m_CurrentMap;

	public:
		Collidable();
		void setTexture(sf::Texture* Tex2);
		void linkMap(Map* NewMap);
		void setPosition(sf::Vector2f Pos);
		void setSize(sf::Vector2f Dims);

};