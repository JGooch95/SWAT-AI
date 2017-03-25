#pragma once

#include <SFML\Graphics.hpp>
#include "Map.h"
#include "Settings.h"

class Object : public sf::Drawable
{
	protected:
		sf::RectangleShape m_MainSprite; //Holds the sprite
		const sf::Texture* m_CurrentTexture; // Holds a pointer to the texture
		Map* m_CurrentMap; //Holds the data for the map
		Settings* m_CurrentSettings;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Object();

		//Setters
		void setTexture(sf::Texture* tex2);
		void setPosition(sf::Vector2f pos);
		void setOrigin(sf::Vector2f pos);
		void setSize(sf::Vector2f dims);
		void setArea(sf::Vector2f dims);
		void setRotation(float fRot);

		//Getters
		sf::Vector2f getPosition();
		sf::Vector2f getOrigin();
		sf::Vector2f getSize();
		sf::FloatRect getRect();
		sf::Vector2f getScale();
		std::vector<sf::Vector2f> getEdges();
};