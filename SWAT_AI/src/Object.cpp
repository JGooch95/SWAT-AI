#include "../include/Object.h"

Object::Object()
{
	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100.0f, 100.0f);
	m_CurrentMap = Map::getInstance();
	m_CurrentSettings = Settings::getInstance();
}

//Setters
void Object::setTexture(sf::Texture* tex2)
{
	m_CurrentTexture = tex2;
	m_MainSprite.setTexture(m_CurrentTexture, true); //Applies the texture to the sprite.
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, tex2->getSize().x, tex2->getSize().y));
}

void Object::setPosition(sf::Vector2f pos)
{
	m_MainSprite.setPosition(pos);
}

void Object::setOrigin(sf::Vector2f pos)
{
	m_MainSprite.setOrigin(pos.x, pos.y);
}

void Object::setSize(sf::Vector2f dims)
{
	m_MainSprite.setSize(dims);
}

void Object::setColor(sf::Color newColor)
{
	m_MainSprite.setFillColor(newColor);
}

void Object::setArea(sf::Vector2f dims)
{
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, dims.x, dims.y));
	m_MainSprite.setTexture(m_CurrentTexture);
}

void Object::setRotation(float fRot)
{
	m_MainSprite.setRotation(fRot);
}

//Getters
sf::Vector2f Object::getPosition()
{
	return m_MainSprite.getPosition();
}

sf::Vector2f Object::getOrigin()
{
	return sf::Vector2f(m_MainSprite.getOrigin().x, m_MainSprite.getOrigin().y);
}

sf::Vector2f Object::getSize()
{
	return m_MainSprite.getSize();
}

sf::FloatRect Object::getRect()
{
	return m_MainSprite.getGlobalBounds();
}

sf::Vector2f Object::getScale()
{
	return m_MainSprite.getScale();
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Object::getEdges()
{
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> vEdges; //Holds the edges of the object
	
	//Top edge
	vEdges.push_back(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(getRect().left,  getRect().top), 
														   sf::Vector2f(getRect().left + getRect().width, getRect().top)));
	
	//Right edge
	vEdges.push_back(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(getRect().left + getRect().width, getRect().top), 
														   sf::Vector2f(getRect().left + getRect().width, getRect().top + getRect().height)));
	
	//Bottom edge
	vEdges.push_back(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(getRect().left + getRect().width, getRect().top + getRect().height), 
														   sf::Vector2f(getRect().left, getRect().top + getRect().height)));
	
	
	//Left edge
	vEdges.push_back(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(getRect().left, getRect().top + getRect().height),
														   sf::Vector2f(getRect().left, getRect().top)));
	
	return vEdges;
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
}