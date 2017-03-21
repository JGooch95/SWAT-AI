#pragma once

#include <SFML\Graphics.hpp>
#include "Map.h"

class Object : public sf::Drawable
{
protected:
	sf::Sprite m_MainSprite;
	sf::Texture* m_CurrentTexture;
	Map* m_CurrentMap;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
	Object();
	void setTexture(sf::Texture* tex2);
	void linkMap(Map* newMap);
	void setPosition(sf::Vector2f pos);
	void setOrigin(sf::Vector2f pos);
	void setSize(sf::Vector2f dims);
	void setRotation(float Rot);
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	sf::FloatRect getRect();
	sf::Vector2f getScale();
	std::vector<sf::Vector2f> getEdges();

};