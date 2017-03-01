#include "../include/Object.h"

Object::Object()
{
	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100, 100);
}

void Object::linkMap(Map* newMap)
{
	m_CurrentMap = newMap;
}

void Object::setTexture(sf::Texture* tex2)
{
	m_MainSprite.setTexture(*tex2); //Applies the texture to the sprite.
}

void Object::setPosition(sf::Vector2f pos)
{
	m_MainSprite.setPosition(pos);
}

void Object::setSize(sf::Vector2f dims)
{
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, dims.x, dims.y));
}
sf::FloatRect Object::getRect()
{
	return m_MainSprite.getGlobalBounds();
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
}