#include "../include/Collidable.h"

Collidable::Collidable() 
{
	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100, 100);
}

void Collidable::linkMap(Map* NewMap)
{
	m_CurrentMap = NewMap;
}

void Collidable::setTexture(sf::Texture* Tex2)
{
	m_MainSprite.setTexture(*Tex2); //Applies the texture to the sprite.
}

void Collidable::setPosition(sf::Vector2f Pos)
{
	m_MainSprite.setPosition(Pos);
}

void Collidable::setSize(sf::Vector2f Dims)
{
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, Dims.x, Dims.y));
}