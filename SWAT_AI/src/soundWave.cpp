#include "../include/SoundWave.h"

soundWave::soundWave()
{

}

soundWave::soundWave(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position)
{
	m_fCurrentRadius = 0;
	m_fDesiredRadius = fRadius;
	m_fIncreaseRate = fIncreaseRate;
	m_fDecreaseRate = fDecreaseRate;
	iDirection = 1;
	m_Wave.setFillColor(sf::Color(255,255,255,10));
	m_Wave.setPosition(position);
	m_bDone = false;
	//m_Wave.setOutlineThickness(1);
	//m_Wave.setOutlineColor(sf::Color(255, 255, 255, 50));
}

void soundWave::update()
{
	m_Wave.setOrigin(sf::Vector2f(m_fCurrentRadius, m_fCurrentRadius));

	if (m_fCurrentRadius > m_fDesiredRadius/2)
	{
		m_fCurrentRadius += m_fDecreaseRate;
		m_Wave.setFillColor(sf::Color(255, 255, 255, ((m_fDesiredRadius  - m_fCurrentRadius) / (m_fDesiredRadius/2)) * 30.0f));
		m_Wave.setRadius(m_fCurrentRadius);
		if (m_fCurrentRadius > m_fDesiredRadius)
		{
			m_bDone = true;
		}
	}
	else
	{
		m_fCurrentRadius += m_fIncreaseRate;
		m_Wave.setFillColor(sf::Color(255, 255, 255, (m_fCurrentRadius / (m_fDesiredRadius/2)) * 30.0f));
		m_Wave.setRadius(m_fCurrentRadius);
	}
}

bool soundWave::isDone()
{
	return m_bDone;
}

void soundWave::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Wave);
}