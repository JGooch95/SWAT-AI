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

	m_MovementLine.setPrimitiveType(sf::LinesStrip);
	m_MovementLine.resize(2);
	for (int i = 0; i < m_MovementLine.getVertexCount(); i++)
	{
		m_MovementLine[i].color = sf::Color(0, 255, 255, 255);
	}

	m_CollisionLine.setPrimitiveType(sf::LinesStrip);
	m_CollisionLine.resize(2);
	for (int i = 0; i < m_CollisionLine.getVertexCount(); i++)
	{
		m_CollisionLine[i].color = sf::Color(255, 255, 0, 255);
	}

	m_VisionRays.setPrimitiveType(sf::TrianglesFan);

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
	m_iAimingDirection = 1;
	m_CurrentTarget = NULL;
	bDrawVision = false;
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
}

void Character::lookAt(float fAngle)
{
	sf::Vector2f RotVect((m_MainSprite.getLocalBounds().height / 2) * cos((fAngle +90) * (3.14159265359f / 180.0f)),
						 (m_MainSprite.getLocalBounds().height / 2) * sin((fAngle +90) * (3.14159265359f / 180.0f))); //Finding the vector between the character's center and the mouse

	//Sets the rotation of the sprite and adjusts the orientation line according to the rotation
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = m_MainSprite.getPosition() + RotVect;

	m_MainSprite.setRotation(fAngle);
	m_Weapon1.aim(fAngle);
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

		if (fMagnitude == 0)
		{
			m_Path.pop_front();
		}
		else
		{
			Velocity /= fMagnitude;

			m_MovementLine[0].position = m_MainSprite.getPosition();
			m_MovementLine[1].position = m_MainSprite.getPosition() + (Velocity * (m_MainSprite.getLocalBounds().height / 2));

			m_fMovementAngle = (atan2f(Velocity.y, Velocity.x) * (180.0f / 3.14f)) - 90;
			m_fMovementAngle = Util::setWithinRange(m_fMovementAngle, 0.0f, 360.0f);

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
}

void Character::update()
{
	move();
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_AmmoBar.getSize().x / 2, m_MainSprite.getPosition().y - 40));
	m_Weapon1.setPosition(m_MainSprite.getPosition());

	if (m_bReloading)
	{
		if (m_ReloadClock.getElapsedTime().asSeconds() >= 2.0f)
		{
			m_AmmoBar.setLevel(m_AmmoBar.getLevelLimits().y);
			m_bReloading = false;
		}
	}

	float fCone = 30.0f;

	switch (m_CurrentState)
	{
		case SEARCH_SWEEP:
		{
			if (m_fAimingAngle > fCone)
			{
				m_fAimingAngle = fCone;
				m_iAimingDirection *= -1;
			}
			if (m_fAimingAngle < -fCone)
			{
				m_fAimingAngle= -fCone;
				m_iAimingDirection *= -1;
			}
			m_fAimingAngle += m_iAimingDirection;

			lookAt(m_fMovementAngle + m_fAimingAngle);
			break;
		}
		case AIM:
		{
			if (m_CurrentTarget != NULL)
			{
				lookAt(m_CurrentTarget->getPosition());
				sf::Vector2f Vect = m_CurrentTarget->getPosition() - m_MainSprite.getPosition();
				m_fAimingAngle = (atan2f(Vect.y, Vect.x) * (180.0f / 3.14f)) - m_fMovementAngle - 90;// -m_fMovementAngle; // Finding the angle of the vector for the sprite
				m_fAimingAngle = Util::setWithinRange(m_fAimingAngle, 0.0f, 360.0f);

				if (m_CurrentTarget->getHealth() <= 0)
				{
					m_CurrentTarget = NULL;
				}
			}
			else
			{
				m_CurrentState = SEARCH_SWEEP;
			}
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

struct Ray
{
	float angle;
	sf::Vector2f Vect;
	sf::Vector2f OriginalVect;
};

int partition(std::vector<Ray>& Rays, int startIndex, int endIndex)
{
	int i = startIndex;

	for (int j = startIndex + 1; j < endIndex; j++)
	{
		if (Rays.at(j).angle < Rays.at(startIndex).angle)
		{
			i = i + 1;
			iter_swap(Rays.begin() + i, Rays.begin() + j);
		}
	}

	iter_swap(Rays.begin() + i, Rays.begin() + startIndex);
	return i;
}

void QuickSort(std::vector<Ray>& Rays, int startIndex, int endIndex)
{
	if (startIndex < endIndex)
	{
		int r = partition(Rays, startIndex, endIndex);
		QuickSort(Rays, startIndex, r);
		QuickSort(Rays, r + 1, endIndex);
	}
}

std::pair<bool, sf::Vector2f> Character::findLowestIntersect(std::vector<sf::Vector2f>vEdges, std::vector<sf::Vector2f> newRay)
{
	sf::Vector2f lowestIntersect = newRay.at(1);
	bool intersected = false;

	//For every edge
	for (int j = 0; j < vEdges.size(); j += 2)
	{
		//Checks where the ray and the edge intersect
		sf::Vector2f currentIntersect = Util::lineIntersect(vEdges.at(j), vEdges.at(j + 1), newRay.at(0), newRay.at(1));

		//If the ray is shorter than the previous rays then set the ray to be the shortest ray
		if (Util::magnitude(currentIntersect - m_MainSprite.getPosition()) < Util::magnitude(lowestIntersect - m_MainSprite.getPosition()))
		{
			lowestIntersect = currentIntersect;
			intersected = true;
		}
	}
	return std::pair<bool, sf::Vector2f>{ intersected, lowestIntersect };
}

bool Character::lazerChecks(std::vector<sf::Vector2f>vEdges)
{
	std::vector<sf::Vector2f> lazerRay = { m_Weapon1.getPosition(), m_Weapon1.getIntersect()};
	std::pair<bool, sf::Vector2f> lowestIntersect = findLowestIntersect(vEdges, lazerRay);

	if (lowestIntersect.first)
	{
		m_Weapon1.setIntersect(lowestIntersect.second);
		return true;
	}
	else
	{
		return false;
	}
}

void Character::visionCalculation(std::vector<sf::Vector2f>vEdges)
{
	std::vector<Ray> vRays;
	std::vector<Ray> vFinalRays;
	Ray tempRay;
	float fViewDistance = 2000.0f;
	float fVisionCone = 45.0f;
	
	//Edges
	for (int i = -1; i <= 1; i += 2)
	{
		float fRotAngle = m_fMovementAngle + m_fAimingAngle + (i*fVisionCone) + 90;
		fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);

		sf::Vector2f rayVect = sf::Vector2f(fViewDistance * cos((fRotAngle) * (3.14159265359f / 180.0f)),
			fViewDistance * sin((fRotAngle) * (3.14159265359f / 180.0f)));

		//Adds the data to a temp Ray to be added to the ray vector.
		tempRay.Vect = m_MainSprite.getPosition() + rayVect;
		tempRay.OriginalVect = sf::Vector2f(fViewDistance, fViewDistance);
		tempRay.angle = fRotAngle;
		vRays.push_back(tempRay);
	}

	//Generate the rays
	for (int i = 0; i < vEdges.size(); i++)
	{
		//Create a ray pointing towards the corner given
		sf::Vector2f rayVect = vEdges.at(i) - m_MainSprite.getPosition();

		//Gets the angle of the vector
		float fRotAngle = atan2f(rayVect.y, rayVect.x) * (180.0f / 3.14f);
		fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);
		
		rayVect = sf::Vector2f(fViewDistance * cos((fRotAngle) * (3.14159265359f / 180.0f)),
			fViewDistance * sin((fRotAngle) * (3.14159265359f / 180.0f)));

		//Adds the data to a temp Ray to be added to the ray vector.
		tempRay.Vect = m_MainSprite.getPosition() + rayVect;
		tempRay.OriginalVect = vEdges.at(i);
		tempRay.angle = fRotAngle;
		vRays.push_back(tempRay);
	}

	//Check for ray collisions
	//Edges of vision cone
	for (int i = 0; i < 2; i++)
	{
		//Set the lowest intersect to the rays location
		std::vector<sf::Vector2f> viewRay = { m_MainSprite.getPosition(), vRays.at(i).Vect };
		vRays.at(i).Vect = findLowestIntersect(vEdges, viewRay).second; //Sets the new length of the ray
		vFinalRays.push_back(vRays.at(i));
	}

	//For every ray
	for (int i = 2; i < vRays.size(); i++)
	{
		//Set the lowest intersect to the rays location
		std::vector<sf::Vector2f> viewRay = { m_MainSprite.getPosition(), vRays.at(i).Vect };
		vRays.at(i).Vect = findLowestIntersect(vEdges, viewRay).second; //Sets the new length of the ray

		//If the distance to the corner being pointed at is less than the ray distance then the corner is added to get the correct effect
		if (Util::magnitude(vRays.at(i).OriginalVect - m_MainSprite.getPosition()) < Util::magnitude(vRays.at(i).Vect - m_MainSprite.getPosition()))
		{
			tempRay.Vect = vRays.at(i).OriginalVect;
			tempRay.angle = vRays.at(i).angle;
			tempRay.OriginalVect = sf::Vector2f(fViewDistance, fViewDistance);
			tempRay.angle = Util::setWithinRange(tempRay.angle, 0.0f, 360.0f);
			vRays.push_back(tempRay);
		}

		// Recalculates the angle and Pushes the vector of rays to the new vector
		vRays.at(i).angle = atan2f(vRays.at(i).Vect.y - m_MainSprite.getPosition().y, vRays.at(i).Vect.x - m_MainSprite.getPosition().x) * (180.0f / 3.14f);
		vRays.at(i).angle = Util::setWithinRange(vRays.at(i).angle, 0.0f, 360.0f);
	}
	
	//For every ray position them in such a way that they are all within range
	for (int i = 2; i < vRays.size(); i++)
	{
		//If the limits overlap the maximum angle
		if ((vRays.at(0).angle >= 360 - fVisionCone && vRays.at(0).angle <= 360))
		{
			//If the ray lies within this zone
			if (vRays.at(i).angle >= 360 - fVisionCone && vRays.at(i).angle <= 360)
			{
				vRays.at(i).angle -= 360; //Move the ray to the lower limit
			}
		}
		//If the limits overlap the minimum angle
		if (vRays.at(1).angle >= 0 && vRays.at(1).angle <= fVisionCone)
		{
			//If the ray lies within this zone
			if (vRays.at(i).angle >= 0 && vRays.at(i).angle <= fVisionCone)
			{
				vRays.at(i).angle += 360; //Move the ray to the upper limit
			}
		}
	}

	//If the limits overlap the maximum angle
	if (vFinalRays.at(0).angle >= 360 - fVisionCone && vFinalRays.at(0).angle <= 360)
	{
		vFinalRays.at(0).angle -= 360; //Move the limit to the lower limit
	}
	//If the limits overlap the minimum angle
	if (vFinalRays.at(1).angle >= 0 && vFinalRays.at(1).angle <= fVisionCone)
	{
		vFinalRays.at(1).angle += 360; //Move the limit to the upper limit
	}

	//For every ray
	for (int i = 2; i < vRays.size(); i++)
	{
		//If the ray is within the vision cone add the ray to the vector of rays
		if (vRays.at(i).angle >= vFinalRays.at(0).angle && vRays.at(i).angle <= vFinalRays.at(1).angle)
		{
			vFinalRays.push_back(vRays.at(i));
		}
	}

	//Sort the rays into angle order
	QuickSort(vFinalRays, 0, vFinalRays.size());
	
	//Add the rays to the vertex array
	m_VisionRays.clear();
	m_VisionRays.resize((vFinalRays.size()+1));

	//Center Point
	sf::Vertex newVertex;
	newVertex.color = sf::Color(255, 255, 255, 70);
	newVertex.position = m_MainSprite.getPosition();
	m_VisionRays[0] = newVertex;

	//For every ray create a triangle 
	for (int i = 0; i < vFinalRays.size(); i++)
	{
		newVertex.position = vFinalRays.at(i).Vect;
		m_VisionRays[i + 1] = newVertex;
	}
}


std::vector<sf::Vector2f> Character::getCollisionLine(float fAngle)
{
	sf::Vector2f radiusLine = sf::Vector2f(m_MainSprite.getLocalBounds().width /2  * cos(fAngle * (3.14159265359 / 180)),
										   m_MainSprite.getLocalBounds().width / 2 * sin(fAngle * (3.14159265359 / 180)));

	std::vector<sf::Vector2f> newCollisionLine = {m_MainSprite.getPosition() - radiusLine , m_MainSprite.getPosition() + radiusLine};
	m_CollisionLine[0].position = newCollisionLine[0];
	m_CollisionLine[1].position = newCollisionLine[1];
	return newCollisionLine;
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

void Character::setTarget(Character* newTarget)
{
	m_CurrentTarget = newTarget;
	if (m_CurrentTarget != NULL)
	{
		m_CurrentState = AIM;
	}
}

void Character::setVision(bool bState)
{
	bDrawVision = bState;
}

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Draws the character Sprite
	if (bDrawVision)
	{
		target.draw(m_VisionRays);
	}
	target.draw(m_Weapon1);
	target.draw(m_MainSprite);
	target.draw(m_OrientationLine);
	target.draw(m_PathLine);
	target.draw(m_MovementLine);
	target.draw(m_CollisionLine);
	target.draw(m_AmmoBar);
	target.draw(m_HealthBar);
}