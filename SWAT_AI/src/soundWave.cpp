#include "../include/SoundWave.h"

soundWave::soundWave()
{

}

soundWave::soundWave(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position)
{
	m_fDesiredRadius = fRadius;
	m_fIncreaseRate = fIncreaseRate;
	m_fDecreaseRate = fDecreaseRate;
	m_Wave.setFillColor(sf::Color(255,255,255,10));
	m_Wave.setPosition(position);
	m_Wave.setRadius(0.0f);
}

void soundWave::update()
{
	//If the wave has passed the center point
	if (m_Wave.getRadius() > m_fDesiredRadius/2) 
	{
		//Start to reduce the color of the wave and use the new speed
		m_Wave.setRadius(m_Wave.getRadius() + m_fDecreaseRate);
		m_Wave.setFillColor(sf::Color(255, 255, 255, ((m_fDesiredRadius  - m_Wave.getRadius()) / (m_fDesiredRadius/2)) * 30.0f));
	}
	else
	{
		//Increase the color of the wave and use the increasing speed
		m_Wave.setRadius(m_Wave.getRadius() + m_fIncreaseRate);
		m_Wave.setFillColor(sf::Color(255, 255, 255, (m_Wave.getRadius() / (m_fDesiredRadius/2)) * 30.0f));
	}

	//Center the origin
	m_Wave.setOrigin(sf::Vector2f(m_Wave.getRadius(), m_Wave.getRadius()));
}

bool soundWave::isDone()
{
	if (m_Wave.getRadius() > m_fDesiredRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

sf::Vector2f soundWave::getPosition()
{
	return m_Wave.getPosition();
}

float soundWave::getRadius()
{
	return m_Wave.getRadius();
}

void soundWave::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Wave);
}