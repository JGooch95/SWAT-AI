#include "../include/Bar.h"

Bar::Bar()
{
	m_LevelLimits.x = 100.0f;
	m_LevelLimits.y = 100.0f;
}

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
	m_LevelLimits.x = fLevel;
	if (m_LevelLimits.x < 0.0f)
	{
		m_LevelLimits.x = 0.0f;
	}
	else if (m_LevelLimits.x > m_LevelLimits.y)
	{
		m_LevelLimits.x = m_LevelLimits.y;
	}
	update();
}

void Bar::setLimit(float fLimit)
{
	if (fLimit > 0)
	{
		m_LevelLimits.y = fLimit;
		update();
	}
	else
	{
		std::printf("Bar Error: Cannot set limit to 0 \n");
	}
}

void Bar::update()
{
		m_FrontBox.setSize(sf::Vector2f(m_BackBox.getSize().x * (m_LevelLimits.x / m_LevelLimits.y), m_FrontBox.getSize().y));
}

sf::Vector2f Bar::getSize()
{
	return m_BackBox.getSize();
}

sf::Vector2f Bar::getLevelLimits()
{
	return m_LevelLimits;
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