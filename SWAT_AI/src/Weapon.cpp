#include "../include/Weapon.h"

Weapon::Weapon()
{
	m_AimLine = sf::VertexArray(sf::Lines, 2);
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - 2000);
	for (int i = 0; i < m_AimLine.getVertexCount(); i++)
	{
		m_AimLine[i].color = sf::Color(255, 0, 0, 255);
	}

	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 25, 50));
	m_MainSprite.setOrigin(m_MainSprite.getLocalBounds().width / 2, -m_MainSprite.getLocalBounds().height / 2 + 10);
}

void Weapon::shoot()
{

}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_AimLine);
	target.draw(m_MainSprite);
}

void Weapon::aim(sf::Vector2f Location)
{
	float fMagnitude = sqrtf(pow(Location.x, 2.0f) + pow(Location.y, 2.0f));
	Location /= fMagnitude;

	Location *= LazerLength(sf::FloatRect(1,1,1,1)); //Multiplies it by the length

	float fRotAngle = -atan2f(Location.x, Location.y) * (180.0f / 3.14f); // Finding the angle of the vector for the sprite

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + Location;

	m_MainSprite.setRotation(fRotAngle);
}

float Weapon::LazerLength(sf::FloatRect ColliderRect)
{
	return 2000.0f;
}