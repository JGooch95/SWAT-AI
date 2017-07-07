#include "../include/WaveEffect.h"

WaveEffect::WaveEffect()
{

}

WaveEffect::WaveEffect(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position, waveType newType)
{
	m_fDesiredRadius = fRadius;
	m_fIncreaseRate = fIncreaseRate;
	m_fDecreaseRate = fDecreaseRate;
	m_Wave.setFillColor(sf::Color(255,255,255,10));
	m_Wave.setPosition(position);
	m_Wave.setRadius(0.0f);
	currentType = newType;
}

void WaveEffect::update()
{
	//If the wave has passed the center point
	if (m_Wave.getRadius() > m_fDesiredRadius/2) 
	{
		//Start to reduce the color of the wave and use the new speed
		m_Wave.setRadius(m_Wave.getRadius() + m_fDecreaseRate);
		switch (currentType)
		{
		case Sound:
			m_Wave.setFillColor(sf::Color(255, 255, 255, ((m_fDesiredRadius - m_Wave.getRadius()) / (m_fDesiredRadius / 2)) * 30.0f));
			break;
		case Explosion:
			m_Wave.setFillColor(sf::Color(255, 200, 0, ((m_fDesiredRadius - m_Wave.getRadius()) / (m_fDesiredRadius / 2)) * 100.0f));
			break;
		case Flash:
			m_Wave.setFillColor(sf::Color(255, 255, 255, ((m_fDesiredRadius - m_Wave.getRadius()) / (m_fDesiredRadius / 2)) * 100.0f));
			break;
		}
	}
	else
	{
		//Increase the color of the wave and use the increasing speed
		m_Wave.setRadius(m_Wave.getRadius() + m_fIncreaseRate);
		switch (currentType)
		{
		case Sound:
			m_Wave.setFillColor(sf::Color(255, 255, 255, (m_Wave.getRadius() / (m_fDesiredRadius / 2)) * 30.0f));
			break;
		case Explosion:
			m_Wave.setFillColor(sf::Color(255, 50, 0, (m_Wave.getRadius() / (m_fDesiredRadius / 2)) * 100.0f));
			break;
		case Flash:
			m_Wave.setFillColor(sf::Color(255, 255, 255, (m_Wave.getRadius() / (m_fDesiredRadius / 2)) * 100.0f));
			break;
		}
	}

	//Center the origin
	m_Wave.setOrigin(sf::Vector2f(m_Wave.getRadius(), m_Wave.getRadius()));
}

//Getters
bool WaveEffect::isDone()
{
	if (m_Wave.getRadius() >= m_fDesiredRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

sf::Vector2f WaveEffect::getPosition()
{
	return m_Wave.getPosition();
}

float WaveEffect::getRadius()
{
	return m_Wave.getRadius();
}

waveType WaveEffect::getType()
{
	return currentType;
}

void WaveEffect::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Wave);
}