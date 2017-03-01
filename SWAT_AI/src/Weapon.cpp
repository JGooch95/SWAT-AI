#include "../include/Weapon.h"

Weapon::Weapon()
{
	m_AimLine = sf::VertexArray(sf::Lines, 2);
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - 2000.0f);
	for (int i = 0; i < m_AimLine.getVertexCount(); i++)
	{
		m_AimLine[i].color = sf::Color(255, 0, 0, 255);
	}

	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 25, 50));
	m_MainSprite.setOrigin(m_MainSprite.getLocalBounds().width / 2, -m_MainSprite.getLocalBounds().height / 2 + 10);
	m_fFireRate = 0.2f;
	m_fDamage = 5.0f;
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_AimLine);
	target.draw(m_MainSprite);
}

void Weapon::aim(sf::Vector2f location)
{
	float fMagnitude = sqrtf(pow(location.x, 2.0f) + pow(location.y, 2.0f));
	location /= fMagnitude;

	location *= 2000.0f; //Multiplies it by the length

	float fRotAngle = -atan2f(location.x, location.y) * (180.0f / 3.14f); // Finding the angle of the vector for the sprite

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_LazerPoint = m_MainSprite.getPosition() + location;

	m_AimLine[1].position = m_LazerPoint;
	m_MainSprite.setRotation(fRotAngle);
}

void Weapon::aim(float fAngle)
{
	sf::Vector2f RotVect(2000.0f * cos((fAngle + 90.0f) * (3.14159265359 / 180)),
						 2000.0f * sin((fAngle + 90.0f) * (3.14159265359 / 180))); //Finding the vector between the character's center and the mouse


	//Sets the rotation of the sprite and adjusts the orientation line according to the rotation
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + RotVect;
	m_LazerPoint = m_MainSprite.getPosition() + RotVect;

	m_MainSprite.setRotation(fAngle);
	
}

sf::Vector2f Weapon::calcLazerIntersect(sf::Vector2f a, sf::Vector2f b)
{
	return  Util::lineIntersect(a, b, m_MainSprite.getPosition(), m_LazerPoint);
}

void Weapon::setIntersect(sf::Vector2f vect)
{
	m_LazerPoint = vect;
	m_AimLine[1].position = m_LazerPoint;
}

sf::Vector2f Weapon::getIntersect()
{
	return m_AimLine[1].position;
}

float Weapon::getDamage()
{
	return m_fDamage;
}

float Weapon::getFireRate()
{
	return m_fFireRate;
}