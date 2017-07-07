#include "../include/Bar.h"

Bar::Bar()
{
	//Sets the default limits of the bar
	m_LevelLimits.lower = 100.0f;
	m_LevelLimits.upper = 100.0f;
}

void Bar::update()
{
	m_FrontBox.setSize(sf::Vector2f(m_BackBox.getSize().x * (m_LevelLimits.lower / m_LevelLimits.upper), m_FrontBox.getSize().y));
}

//Setters
void Bar::setSize(sf::Vector2f newDimensions)
{
	m_BackBox.setSize(newDimensions);
	m_FrontBox.setSize(newDimensions);
	update();
}

void Bar::setPosition(sf::Vector2f newPosition)
{
	m_BackBox.setPosition(newPosition);
	m_FrontBox.setPosition(newPosition);
}

void Bar::setLevelColor(sf::Color newColor)
{
	m_FrontBox.setFillColor(newColor);
}

void Bar::setBarColor(sf::Color newColor)
{
	m_BackBox.setFillColor(newColor);
}

void Bar::setLevel(float fLevel)
{
	m_LevelLimits.lower = fLevel;

	//Limits the level of the bar between 0 and the upper limit
	if (m_LevelLimits.lower < 0.0f)
	{
		m_LevelLimits.lower = 0.0f;
	}
	else if (m_LevelLimits.lower > m_LevelLimits.upper)
	{
		m_LevelLimits.lower = m_LevelLimits.upper;
	}
	update();
}

void Bar::setLimit(float fLimit)
{
	//Ensures limit is higher than 0 before setting
	if (fLimit > 0)
	{
		m_LevelLimits.upper = fLimit;
		update();
	}
	else
	{
		std::printf("Bar Error: Cannot set limit to 0 \n");
	}
}

//Getters
sf::Vector2f Bar::getSize()
{
	return m_BackBox.getSize();
}

sf::Vector2f Bar::getLevelLimits()
{
	return sf::Vector2f(m_LevelLimits.lower, m_LevelLimits.upper);
}

sf::Vector2f Bar::getPosition()
{
	return m_BackBox.getPosition();
}

void Bar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_BackBox);
	target.draw(m_FrontBox);
}