#include "../include/Character.h"
#include <iostream>
#include <math.h>

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

Character::Character()
{
	//Links the textures and sounds
	m_Textures = TextureLoader::getInstance();
	m_SoundManager = SoundManager::getInstance();

	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100, 100);

	//Sets up the Path line
	m_PathLine.setPrimitiveType(sf::LinesStrip); 

	//Sets up the orientation indication line
	m_OrientationLine = sf::VertexArray(sf::Lines, 2);
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - (m_MainSprite.getLocalBounds().height / 2));
	for (int i = 0; i < m_OrientationLine.getVertexCount(); i++)
	{
		m_OrientationLine[i].color = sf::Color(255, 0, 255, 255);
	}

	//Sets up the movement indication line
	m_MovementLine.setPrimitiveType(sf::LinesStrip);
	m_MovementLine.resize(2);
	for (int i = 0; i < m_MovementLine.getVertexCount(); i++)
	{
		m_MovementLine[i].color = sf::Color(0, 255, 255, 255);
	}

	//Sets up the collision indication line
	m_CollisionLine.setPrimitiveType(sf::LinesStrip);
	m_CollisionLine.resize(2);
	for (int i = 0; i < m_CollisionLine.getVertexCount(); i++)
	{
		m_CollisionLine[i].color = sf::Color(255, 255, 0, 255);
	}

	//Sets up the vision cone
	m_VisionRays.setPrimitiveType(sf::TrianglesFan);

	//Sets up the health bar
	m_HealthLevels.lower = 100;
	m_HealthLevels.upper = 100;
	m_HealthBar.setSize(sf::Vector2f(70, 5));
	m_HealthBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_HealthBar.setLevelColor(sf::Color(0, 255, 0, 255));
	m_HealthBar.setLevel(m_HealthLevels.lower);
	m_HealthBar.setLimit(m_HealthLevels.upper);
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));

	//Sets up the ammo bar
	m_AmmoBar.setSize(sf::Vector2f(70, 5));
	m_AmmoBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_AmmoBar.setLevelColor(sf::Color(255, 255, 0, 255));
	m_AmmoBar.setLevel(m_Weapon1.getAmmoLevels().lower);
	m_AmmoBar.setLimit(m_Weapon1.getAmmoLevels().upper);
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 40));

	//Sets up the characters loadout
	m_vLoadout.push_back(None);
	m_vLoadout.push_back(None);

	//Sets up the weapon
	m_Weapon1.setWeaponType(Assault);

	//Sets the up the sounds
	m_StepSound.setBuffer(*m_SoundManager->getSound(4));
	m_LoadoutSound.setBuffer(*m_SoundManager->getSound(7));

	//Sets up the textures
	m_DeathImage.setTexture(m_Textures->getTexture(32));

	//Sets the other variables
	m_CurrentState = SEARCH_SWEEP;
	m_iAimingDirection = 1;
	m_CurrentTarget = NULL;
	m_bDrawVision = false;
	m_bDead = false;

	//Sets up the seed for the randomiser
	srand(time(NULL)); 
}

void Character::update()
{
	//Moves the player 
	move();

	//Repositions,resizes and updates the ammo and health UI
	m_HealthBar.setSize(sf::Vector2f(m_MainSprite.getSize().x * 2, m_MainSprite.getSize().y / 8));
	m_AmmoBar.setSize(sf::Vector2f(m_MainSprite.getSize().x * 2, m_MainSprite.getSize().y / 8));

	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - m_MainSprite.getSize().y - m_AmmoBar.getSize().y - (m_AmmoBar.getSize().y/2)));
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_AmmoBar.getSize().x / 2, m_MainSprite.getPosition().y - m_MainSprite.getSize().y));

	m_HealthBar.setLevel(m_HealthLevels.lower);
	m_HealthBar.setLimit(m_HealthLevels.upper);

	m_AmmoBar.setLevel(m_Weapon1.getAmmoLevels().lower);
	m_AmmoBar.setLimit(m_Weapon1.getAmmoLevels().upper);

	//Repositions, resizes and updates the weapon
	m_Weapon1.setPosition(getPosition());
	m_Weapon1.setSize(sf::Vector2f(getSize().x*(2.0f / 3.0f), getSize().y *(4.0f / 3.0f)));
	m_Weapon1.setOrigin(sf::Vector2f(m_Weapon1.getSize().x / 2.0f, -getSize().y / 2.0f));
	m_Weapon1.update();

	switch (m_CurrentState)
	{
		case SEARCH_SWEEP:
		{
			float fCone = 30.0f; //Holds the breadth of the cone to turn between

			//Sweeps between the cone extents
			if (m_fAimingAngle > fCone)
			{
				m_fAimingAngle = fCone;
				m_iAimingDirection *= -1;
			}
			if (m_fAimingAngle < -fCone)
			{
				m_fAimingAngle = -fCone;
				m_iAimingDirection *= -1;
			}

			//Aims at the new amgle
			m_fAimingAngle += m_iAimingDirection;
			lookAt(m_fMovementAngle + m_fAimingAngle);
			break;
		}
		case AIM:
		{
			//If there is a target
			if (m_CurrentTarget != NULL)
			{
				lookAt(m_CurrentTarget->getPosition()); //Aim at the target

				//Aims the vision cone towards the new aiming direction
				sf::Vector2f Vect = m_CurrentTarget->getPosition() - m_MainSprite.getPosition();
				m_fAimingAngle = Util::getAngle(Vect) - m_fMovementAngle - 90; // Finding the angle of the vector for the sprite
				m_fAimingAngle = Util::setWithinRange(m_fAimingAngle, 0.0f, 360.0f);

				if (m_CurrentTarget->isDead()) //If the target is dead
				{
					m_CurrentTarget = NULL; //Clear the target
				}
				else
				{
					m_Weapon1.shoot(); //Shoots the weapon
				}
			}
			else
			{
				m_CurrentState = SEARCH_SWEEP; //Switch states
			}
			break;
		}
	}

	//If the characters health is empty
	if (m_HealthLevels.lower <= 0)
	{
		//Kill the character and set up the death sprite
		m_bDead = true;
		m_DeathImage.setPosition(m_MainSprite.getPosition());
		m_DeathImage.setSize(m_MainSprite.getSize());
		m_DeathImage.setOrigin(m_DeathImage.getSize()/2.0f);
	}
}

void Character::move()
{
	if (!m_Path.empty()) //If there is a path to follow
	{
		const float kfMoveSpeed = 1.0f; //The amount of pixels the character moves per frame

		//Sets the node to reach to be the next node in the path
		sf::Vector2f destination((((m_Path.at(0)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
								 (((m_Path.at(0)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
		destination += m_CurrentMap->getPosition();

		sf::Vector2f velocity(destination - m_MainSprite.getPosition()); //Finds the distance between the next path node and the centre of the sprite

		//If the node has already been reached move to the next node
		float fMagnitude = Util::magnitude(velocity);
		if (fMagnitude == 0)
		{
			m_Path.pop_front();
		}
		else
		{
			//Finds the unit normal
			velocity /= fMagnitude;

			//Sets up the movement line
			m_MovementLine[0].position = m_MainSprite.getPosition();
			m_MovementLine[1].position = m_MainSprite.getPosition() + (velocity * getSize().y / 2.0f);

			//Gets the angle the character is heading towards for later use
			m_fMovementAngle = Util::getAngle(velocity) - 90;
			m_fMovementAngle = Util::setWithinRange(m_fMovementAngle, 0.0f, 360.0f);

			velocity *= kfMoveSpeed; //Multiplies the direction by the speed

			fDistanceSinceStep += Util::magnitude(velocity);
			m_MainSprite.setPosition(m_MainSprite.getPosition() + velocity); //Moves the Sprite

			//If the node has been reached then move to the next node
			if (m_MainSprite.getPosition().x >= destination.x - 10 &&
				m_MainSprite.getPosition().x <= destination.x + 10 &&
				m_MainSprite.getPosition().y <= destination.y + 10 &&
				m_MainSprite.getPosition().y >= destination.y - 10)
			{
				m_Path.pop_front();

				//Sets up the path line ready to start drawing a new path
				m_PathLine.clear();
				m_PathLine.resize(m_Path.size());
				for (int i = 0; i < m_Path.size(); i++)
				{
					m_PathLine[i] = sf::Vertex(m_CurrentMap->getPosition() + sf::Vector2f(
						(((m_Path.at(i)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
						(((m_Path.at(i)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2))),
						sf::Color(0, 255, 0, 255));
				}
			}
		}
	}
}

void Character::lookAt(sf::Vector2f position)
{
	sf::Vector2f rotVect (position - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse

	//Aims the weapon towards that position
	m_Weapon1.aim(position);

	rotVect /= Util::magnitude(rotVect); 

	m_MainSprite.setRotation(Util::getAngle(rotVect) + 90); //Rotates the sprite towards the target

	//Updates the orientation line
	rotVect *= (getSize().y / 2.0f); //Sets the size of the rotation for the orientation line
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = m_MainSprite.getPosition() + rotVect;
}

void Character::lookAt(float fAngle)
{
	//Finding the vector between the character's center and the mouse
	sf::Vector2f rotVect(Util::rotateVect(sf::Vector2f(getSize().y / 2, getSize().y / 2), fAngle));

	//Sets the rotation of the sprite and adjusts the orientation line according to the rotation
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = m_MainSprite.getPosition() + rotVect;

	//Points the character and the weapon towards the location
	m_MainSprite.setRotation(fAngle);
	m_Weapon1.aim(fAngle);
}

void Character::visionCalculation(std::vector<sf::Vector2f>vEdges)
{
	std::vector<Ray> vRays;
	std::vector<Ray> vFinalRays;
	Ray tempRay;
	float fViewDistance = 2000.0f;
	float fVisionCone = 45.0f;

	//If a scope is being used the vision is refined
	if (m_Weapon1.usingScope())
	{
		fVisionCone -= 20.0f;
	}

	//Edges
	for (int i = -1; i <= 1; i += 2)
	{
		float fRotAngle = m_fMovementAngle + m_fAimingAngle + (i*fVisionCone) + 90;
		fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);

		sf::Vector2f rayVect = Util::rotateVect(sf::Vector2f(fViewDistance, fViewDistance), fRotAngle - 90);

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
		float fRotAngle = Util::getAngle(rayVect);
		fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);

		rayVect = Util::rotateVect(sf::Vector2f(fViewDistance, fViewDistance), fRotAngle - 90);

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
		vRays.at(i).Vect = Util::findLowestIntersect(vEdges, viewRay).second; //Sets the new length of the ray
		vFinalRays.push_back(vRays.at(i));
	}

	//For every ray
	for (int i = 2; i < vRays.size(); i++)
	{
		//Set the lowest intersect to the rays location
		std::vector<sf::Vector2f> viewRay = { m_MainSprite.getPosition(), vRays.at(i).Vect };
		vRays.at(i).Vect = Util::findLowestIntersect(vEdges, viewRay).second; //Sets the new length of the ray

																			  //If the distance to the corner being pointed at is less than the ray distance then the corner is added to get the correct effect
		if (Util::magnitude(vRays.at(i).OriginalVect - m_MainSprite.getPosition()) < Util::magnitude(vRays.at(i).Vect - m_MainSprite.getPosition()))
		{
			tempRay.Vect = vRays.at(i).OriginalVect;
			tempRay.angle = Util::setWithinRange(vRays.at(i).angle, 0.0f, 360.0f);
			tempRay.OriginalVect = sf::Vector2f(fViewDistance, fViewDistance);
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
	m_VisionRays.resize((vFinalRays.size() + 1));

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

float Character::bulletChecks(std::vector<sf::Vector2f>vEdges)
{
	return m_Weapon1.bulletChecks(vEdges); //Returns the damage the weapon deals if it hits
}

bool Character::lazerChecks(std::vector<sf::Vector2f>vEdges)
{
	//Finds where the lazer intersects and updates its position
	std::vector<sf::Vector2f> lazerRay = { m_Weapon1.getPosition(), m_Weapon1.getIntersect() };
	std::pair<bool, sf::Vector2f> lowestIntersect = Util::findLowestIntersect(vEdges, lazerRay);

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

//Setters
void Character::setHealth(float fLevel)
{
	if (fLevel < 0)
	{
		fLevel = 0;
	}
	m_HealthLevels.lower = fLevel;
	m_HealthBar.setLevel(fLevel);
}

void Character::setPath(std::deque<Node*> newPath)
{
	m_Path = newPath;
}

void Character::setVision(bool bState)
{
	m_bDrawVision = bState;
}

void Character::setTarget(Character* newTarget)
{
	m_CurrentTarget = newTarget;
	if (m_CurrentTarget != NULL) //If the target isnt empty then aim at the target
	{
		m_CurrentState = AIM;
	}
}

void Character::setClass(classType newClassType)
{
	m_CurrentClass = newClassType;
	m_Weapon1.setWeaponType(newClassType);
	m_Weapon1.reload();
}

void Character::setLoadoutItem(int iIndex, loadoutItem itemType)
{
	m_LoadoutSound.play();

	//If skip a loadout item if that item has been selected in another slot
	bool bSame = true;
	while (bSame)
	{
		bSame = false;

		//For every loadout slot
		for (int i = 0; i <  m_vLoadout.size(); i++)
		{
			if (i != iIndex) //If the slot isnt the current slot being changed
			{
				if (m_vLoadout.at(i) == itemType && itemType != None) //Check if the item is the same
				{
					itemType = getNextLoadoutItem(itemType); //If it is then get the next loadout item
					bSame = true;
				}
			}
		}
	}
	m_vLoadout.at(iIndex) = itemType;

	//Resets the weapon attachments 
	m_Weapon1.setLazer(false);
	m_Weapon1.setSilencer(false);
	m_Weapon1.setScope(false);

	//For every loadout slot
	for (int i = 0; i <  m_vLoadout.size(); i++)
	{
		//If theyre any of the items given then activate the attachments
		switch (m_vLoadout.at(i))
		{
			case Lazer:
				m_Weapon1.setLazer(true);
				break;

			case Silencer:
				m_Weapon1.setSilencer(true);
				break;

			case Scope:
				m_Weapon1.setScope(true);
				break;

			case None:
				break;
		}
	}
}

//Getters
float Character::getRotation()
{
	return m_MainSprite.getRotation();
}

int Character::getLoadoutSize()
{
	return m_vLoadout.size();
}

Util::Limits Character::getHealthData()
{
	return m_HealthLevels;
}

Util::Limits Character::getAmmoData()
{
	return m_Weapon1.getAmmoLevels();
}

std::vector<sf::Vector2f> Character::getCollisionLine(float fAngle)
{
	//Gets the radius and rotates it to the current orientation 
	sf::Vector2f radiusLine = Util::rotateVect(sf::Vector2f(getSize().x / 2, getSize().x / 2), fAngle - 90);

	//The orientation is then applied to the position to get aan orientation spanning the characters width
	std::vector<sf::Vector2f> vNewCollisionLine = { m_MainSprite.getPosition() - radiusLine , m_MainSprite.getPosition() + radiusLine };
	m_CollisionLine[0].position = vNewCollisionLine[0];
	m_CollisionLine[1].position = vNewCollisionLine[1];
	return vNewCollisionLine;
}

classType Character::getClass()
{
	return m_CurrentClass;
}

loadoutItem Character::getLoadoutItem(int iIndex)
{
	return m_vLoadout.at(iIndex);
}

loadoutItem Character::getNextLoadoutItem(loadoutItem itemType)
{
	//Switches to the next loadout item in line
	switch (itemType)
	{
		case Lazer:
			return Silencer;
			break;

		case Silencer:
			return Scope;
			break;

		case Scope:
			return None;
			break;

		case None:
			return Lazer;
			break;
	}
}

Weapon* Character::getWeapon()
{
	return &m_Weapon1;
}

bool Character::reloading()
{
	return m_Weapon1.reloading();
}

bool Character::isShooting()
{
	return m_Weapon1.isShooting();
}

bool Character::stepTaken()
{
	float fStepDist = getSize().x / 2.0f;

	//If the distance taken is farther than the step distance then play a sound and restart the step
	if (fDistanceSinceStep > fStepDist)
	{
		m_StepSound.play();
		fDistanceSinceStep = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool Character::isDead()
{
	return m_bDead;
}

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!m_bDead)
	{
		if (m_bDrawVision)
		{
			target.draw(m_VisionRays);
		}

		target.draw(m_Weapon1);
	}

	target.draw(m_MainSprite);

	if (m_CurrentSettings->debugActive())
	{
		target.draw(m_OrientationLine);
		target.draw(m_PathLine);
		target.draw(m_MovementLine);
		target.draw(m_CollisionLine);
	}

	if (!m_bDead)
	{
		target.draw(m_AmmoBar);
		target.draw(m_HealthBar);
	}
	else
	{
		target.draw(m_DeathImage);
	}
	
}