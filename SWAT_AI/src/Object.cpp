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
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, tex2->getSize().x, tex2->getSize().y));
}

void Object::setPosition(sf::Vector2f pos)
{
	m_MainSprite.setPosition(pos);
}
void Object::setOrigin(sf::Vector2f pos)
{
	m_MainSprite.setOrigin(pos);
}

void Object::setSize(sf::Vector2f dims)
{
	//m_MainSprite.setTextureRect(sf::IntRect(0, 0, dims.x, dims.y));
	if (m_MainSprite.getTexture() != NULL)
	{
		m_MainSprite.setScale(sf::Vector2f(dims.x / m_MainSprite.getTexture()->getSize().x, dims.y / m_MainSprite.getTexture()->getSize().y));
	}
}

void Object::setRotation(float Rot)
{
	m_MainSprite.setRotation(Rot);
}
sf::FloatRect Object::getRect()
{
	return m_MainSprite.getGlobalBounds();
}

sf::Vector2f Object::getPosition()
{
	return m_MainSprite.getPosition();
}

sf::Vector2f Object::getSize()
{
	return sf::Vector2f(m_MainSprite.getLocalBounds().width, m_MainSprite.getLocalBounds().height);
}
sf::Vector2f Object::getScale()
{
	return m_MainSprite.getScale();
}
std::vector<sf::Vector2f> Object::getEdges()
{
	std::vector<sf::Vector2f> Edges; //Holds the wall edges
	
	Edges.push_back(sf::Vector2f(getRect().left,  getRect().top));
	Edges.push_back(sf::Vector2f(getRect().left + getRect().width, getRect().top));
											
	Edges.push_back(sf::Vector2f(getRect().left + getRect().width, getRect().top));
	Edges.push_back(sf::Vector2f(getRect().left + getRect().width, getRect().top + getRect().height));
											
	Edges.push_back(sf::Vector2f(getRect().left + getRect().width, getRect().top + getRect().height));
	Edges.push_back(sf::Vector2f(getRect().left,  getRect().top  + getRect().height));
											
	Edges.push_back(sf::Vector2f(getRect().left,  getRect().top  + getRect().height));
	Edges.push_back(sf::Vector2f(getRect().left,  getRect().top));
	
	return Edges;
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
}