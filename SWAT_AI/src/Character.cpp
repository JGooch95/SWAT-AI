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

	m_HealthBar.setSize(sf::Vector2f(70, 5));
	m_HealthBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_HealthBar.setLevelColor(sf::Color(0, 255, 0, 255));
	m_HealthBar.setLevel(100);
	m_HealthBar.setLimit(100);
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));

}

void Character::lookAt(sf::Vector2f Position)
{
	sf::Vector2f RotVect (Position - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse

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

void Character::setPath(std::deque<Node*> NewPath)
{
	Path = NewPath;
}

void Character::Move()
{
	if (!Path.empty())
	{
		const float kfMoveSpeed = 2; //The amount of pixels the character moves per frame

		//Sets the node to reach to be the next node in the path
		sf::Vector2f Destination((((Path.at(0)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
								 (((Path.at(0)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));

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
			Path.pop_front();
		}

		//Sets up the path line ready to start drawing a new path
		m_PathLine.clear();
		m_PathLine.resize(Path.size());
		for (int i = 0; i < Path.size(); i++)
		{

			m_PathLine[i] = sf::Vertex(sf::Vector2f(
				(((Path.at(i)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
				(((Path.at(i)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2))),
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
	Move();
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));
	m_HealthBar.setLevel(m_HealthBar.getLevelLimits().x - 1);
}

void Character::setGunTexture(sf::Texture* Tex2)
{
	m_Weapon1.setTexture(Tex2); //Applies the texture to the sprite.
}

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Draws the character Sprite
	target.draw(m_Weapon1);
	target.draw(m_MainSprite);
	target.draw(m_OrientationLine);
	target.draw(m_PathLine);
	target.draw(m_HealthBar);
}