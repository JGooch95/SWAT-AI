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
}

void soundWave::update()
{
	if (iDirection == 1)
	{
		m_fCurrentRadius += m_fIncreaseRate * iDirection;
	}
	else if (iDirection == -1)
	{
		m_fCurrentRadius += m_fDecreaseRate * iDirection;
	}

	m_Wave.setOrigin(sf::Vector2f(m_fCurrentRadius, m_fCurrentRadius));

	if (m_fCurrentRadius >= m_fDesiredRadius)
	{
		iDirection = -1;
	}
	if (m_fCurrentRadius < 0)
	{
		m_bDone = true;
	}

	m_Wave.setRadius(m_fCurrentRadius);
	m_Wave.setFillColor(sf::Color(255, 255, 255, (m_fCurrentRadius/m_fDesiredRadius) * 30.0f));
}

bool soundWave::isDone()
{
	return m_bDone;
}

void soundWave::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Wave);
}