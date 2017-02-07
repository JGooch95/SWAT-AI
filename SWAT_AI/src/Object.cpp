#include "../include/Object.h"

Object::Object()
{
	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100, 100);
}

void Object::linkMap(Map* NewMap)
{
	m_CurrentMap = NewMap;
}

void Object::setTexture(sf::Texture* Tex2)
{
	m_MainSprite.setTexture(*Tex2); //Applies the texture to the sprite.
}

void Object::setPosition(sf::Vector2f Pos)
{
	m_MainSprite.setPosition(Pos);
}

void Object::setSize(sf::Vector2f Dims)
{
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, Dims.x, Dims.y));
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
}