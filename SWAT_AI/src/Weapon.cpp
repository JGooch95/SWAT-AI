#include "../include/Weapon.h"

Weapon::Weapon()
{
	//Sets up the AimLine
	m_AimLine = sf::VertexArray(sf::Lines, 2);
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - 2000.0f);
	for (int i = 0; i < m_AimLine.getVertexCount(); i++)
	{
		m_AimLine[i].color = sf::Color(255, 0, 0, 255);
	}

	//Sets up the default texture data
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 25, 50));
	m_MainSprite.setOrigin(m_MainSprite.getLocalBounds().width / 2, -m_MainSprite.getLocalBounds().height / 2 + 10);

	//Sets the default fire rate
	m_fFireRate = 0.2f;
	
	//Sets the default limits
	m_RangeLimits.lower = 200.0f;
	m_RangeLimits.upper = 500.0f;

	m_DamageLimits.lower = 1.0f;
	m_DamageLimits.upper = 5.0f;
}

void Weapon::aim(sf::Vector2f location)
{
	//Finds the unit vector
	float fMagnitude = sqrtf(pow(location.x, 2.0f) + pow(location.y, 2.0f));
	location /= fMagnitude;

	float fRotAngle = -atan2f(location.x, location.y) * (180.0f / 3.14f); // Finding the angle of the vector given

	location *= 2000.0f; //Multiplies the vector by the desired length

	//Sets the rotation of the sprite and adjusts the aimline according to the rotation
	m_MainSprite.setRotation(fRotAngle);

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + location;
}

void Weapon::aim(float fAngle)
{
	//Points a vector towards the angle given
	sf::Vector2f RotVect(2000.0f * cos((fAngle + 90.0f) * (3.14159265359 / 180)),
						 2000.0f * sin((fAngle + 90.0f) * (3.14159265359 / 180))); 


	//Sets the rotation of the sprite and adjusts the aimline according to the rotation
	m_MainSprite.setRotation(fAngle);

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + RotVect;
}

void Weapon::update()
{
	m_MainSprite.setScale(m_CurrentMap->getTileSize().x / 25, m_CurrentMap->getTileSize().y / 50);
}

//Setters
void Weapon::setFireRate(float fVal)
{
	m_fFireRate = fVal;
}

void Weapon::setDamage(sf::Vector2f Val)
{
	m_DamageLimits.lower = Val.x;
	m_DamageLimits.upper = Val.y;
}

void Weapon::setRange(sf::Vector2f Val)
{
	m_RangeLimits.lower = Val.x;
	m_RangeLimits.upper = Val.y;
}

void Weapon::setOrigin(sf::Vector2f Val)
{
	m_MainSprite.setOrigin(Val);
}

void Weapon::setIntersect(sf::Vector2f vect)
{
	m_AimLine[1].position = vect;
}

//Getters
float Weapon::getDamage()
{
	//Finds the distance between the target and the character
	float dist = Util::magnitude(m_AimLine[1].position - m_MainSprite.getPosition());
	float damage = m_DamageLimits.upper;

	//If the distance is further than the range then lower the damage
	if (dist > m_RangeLimits.upper)
	{
		dist = dist - m_RangeLimits.upper;
		damage = m_DamageLimits.upper * (1-(dist / m_RangeLimits.upper));
	}
	//If the distance is lower than the range then lower the damge
	else if (dist < m_RangeLimits.lower)
	{
		damage = m_DamageLimits.upper * (dist / m_RangeLimits.lower);
	}

	//limit the damage to the lowest damage if surpassed
	if (damage < m_DamageLimits.lower)
	{
		damage = m_DamageLimits.lower;
	}

	return damage;
}

float Weapon::getFireRate()
{
	return m_fFireRate;
}

sf::Vector2f Weapon::getPosition()
{
	return m_MainSprite.getPosition();
}

sf::Vector2f Weapon::getIntersect()
{
	return m_AimLine[1].position;
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_AimLine);
	target.draw(m_MainSprite);
}