#include "../include/Character.h"
#include <iostream>
#include <math.h>

Character::Character()
{
	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100, 100);
	m_MainSprite.setOrigin((m_MainSprite.getLocalBounds().width / 2), (m_MainSprite.getLocalBounds().height / 2));

	//Sets up the orientation indication
	m_OrientationLine = sf::VertexArray(sf::Lines, 2);
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - (m_MainSprite.getLocalBounds().height / 2));
	for (int i = 0; i < m_OrientationLine.getVertexCount(); i++)
	{
		m_OrientationLine[i].color = sf::Color(255, 0, 255, 255);
	}

	m_Weapon1.setPosition(sf::Vector2f(m_MainSprite.getPosition().x + 10, m_MainSprite.getPosition().y));

	//Sets up the Path line
	m_PathLine.setPrimitiveType(sf::LinesStrip); 

	m_CollisionLine.setPrimitiveType(sf::LinesStrip);
	m_CollisionLine.resize(2);
	for (int i = 0; i < m_CollisionLine.getVertexCount(); i++)
	{
		m_CollisionLine[i].color = sf::Color(255, 255, 0, 255);
	}

	m_HealthBar.setSize(sf::Vector2f(70, 5));
	m_HealthBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_HealthBar.setLevelColor(sf::Color(0, 255, 0, 255));
	m_HealthBar.setLevel(100);
	m_HealthBar.setLimit(100);
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));

	m_AmmoBar.setSize(sf::Vector2f(70, 5));
	m_AmmoBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_AmmoBar.setLevelColor(sf::Color(255, 255, 0, 255));
	m_AmmoBar.setLevel(30);
	m_AmmoBar.setLimit(30);
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 40));

	m_CurrentState = SEARCH_SWEEP;
	m_ReloadClock.restart();
	m_FireRateClock.restart();
}

void Character::lookAt(sf::Vector2f position)
{
	sf::Vector2f RotVect (position - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse

	//Finding the unit normal
	float fMagnitude = sqrtf(pow(RotVect.x, 2.0f) + pow(RotVect.y, 2.0f)); 
	RotVect /= fMagnitude; 

	RotVect *= m_MainSprite.getLocalBounds().height / 2; //Multiplies it by the length

	float fRotAngle = -atan2f(RotVect.x, RotVect.y) * (180.0f / 3.14f); // Finding the angle of the vector for the sprite

	//Sets the rotation of the sprite and adjusts the orientation line according to the rotation
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = m_MainSprite.getPosition() + RotVect;

	m_MainSprite.setRotation(fRotAngle);

	m_Weapon1.aim(RotVect);
	m_Weapon1.setPosition(m_MainSprite.getPosition());
}

void Character::setPath(std::deque<Node*> newPath)
{
	m_Path = newPath;
}

void Character::move()
{
	if (!m_Path.empty())
	{
		const float kfMoveSpeed = 2.0f; //The amount of pixels the character moves per frame

		//Sets the node to reach to be the next node in the path
		sf::Vector2f Destination((((m_Path.at(0)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
								 (((m_Path.at(0)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));

		sf::Vector2f Velocity(Destination - m_MainSprite.getPosition()); //Finds the distance between the next path node and the centre of the sprite

		//Finds the unit normal
		float fMagnitude = sqrtf(pow(Velocity.x, 2) + pow(Velocity.y, 2));
		Velocity /= fMagnitude; 

		Velocity *= kfMoveSpeed; //Multiplies it by the speed

		m_MainSprite.setPosition(m_MainSprite.getPosition() + Velocity); //Moves the Sprite

		if (m_MainSprite.getPosition().x >= Destination.x - 10 &&
			m_MainSprite.getPosition().x <= Destination.x + 10 &&
			m_MainSprite.getPosition().y <= Destination.y + 10 &&
			m_MainSprite.getPosition().y >= Destination.y - 10)
		{
			m_Path.pop_front();
		}

		//Sets up the path line ready to start drawing a new path
		m_PathLine.clear();
		m_PathLine.resize(m_Path.size());
		for (int i = 0; i < m_Path.size(); i++)
		{

			m_PathLine[i] = sf::Vertex(sf::Vector2f(
				(((m_Path.at(i)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
				(((m_Path.at(i)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2))),
				sf::Color(0, 255, 0, 255));
		}
	}
}

sf::Vector2f Character::getPosition()
{
	return m_MainSprite.getPosition();
}

void Character::update()
{
	move();
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_AmmoBar.getSize().x / 2, m_MainSprite.getPosition().y - 40));
	
	if (m_bReloading)
	{
		if (m_ReloadClock.getElapsedTime().asSeconds() >= 2.0f)
		{
			m_AmmoBar.setLevel(m_AmmoBar.getLevelLimits().y);
			m_bReloading = false;
		}
	}

	switch (m_CurrentState)
	{
		case SEARCH_SWEEP:
		{

			break;
		}
		default:
		{
			break;
		}
	}
}

void Character::setGunTexture(sf::Texture* tex2)
{
	m_Weapon1.setTexture(tex2); //Applies the texture to the sprite.
}

bool Character::lazerChecks(std::vector<sf::Vector2f>vEdges)
{
	bool bIntersectFound = false;
	sf::Vector2f lowestIntersect(0, 2000);
	float fLength;
	float fMagnitude;

	for (int i = 0; i < vEdges.size(); i+=2)
	{
		sf::Vector2f currentIntersect = m_Weapon1.calcLazerIntersect(vEdges.at(i), vEdges.at(i+1));
		fLength = sqrtf(pow(currentIntersect.x - m_MainSprite.getPosition().x, 2.0f) + pow(currentIntersect.y - m_MainSprite.getPosition().y, 2.0f));
		fMagnitude = sqrtf(pow(lowestIntersect.x - m_MainSprite.getPosition().x, 2.0f) + pow(lowestIntersect.y - m_MainSprite.getPosition().y, 2.0f));
	
		if (fLength < fMagnitude)
		{
			lowestIntersect = currentIntersect;
			bIntersectFound = true;
		}
	}
	if (bIntersectFound)
	{
		m_Weapon1.setIntersect(lowestIntersect);
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<sf::Vector2f> Character::getCollisionLine(float fAngle)
{
	sf::Vector2f radiusLine = sf::Vector2f(m_MainSprite.getLocalBounds().width /2  * cos(fAngle * (3.14159265359 / 180)),
										   m_MainSprite.getLocalBounds().width / 2 * sin(fAngle * (3.14159265359 / 180)));

	std::vector<sf::Vector2f> blob = {m_MainSprite.getPosition() - radiusLine , m_MainSprite.getPosition() + radiusLine};
	m_CollisionLine[0].position = blob[0];
	m_CollisionLine[1].position = blob[1];
	return blob;

}

float Character::getRotation()
{
	return m_MainSprite.getRotation();
}

void Character::setHealth(float fLevel)
{
	m_HealthBar.setLevel(fLevel);
}

float Character::getHealth()
{
	return m_HealthBar.getLevelLimits().x;
}

float Character::shoot()
{
	if (!m_bReloading)
	{
		//If there is no ammo then reload
		if (m_AmmoBar.getLevelLimits().x <= 0)
		{
			reload();
		}
		//Otherwise if the gun can shoot then shoot
		else if (m_FireRateClock.getElapsedTime().asSeconds() >= m_Weapon1.getFireRate())
		{
			m_FireRateClock.restart();
			m_AmmoBar.setLevel(m_AmmoBar.getLevelLimits().x - 1);
			return m_Weapon1.getDamage();
		}
	}
	return 0.0f; //Deal no damage
}

void Character::reload()
{
	m_bReloading = true;
	m_ReloadClock.restart();
}

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Draws the character Sprite
	target.draw(m_Weapon1);
	target.draw(m_MainSprite);
	target.draw(m_OrientationLine);
	target.draw(m_PathLine);
	target.draw(m_CollisionLine);
	target.draw(m_AmmoBar);
	target.draw(m_HealthBar);
}