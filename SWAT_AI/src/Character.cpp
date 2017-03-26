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
	//Sets up the sprite
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
	m_MainSprite.setPosition(100, 100);
	//m_MainSprite.setOrigin((m_MainSprite.getLocalBounds().width / 2), (m_MainSprite.getLocalBounds().height / 2));

	//m_Weapon1.setPosition(sf::Vector2f(m_MainSprite.getPosition().x + (m_MainSprite.getLocalBounds().height * m_MainSprite.getScale().y), m_MainSprite.getPosition().y));
	//m_Weapon1.setOrigin(sf::Vector2f((m_MainSprite.getLocalBounds().width * m_MainSprite.getScale().x) / 2, -(m_MainSprite.getLocalBounds().height * m_MainSprite.getScale().y) / 2));

	//Sets up the Path line
	m_PathLine.setPrimitiveType(sf::LinesStrip); 

	//Sets up the orientation indication
	m_OrientationLine = sf::VertexArray(sf::Lines, 2);
	m_OrientationLine[0].position = m_MainSprite.getPosition();
	m_OrientationLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - (m_MainSprite.getLocalBounds().height / 2));
	for (int i = 0; i < m_OrientationLine.getVertexCount(); i++)
	{
		m_OrientationLine[i].color = sf::Color(255, 0, 255, 255);
	}

	//Sets up the movement indication
	m_MovementLine.setPrimitiveType(sf::LinesStrip);
	m_MovementLine.resize(2);
	for (int i = 0; i < m_MovementLine.getVertexCount(); i++)
	{
		m_MovementLine[i].color = sf::Color(0, 255, 255, 255);
	}

	//Sets up the collision indication
	m_CollisionLine.setPrimitiveType(sf::LinesStrip);
	m_CollisionLine.resize(2);
	for (int i = 0; i < m_CollisionLine.getVertexCount(); i++)
	{
		m_CollisionLine[i].color = sf::Color(255, 255, 0, 255);
	}

	//Sets up the vision cone
	m_VisionRays.setPrimitiveType(sf::TrianglesFan);

	m_HealthLevels.lower = 100;
	m_HealthLevels.upper = 100;

	//Sets up the health bar
	m_HealthBar.setSize(sf::Vector2f(70, 5));
	m_HealthBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_HealthBar.setLevelColor(sf::Color(0, 255, 0, 255));
	m_HealthBar.setLevel(m_HealthLevels.lower);
	m_HealthBar.setLimit(m_HealthLevels.upper);
	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 50));

	m_Weapon1.setAmmoLimit(30);
	m_Weapon1.setAmmo(30);

	//Sets up the ammo bar
	m_AmmoBar.setSize(sf::Vector2f(70, 5));
	m_AmmoBar.setBarColor(sf::Color(255, 0, 0, 255));
	m_AmmoBar.setLevelColor(sf::Color(255, 255, 0, 255));
	m_AmmoBar.setLevel(m_Weapon1.getAmmoLevels().lower);
	m_AmmoBar.setLimit(m_Weapon1.getAmmoLevels().upper);
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - 40));

	loadout.push_back(None);
	loadout.push_back(None);

	m_CurrentState = SEARCH_SWEEP;
	m_iAimingDirection = 1;
	m_CurrentTarget = NULL;
	m_bDrawVision = false;
	srand(time(NULL));

	//setClass(Assault, m_)

	m_SoundManager = SoundManager::getInstance();
	m_StepSound.setBuffer(*m_SoundManager->getSound(4));
	m_LoadoutSound.setBuffer(*m_SoundManager->getSound(7));
}

void Character::update()
{
	move();
	m_HealthBar.setSize(sf::Vector2f(m_MainSprite.getSize().x * 2, m_MainSprite.getSize().y / 8));
	m_AmmoBar.setSize(sf::Vector2f(m_MainSprite.getSize().x * 2, m_MainSprite.getSize().y / 8));

	m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - m_MainSprite.getSize().y - m_AmmoBar.getSize().y - (m_AmmoBar.getSize().y/2)));
	m_AmmoBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_AmmoBar.getSize().x / 2, m_MainSprite.getPosition().y - m_MainSprite.getSize().y));

	m_HealthBar.setLevel(m_HealthLevels.lower);
	m_HealthBar.setLimit(m_HealthLevels.upper);

	m_AmmoBar.setLevel(m_Weapon1.getAmmoLevels().lower);
	m_AmmoBar.setLimit(m_Weapon1.getAmmoLevels().upper);

	m_Weapon1.setPosition(getPosition());
	m_Weapon1.setSize(sf::Vector2f(getSize().x*(2.0f / 3.0f), getSize().y *(4.0f / 3.0f)));
	m_Weapon1.setOrigin(sf::Vector2f(m_Weapon1.getSize().x / 2.0f, -getSize().y / 2.0f));

	m_Weapon1.update();

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
			m_fAimingAngle = -fCone;
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
			m_fAimingAngle = Util::getAngle(Vect) - m_fMovementAngle - 90;// -m_fMovementAngle; // Finding the angle of the vector for the sprite
			m_fAimingAngle = Util::setWithinRange(m_fAimingAngle, 0.0f, 360.0f);

			//if (lazerChecks({ m_CurrentTarget->getCollisionLine(getRotation()).at(0),
			//				  m_CurrentTarget->getCollisionLine(getRotation()).at(1) }))
			//{
			m_Weapon1.shoot();
			//}

			if (m_CurrentTarget->getHealthData().lower <= 0)
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

void Character::move()
{
	if (!m_Path.empty())
	{
		const float kfMoveSpeed = 1.0f; //The amount of pixels the character moves per frame

																	//Sets the node to reach to be the next node in the path
		sf::Vector2f destination((((m_Path.at(0)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
			(((m_Path.at(0)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
		destination += m_CurrentMap->getPosition();

		sf::Vector2f velocity(destination - m_MainSprite.getPosition()); //Finds the distance between the next path node and the centre of the sprite

																		 //Finds the unit normal
		float fMagnitude = Util::magnitude(velocity);

		if (fMagnitude == 0)
		{
			m_Path.pop_front();
		}
		else
		{
			velocity /= fMagnitude;

			m_MovementLine[0].position = m_MainSprite.getPosition();
			m_MovementLine[1].position = m_MainSprite.getPosition() + (velocity * getSize().y / 2.0f);

			m_fMovementAngle = Util::getAngle(velocity) - 90;
			m_fMovementAngle = Util::setWithinRange(m_fMovementAngle, 0.0f, 360.0f);

			velocity *= kfMoveSpeed; //Multiplies it by the speed

			fDistanceSinceStep += Util::magnitude(velocity);
			m_MainSprite.setPosition(m_MainSprite.getPosition() + velocity); //Moves the Sprite

			if (m_MainSprite.getPosition().x >= destination.x - 10 &&
				m_MainSprite.getPosition().x <= destination.x + 10 &&
				m_MainSprite.getPosition().y <= destination.y + 10 &&
				m_MainSprite.getPosition().y >= destination.y - 10)
			{
				m_Path.pop_front();
			}

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

void Character::lookAt(sf::Vector2f position)
{
	sf::Vector2f rotVect (position - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse

	m_Weapon1.aim(position);

	rotVect /= Util::magnitude(rotVect); 

	m_MainSprite.setRotation(Util::getAngle(rotVect) + 90);

	rotVect *= (getSize().y / 2.0f);

	//Sets the rotation of the sprite and adjusts the orientation line according to the rotation
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
	return m_Weapon1.bulletChecks(vEdges);
}

bool Character::checkVisionCone(sf::Vector2f position)
{
	return false;
}

bool Character::lazerChecks(std::vector<sf::Vector2f>vEdges)
{
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

void Character::setupTextures()
{
	m_Weapon1.setSize(sf::Vector2f((getSize().x / 2), getSize().y / 2));
	m_Weapon1.setupTextures();
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

void Character::setGunTexture(sf::Texture* tex2)
{
	m_Weapon1.setTexture(tex2); //Applies the texture to the sprite.
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
	if (m_CurrentTarget != NULL)
	{
		m_CurrentState = AIM;
	}
}

void Character::setMuzzle(sf::Texture* tex2)
{
	m_Weapon1.setMuzzle(tex2);
}

void Character::setClass(classType newClassType, sf::Texture* GunTexture)
{
	currentClass = newClassType;
	setGunTexture(GunTexture);
	switch (newClassType)
	{
		case Assault:
			m_Weapon1.setDamage(sf::Vector2f(1.0f, 5.0f));
			m_Weapon1.setFireRate(0.2f);
			m_Weapon1.setRange(sf::Vector2f(200.0f, 500.0f));
			m_Weapon1.setAmmoLimit(30);
			m_Weapon1.setAmmo(0);
			m_Weapon1.setReloadTime(2.0f);
			m_Weapon1.setAccuracy(0.7f);
			m_Weapon1.setWeaponVolume(100);
			m_Weapon1.setShotSound(1);
			m_Weapon1.setReloadSound(6);
			break;

		case Support:
			m_Weapon1.setDamage(sf::Vector2f(1.0f, 3.0f));
			m_Weapon1.setFireRate(0.05f);
			m_Weapon1.setRange(sf::Vector2f(200.0f, 500.0f));
			m_Weapon1.setAmmoLimit(100);
			m_Weapon1.setAmmo(0);
			m_Weapon1.setReloadTime(5.0f);
			m_Weapon1.setAccuracy(0.3f);
			m_Weapon1.setWeaponVolume(150);
			m_Weapon1.setShotSound(0);
			m_Weapon1.setReloadSound(6);
			break;

		case Sniper:
			m_Weapon1.setDamage(sf::Vector2f(2.0f, 50.0f));
			m_Weapon1.setFireRate(1.0f);
			m_Weapon1.setRange(sf::Vector2f(400.0f, 1000.0f));
			m_Weapon1.setAmmoLimit(1);
			m_Weapon1.setAmmo(0);
			m_Weapon1.setReloadTime(4.0f);
			m_Weapon1.setAccuracy(0.8f);
			m_Weapon1.setWeaponVolume(200);
			m_Weapon1.setShotSound(2);
			m_Weapon1.setReloadSound(9);
			break;

		case Shotgunner:
			m_Weapon1.setDamage(sf::Vector2f(0.5f, 60.0f));
			m_Weapon1.setFireRate(2.0f);
			m_Weapon1.setRange(sf::Vector2f(0.0f, 250.0f));
			m_Weapon1.setAmmoLimit(8);
			m_Weapon1.setAmmo(0);
			m_Weapon1.setReloadTime(2.5f);
			m_Weapon1.setAccuracy(0.5f);
			m_Weapon1.setWeaponVolume(100);
			m_Weapon1.setShotSound(3);
			m_Weapon1.setReloadSound(8);
			break;
	}
	m_Weapon1.reload();
}

void Character::setLoadoutItem(int iIndex, loadoutItem itemType)
{
	m_LoadoutSound.play();
	bool bSame = true;
	while (bSame)
	{
		bSame = false;
		for (int i = 0; i < loadout.size(); i++)
		{
			if (i != iIndex)
			{
				if (loadout.at(i) == itemType && itemType != None)
				{
					itemType = getNextLoadoutItem(itemType);
					bSame = true;
				}
			}
		}
	}
	loadout.at(iIndex) = itemType;

	m_Weapon1.setLazer(false);
	m_Weapon1.setSilencer(false);
	m_Weapon1.setScope(false);

	for (int i = 0; i < loadout.size(); i++)
	{
		switch (loadout.at(i))
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

bool Character::reloading()
{
	return m_Weapon1.reloading();
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
	sf::Vector2f radiusLine = Util::rotateVect(sf::Vector2f(getSize().x / 2, getSize().x / 2), fAngle - 90);

	std::vector<sf::Vector2f> newCollisionLine = {m_MainSprite.getPosition() - radiusLine , m_MainSprite.getPosition() + radiusLine};
	m_CollisionLine[0].position = newCollisionLine[0];
	m_CollisionLine[1].position = newCollisionLine[1];
	return newCollisionLine;
}

classType Character::getClass()
{
	return currentClass;
}

loadoutItem Character::getLoadoutItem(int iIndex)
{
	return loadout.at(iIndex);
}

int Character::getLoadoutSize()
{
	return loadout.size();
}

loadoutItem Character::getNextLoadoutItem(loadoutItem itemType)
{
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

bool Character::isShooting()
{
	return m_Weapon1.isShooting();
}

Weapon* Character::getWeapon()
{
	return &m_Weapon1;
}

bool Character::stepTaken()
{
	float stepDist = getSize().x / 2.0f;
	if (fDistanceSinceStep > stepDist)
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

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_bDrawVision)
	{
		target.draw(m_VisionRays);
	}
	target.draw(m_Weapon1);
	target.draw(m_MainSprite);
	if (m_CurrentSettings->debugActive())
	{
		target.draw(m_OrientationLine);
		target.draw(m_PathLine);
		target.draw(m_MovementLine);
		target.draw(m_CollisionLine);
	}
	target.draw(m_AmmoBar);
	target.draw(m_HealthBar);
}