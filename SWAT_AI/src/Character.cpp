#include "../include/Character.h"
#include <iostream>
#include <math.h>

struct Ray //Used for ray casting algorithms
{
	float angle;
	sf::Vector2f vect;
	sf::Vector2f originalVect;
};

int partition(std::vector<Ray>& vRays, int iStartIndex, int iEndIndex)
{
	//Set the divider to the first item and uses the start item as a pivot
	int iPivot = iStartIndex;
	int iDivider = iStartIndex;

	//For every item after the divider
	for (int iCurrentElement = iDivider + 1; iCurrentElement < iEndIndex; iCurrentElement++)
	{
		//If the angle is less than the angle of the pivot
		if (vRays.at(iCurrentElement).angle < vRays.at(iPivot).angle)
		{
			//Then swap the item next to the divider and the item 
			//being checked then move the divider along 1 unit to have the lower item on the left side of the divider
			iDivider += 1;
			iter_swap(vRays.begin() + iDivider, vRays.begin() + iCurrentElement);
		}
	}

	//Place the pivot item in the center of these items by switching the lower side of the wall and the starting index
	iter_swap(vRays.begin() + iDivider, vRays.begin() + iPivot);
	return iDivider;
}

void QuickSort(std::vector<Ray>& vRays, int iStartIndex, int iEndIndex)
{
	//Keeps recursively looping until the divider surpasses the end of the list
	if (iStartIndex < iEndIndex)
	{
		int iDivider = partition(vRays, iStartIndex, iEndIndex); //Finds where the divider lies for the next level of sorting
		QuickSort(vRays, iStartIndex, iDivider); //Sorts the left side of the divider
		QuickSort(vRays, iDivider + 1, iEndIndex);  //Sorts the right side of the divider
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
	m_VisionLines.setPrimitiveType(sf::Lines);

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
	m_DeathSound.setBuffer(*m_SoundManager->getSound(10));

	//Sets up the textures
	m_DeathImage.setTexture(m_Textures->getTexture(32));

	//Sets the other variables
	m_MovementState = IDLE;
	m_AimingState = SEARCH_SWEEP;
	patrolNode = 0;
	patrolDirection = 1;
	m_iAimingDirection = 1;
	m_CurrentTarget = NULL;
	m_bDrawVision = false;
	m_bDead = false;
	spinAmount = 0;

	//Sets up the seed for the randomiser
	srand(time(NULL)); 

	//Initialises the path finder
	m_Pathfinder.setup();
}

void Character::update()
{
	//Updates any sound waves that are expanding
	for (int i = 0; i < m_Waves.size(); i++)
	{
		if (m_Waves.at(i) != NULL)
		{
			m_Waves.at(i)->update();
			if (m_Waves.at(i)->isDone()) //If the wave has finished delete it
			{
				delete m_Waves.at(i);
				m_Waves.at(i) = NULL;
				m_Waves.erase(m_Waves.begin() + i);
			}
		}
	}

	if (!m_bDead)
	{
		//Moves the player 
		move();
		visionCalculation();//Perform checks for the vision cone

		if (stepTaken())
		{
			m_StepSound.play();
			sf::Vector2u CharacterTile(((int)getPosition().x - getPosition().x) / (int)m_CurrentMap->getTileSize().x,
				((int)getPosition().y - getPosition().y) / (int)m_CurrentMap->getTileSize().y);

			//Changes the amount of sound footsteps make if the character treads on different materials
			if (m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'B' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'R' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'G')
			{
				m_Waves.push_back(new soundWave(150, 3.0f, 1.0f, getPosition()));
			}
			else if (m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'C' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'K' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'F')
			{
				m_Waves.push_back(new soundWave(300, 3.0f, 1.0f, getPosition()));
			}
			fDistanceSinceStep = 0;
		}

		//Repositions,resizes and updates the ammo and health UI
		m_HealthBar.setSize(sf::Vector2f(m_MainSprite.getSize().x * 2, m_MainSprite.getSize().y / 8));
		m_AmmoBar.setSize(sf::Vector2f(m_MainSprite.getSize().x * 2, m_MainSprite.getSize().y / 8));

		m_HealthBar.setPosition(sf::Vector2f(m_MainSprite.getPosition().x - m_HealthBar.getSize().x / 2, m_MainSprite.getPosition().y - m_MainSprite.getSize().y - m_AmmoBar.getSize().y - (m_AmmoBar.getSize().y / 2)));
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

		switch (m_AimingState)
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
			case SEARCH_SPIN:
			{
				if (m_CurrentTarget == NULL)
				{
					spinAmount++;
					if (spinAmount >= 360)
					{
						spinAmount = 0;
						m_AimingState = SEARCH_SWEEP;
					}

					m_fAimingAngle = spinAmount;
					lookAt(m_fMovementAngle + m_fAimingAngle);
				}
				else
				{
					spinAmount = 0;
					m_AimingState = AIM;
				}
				break;
			}
			case AIM:
			{
				//If there is a target
				if (m_CurrentTarget != NULL)
				{
					lookAt(m_CurrentTarget->getPosition()); //Aim at the target

					if (m_CurrentTarget->isDead()) //If the target is dead
					{
						m_CurrentTarget = NULL; //Clear the target
					}
					else
					{
						m_Weapon1.shoot(); //Shoots the weapon
						if (m_Weapon1.isShooting())
						{
							//Output a shooting wave from the end of the characters gun
							m_Waves.push_back(new soundWave(m_Weapon1.getWeaponVolume(), 10.0f, 1.0f, m_Weapon1.getWeaponEnd()));
						}
					}
				}
				else
				{
					m_AimingState = SEARCH_SWEEP; //Switch states

					if (m_PatrolPath.size() > 0)
					{
						m_MovementState = PATROL;
						sf::Vector2f destination((((m_PatrolPath.at(patrolNode)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
							(((m_PatrolPath.at(patrolNode)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
						destination += m_CurrentMap->getPosition();
						setPath(getPosition(), destination);
					}
					else
					{
						m_MovementState = IDLE;
					}
				}
				break;
			}
			case FOCUS:
			{
				lookAt(focusPoint);
				if (m_MovementState != INVESTIGATING)
				{
					m_AimingState = SEARCH_SWEEP;
				}
				break;
			}
		}

		switch (m_MovementState)
		{
			case IDLE:
			{
				if (m_AimingState == SEARCH_SWEEP)
				{
					if (m_Path.size() > 0)
					{
						m_MovementState = MOVE_TO_SPOT;
					}
					else if (m_PatrolPath.size() > 0)
					{
						m_MovementState = PATROL;
						sf::Vector2f destination((((m_PatrolPath.at(patrolNode)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
							(((m_PatrolPath.at(patrolNode)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
						destination += m_CurrentMap->getPosition();
						setPath(getPosition(), destination);
					}
				}
				break;
			}

			case MOVE_TO_SPOT:
			{
				if (m_Path.empty())
				{
					m_MovementState = IDLE;
					m_AimingState = SEARCH_SWEEP;
				}
				break;
			}

			case INVESTIGATING:
			{
				if (m_AimingState != AIM)
				{
					m_AimingState = FOCUS;
					if (Util::magnitude(getPosition() - m_InvestigationArea) < getSize().y)
					{
						m_MovementState = IDLE;
					}
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
			m_DeathImage.setOrigin(m_DeathImage.getSize() / 2.0f);
			m_DeathSound.play();
		}
	}
}

void Character::move()
{
	std::deque<Node*>* usePath = NULL;
	int iMoveType = 0;

	if (!m_Path.empty()) //If there is a path to follow
	{
		usePath = &m_Path;
		iMoveType = 1;
	}
	else if (m_MovementState == PATROL) //Otherwise check for a patrol
	{
		if (!m_PatrolPath.empty())
		{
			usePath = &m_PatrolPath;
			iMoveType = 2;
		}
	}

	if (usePath != NULL)
	{
		const float kfMoveSpeed = 1.0f; //The amount of pixels the character moves per frame
		sf::Vector2f destination;
										//Sets the node to reach to be the next node in the path
		if (iMoveType == 2)
		{
			destination = sf::Vector2f((((m_PatrolPath.at(patrolNode)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
				(((m_PatrolPath.at(patrolNode)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
		}
		else
		{
			destination = sf::Vector2f((((m_Path.at(0)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
				(((m_Path.at(0)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
		}
		destination += m_CurrentMap->getPosition();

		sf::Vector2f velocity(destination - m_MainSprite.getPosition()); //Finds the distance between the next path node and the centre of the sprite

																		 //If the node has already been reached move to the next node
		float fMagnitude = Util::magnitude(velocity);
		if (fMagnitude == 0)
		{
			if (iMoveType == 2)
			{
				if (patrolNode == m_PatrolPath.size() - 1 && m_PatrolPath.at(0)->parent == NULL)
				{
					patrolDirection = -1;
				}
				else if (patrolNode == 0 && m_PatrolPath.at(0)->parent == NULL)
				{
					patrolDirection = 1;
				}

				if (patrolNode == m_PatrolPath.size() - 1 && m_PatrolPath.at(0)->parent != NULL)
				{
					patrolNode = 0;
				}
				else
				{
					patrolNode += patrolDirection;
				}
			}
			else
			{
				m_Path.pop_front();
			}
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
			if (m_MainSprite.getPosition().x >= destination.x - 1 &&
				m_MainSprite.getPosition().x <= destination.x + 1 &&
				m_MainSprite.getPosition().y <= destination.y + 1 &&
				m_MainSprite.getPosition().y >= destination.y - 1)
			{
				if (iMoveType == 2)
				{
					if (patrolNode == m_PatrolPath.size() - 1 && m_PatrolPath.at(0)->parent == NULL)
					{
						patrolDirection = -1;
					}
					else if (patrolNode == 0 && m_PatrolPath.at(0)->parent == NULL)
					{
						patrolDirection = 1;
					}

					if (patrolNode == m_PatrolPath.size() - 1 && m_PatrolPath.at(0)->parent != NULL)
					{
						patrolNode = 0;
					}
					else
					{
						patrolNode += patrolDirection;
					}
				}
				else
				{
					m_Path.pop_front();
				}

				//Sets up the path line ready to start drawing a new path
				m_PathLine.clear();
				if (m_CurrentSettings->debugActive())
				{
					m_PathLine.resize(usePath->size());
					for (int i = 0; i < usePath->size(); i++)
					{
						m_PathLine[i] = sf::Vertex(m_CurrentMap->getPosition() + sf::Vector2f(
							(((usePath->at(i)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
							(((usePath->at(i)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2))),
							sf::Color(0, 255, 0, 255));
					}
				}
			}
		}
	}
}

void Character::lookAt(sf::Vector2f position)
{
	sf::Vector2f rotVect (position - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse

	m_fAimingAngle = Util::getAngle(rotVect) - m_fMovementAngle - 90; // Finding the angle of the vector for the sprite
	m_fAimingAngle = Util::setWithinRange(m_fAimingAngle, 0.0f, 360.0f);

	//Aims the weapon towards that position
	m_Weapon1.aim(position);

	rotVect /= Util::magnitude(rotVect); 

	m_MainSprite.setRotation(Util::getAngle(rotVect) + 90); //Rotates the sprite towards the target

	//Updates the orientation line
	if (m_CurrentSettings->debugActive())
	{
		rotVect *= (getSize().y / 2.0f); //Sets the size of the rotation for the orientation line
		m_OrientationLine[0].position = m_MainSprite.getPosition();
		m_OrientationLine[1].position = m_MainSprite.getPosition() + rotVect;
	}
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

void Character::visionCalculation()
{
	/*
	std::vector<Ray> vRays;
	std::vector<Ray> vFinalRays;
	Ray tempRay;
	float fViewDistance = 2000.0f;
	float fVisionCone = 45.0f;
	int iCurrentEdge = -1;
	int iPreviousEdge = -1;

	for (int i = 0; i < m_CurrentMap->m_vCorners.size(); i++)
	{
		//Create a ray pointing towards the corner given
		sf::Vector2f rayVect = (*m_CurrentMap->m_vCorners.at(i)) - m_MainSprite.getPosition();
		rayVect /= Util::magnitude(rayVect);

		//Gets the angle of the vector
		float fRotAngle = Util::getAngle(rayVect);
		fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);
		tempRay.angle = fRotAngle;

		tempRay.originalVect = (*m_CurrentMap->m_vCorners.at(i));

		rayVect = Util::rotateVect(sf::Vector2f(fViewDistance, fViewDistance), fRotAngle - 90);
		tempRay.vect = m_MainSprite.getPosition() + rayVect;

		vRays.push_back(tempRay);
	}
	QuickSort(vRays,0,vRays.size());

	for (int i = 0; i < vRays.size(); i++)
	{
		//Set the lowest intersect to the rays location
		std::vector<sf::Vector2f> viewRay = { m_MainSprite.getPosition(), vRays.at(i).vect };
		Util::intersectData tempData = Util::findLowestIntersect2(&m_CurrentMap->m_vEdges, viewRay);
		vRays.at(i).vect = tempData.collisionPoint; //Sets the new length of the ray

		if (tempData.index != iCurrentEdge && tempData.index != -1)
		{
			if (i > 0 && iPreviousEdge != -1)
			{
				//float fRotAngle = Util::getAngle(vRays.at(i - 1).vect - m_MainSprite.getPosition());
				//fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);
				//vRays.at(i - 1).angle = fRotAngle; 
				vFinalRays.push_back(tempRay);
				tempRay.vect = tempRay.originalVect;

				tempRay.vect = Util::lineIntersect(*m_CurrentMap->m_vEdges.at(iPreviousEdge).first, *m_CurrentMap->m_vEdges.at(iPreviousEdge).second, m_MainSprite.getPosition(), tempRay.vect); //Sets the new length of the ray

				vFinalRays.push_back(tempRay);
			}

			iPreviousEdge = iCurrentEdge;
			iCurrentEdge = tempData.index;
			
			float fRotAngle = Util::getAngle(vRays.at(i).vect - m_MainSprite.getPosition());
			fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);
			vRays.at(i).angle = fRotAngle;

			tempRay = vRays.at(i);

			vFinalRays.push_back(tempRay);
		}
		
		//If the distance to the corner being pointed at is less than the ray distance then the corner is added to get the correct effect
		if (Util::magnitude(tempRay.originalVect - m_MainSprite.getPosition()) < Util::magnitude(tempRay.vect - m_MainSprite.getPosition()))
		{
			tempRay.vect = vRays.at(i).originalVect;
			tempRay.angle = Util::setWithinRange(vRays.at(i).angle, 0.0f, 360.0f);
			tempRay.originalVect = sf::Vector2f(fViewDistance, fViewDistance);
			vFinalRays.push_back(tempRay);
		}
		else
		{

		}

		// Recalculates the angle and Pushes the vector of rays to the new vector
		//vRays.at(i).angle = atan2f(vRays.at(i).vect.y - m_MainSprite.getPosition().y, vRays.at(i).vect.x - m_MainSprite.getPosition().x) * (180.0f / 3.14f);
		//vRays.at(i).angle = Util::setWithinRange(vRays.at(i).angle, 0.0f, 360.0f);

		//vRays.push_back(tempRay);
	}

	for (int i = 0; i < vFinalRays.size(); i++)
	{
		vFinalRays.at(i).angle = atan2f(vFinalRays.at(i).vect.y - m_MainSprite.getPosition().y, vFinalRays.at(i).vect.x - m_MainSprite.getPosition().x) * (180.0f / 3.14f);
		vFinalRays.at(i).angle = Util::setWithinRange(vFinalRays.at(i).angle, 0.0f, 360.0f);
	}

	//Sort the rays into angle order
	QuickSort(vFinalRays, 0, vFinalRays.size());

	//Add the rays to the vertex array
	m_VisionRays.clear();
	m_VisionRays.resize((vFinalRays.size() + 1));

	m_VisionLines.clear();
	m_VisionLines.resize((vFinalRays.size() + 1) * 2);

	//Center Point
	sf::Vertex newVertex;
	sf::Vertex newVertex2;
	newVertex.color = sf::Color(255, 255, 255, 70);
	newVertex.position = m_MainSprite.getPosition();
	m_VisionRays[0] = newVertex;

	//For every ray create a triangle 
	for (int i = 0; i < vFinalRays.size(); i++)
	{
		newVertex.position = vFinalRays.at(i).vect;
		m_VisionRays[i + 1] = newVertex;

		newVertex2.position = getPosition();
		newVertex2.color = sf::Color(0, 255, 255, 255);
		m_VisionLines[(2 * i)] = newVertex2;
		newVertex2.position = newVertex.position;
		m_VisionLines[(2 * i) + 1] = newVertex2;
	}*/

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
		tempRay.vect = m_MainSprite.getPosition() + rayVect;
		tempRay.originalVect = sf::Vector2f(fViewDistance, fViewDistance);
		tempRay.angle = fRotAngle;
		vRays.push_back(tempRay);
	}

	//Generate the rays
	for (int i = 0; i < m_CurrentMap->m_vCorners.size(); i++)
	{
		//Create a ray pointing towards the corner given
		sf::Vector2f rayVect = (*m_CurrentMap->m_vCorners.at(i)) - m_MainSprite.getPosition();

		//Gets the angle of the vector
		float fRotAngle = Util::getAngle(rayVect);
		fRotAngle = Util::setWithinRange(fRotAngle, 0.0f, 360.0f);
		tempRay.angle = fRotAngle;

		tempRay.originalVect = (*m_CurrentMap->m_vCorners.at(i));

		rayVect = Util::rotateVect(sf::Vector2f(fViewDistance, fViewDistance), fRotAngle - 90);
		tempRay.vect = m_MainSprite.getPosition() + rayVect;

		vRays.push_back(tempRay);

		rayVect = Util::rotateVect(sf::Vector2f(fViewDistance, fViewDistance), fRotAngle - 90.1);
		tempRay.vect = m_MainSprite.getPosition() + rayVect;

		vRays.push_back(tempRay);

		rayVect = Util::rotateVect(sf::Vector2f(fViewDistance, fViewDistance), fRotAngle - 89.9);
		tempRay.vect = m_MainSprite.getPosition() + rayVect;

		vRays.push_back(tempRay);
	}

	//Check for ray collisions
	//Edges of vision cone
	for (int i = 0; i < 2; i++)
	{
		//Set the lowest intersect to the rays location
		std::vector<sf::Vector2f> viewRay = { m_MainSprite.getPosition(), vRays.at(i).vect };
		vRays.at(i).vect = Util::findLowestIntersect(&m_CurrentMap->m_vEdges, viewRay); //Sets the new length of the ray
		vFinalRays.push_back(vRays.at(i));
	}

	//For every ray
	for (int i = 2; i < vRays.size(); i++)
	{
		//Set the lowest intersect to the rays location
		std::vector<sf::Vector2f> viewRay = { m_MainSprite.getPosition(), vRays.at(i).vect };
		vRays.at(i).vect = Util::findLowestIntersect(&m_CurrentMap->m_vEdges, viewRay); //Sets the new length of the ray

		// Recalculates the angle and Pushes the vector of rays to the new vector
		vRays.at(i).angle = atan2f(vRays.at(i).vect.y - m_MainSprite.getPosition().y, vRays.at(i).vect.x - m_MainSprite.getPosition().x) * (180.0f / 3.14f);
		vRays.at(i).angle = Util::setWithinRange(vRays.at(i).angle, 0.0f, 360.0f);
	}

	for (int i = 2; i < vRays.size(); i++)
	{
		vRays.at(i).angle = Util::setWithinRange(vRays.at(i).angle, 0, 360);
	}

	for (int i =0; i < vFinalRays.size(); i++)
	{
		vFinalRays.at(i).angle = Util::setWithinRange(vFinalRays.at(i).angle, 0, 360);
	}

	//If the vision cone goes past the 0 / 360 crossover
	if (vFinalRays.at(0).angle > vFinalRays.at(1).angle)
	{
		//Put the rays into a consecutive form
		for (int i = 2; i < vRays.size(); i++)
		{
			if (vRays.at(i).angle > vFinalRays.at(0).angle)
			{
				vRays.at(i).angle -= 360;
			}
		}
		vFinalRays.at(0).angle -= 360;
	}

	//Check if the rays lie within the range
	for (int i = 2; i < vRays.size(); i++)
	{
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

	m_VisionLines.clear();
	m_VisionLines.resize((vFinalRays.size() + 1)*2);

	//Center Point
	sf::Vertex newVertex;
	sf::Vertex newVertex2;
	newVertex.color = sf::Color(255, 255, 255, 70);
	newVertex.position = m_MainSprite.getPosition();
	m_VisionRays[0] = newVertex;

	//For every ray create a triangle 
	for (int i = 0; i < vFinalRays.size(); i++)
	{
		newVertex.position = vFinalRays.at(i).vect;
		m_VisionRays[i + 1] = newVertex;
		
		if (m_CurrentSettings->debugActive())
		{
			newVertex2.position = getPosition();
			newVertex2.color = sf::Color(0, 255, 255, 255);
			m_VisionLines[(2 * i)] = newVertex2;
			newVertex2.position = newVertex.position;
			m_VisionLines[(2 * i) + 1] = newVertex2;
		}
	}
}

void Character::bulletEdgeChecks()
{
	if (m_Weapon1.isShooting())
	{
		//Finds where the lazer intersects and updates its position
		std::vector<sf::Vector2f> bulletRay = { m_Weapon1.getBullet()[0].position, m_Weapon1.getBullet()[1].position };
		sf::Vector2f lowestIntersect = Util::findLowestIntersect(&m_CurrentMap->m_vEdges, bulletRay);
		m_Weapon1.setBullet(lowestIntersect);
	}
}

void Character::lazerEdgeChecks()
{
	std::vector<sf::Vector2f> lazerRay = { m_Weapon1.getPosition(), m_Weapon1.getIntersect() };
	sf::Vector2f lowestIntersect = Util::findLowestIntersect(&m_CurrentMap->m_vEdges, lazerRay);
	m_Weapon1.setIntersect(lowestIntersect);
}

void Character::bulletChecks(std::vector<Character*> vCharSet)
{
	if (m_Weapon1.isShooting())
	{
		//Finds where the lazer intersects and updates its position
		sf::Vector2f lowestIntersect(m_Weapon1.getBullet()[1].position);
		std::vector<sf::Vector2f> bulletRay = { m_Weapon1.getPosition(), lowestIntersect };

		int iOutput = -1;
		for (int i = 0; i < vCharSet.size(); i++)
		{
			//Checks where the ray and the edge intersect
			sf::Vector2f currentIntersect = Util::lineIntersect(vCharSet.at(i)->getCollisionLine(getRotation()).first,
				vCharSet.at(i)->getCollisionLine(getRotation()).second,
				bulletRay.at(0),
				bulletRay.at(1));

			//If the ray is shorter than the previous rays then set the ray to be the shortest ray
			if (Util::magnitude(currentIntersect - getPosition()) < Util::magnitude(lowestIntersect - getPosition()))
			{
				iOutput = i;
				lowestIntersect = currentIntersect;
			}
		}
		if (iOutput != -1)
		{
			vCharSet.at(iOutput)->setHealth(vCharSet.at(iOutput)->getHealthData().lower - m_Weapon1.getDamage());

			if (vCharSet.at(iOutput)->getAimingState() != AIM)
			{
				vCharSet.at(iOutput)->setAimingState(SEARCH_SPIN);
			}
		}
		m_Weapon1.setBullet(lowestIntersect);
	}
}

int Character::lazerChecks(std::vector<Character*> vCharSet)
{
	//Finds where the lazer intersects and updates its position
	sf::Vector2f lowestIntersect(m_Weapon1.getIntersect());
	std::vector<sf::Vector2f> lazerRay = { m_Weapon1.getPosition(), m_Weapon1.getIntersect() };

	int iOutput = -1;
	for (int i = 0; i < vCharSet.size(); i++)
	{
		//Checks where the ray and the edge intersect
		sf::Vector2f currentIntersect = Util::lineIntersect(vCharSet.at(i)->getCollisionLine(getRotation()).first, 
															vCharSet.at(i)->getCollisionLine(getRotation()).second, 
															lazerRay.at(0),
															lazerRay.at(1));

		//If the ray is shorter than the previous rays then set the ray to be the shortest ray
		if (Util::magnitude(currentIntersect - m_Weapon1.getPosition()) < Util::magnitude(lowestIntersect - m_Weapon1.getPosition()))
		{
			iOutput = i;
			lowestIntersect = currentIntersect;
		}
	}

	m_Weapon1.setIntersect(lowestIntersect);
	return iOutput;
}

void Character::setPatrolPath(std::vector<int> viPathNodes)
{
	for (int i = 0; i < viPathNodes.size(); i++)
	{
		m_PatrolPath.push_back(new Node);
		m_PatrolPath.at(m_PatrolPath.size() - 1)->index = viPathNodes.at(i);

		if (i > 0)
		{
			m_PatrolPath.at(i)->parent = m_PatrolPath.at(i - 1);
		}

		if (i == viPathNodes.size() - 1)
		{
			if (viPathNodes.at(i) == viPathNodes.at(0))
			{
				m_PatrolPath.at(0)->parent = m_PatrolPath.at(i);
			}
		}
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

void Character::setPath(sf::Vector2f startPos, sf::Vector2f endPos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	m_Path = m_Pathfinder.findPath(startPos, endPos);
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
		m_AimingState = AIM;
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

void Character::setMovementState(States newState)
{
	m_MovementState = newState;
}

void Character::setAimingState(States newState)
{
	m_AimingState = newState;
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

std::pair<sf::Vector2f, sf::Vector2f> Character::getCollisionLine(float fAngle)
{
	//Gets the radius and rotates it to the current orientation 
	sf::Vector2f radiusLine = Util::rotateVect(sf::Vector2f(getSize().x / 2, getSize().x / 2), fAngle - 90);

	//The orientation is then applied to the position to get aan orientation spanning the characters width
	std::vector<sf::Vector2f> vNewCollisionLine = { m_MainSprite.getPosition() - radiusLine , m_MainSprite.getPosition() + radiusLine };
	m_CollisionLine[0].position = vNewCollisionLine[0];
	m_CollisionLine[1].position = vNewCollisionLine[1];

	return std::pair<sf::Vector2f, sf::Vector2f>(vNewCollisionLine[0], vNewCollisionLine[1]);
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

bool Character::hearsSound(soundWave* soundArea)
{
	if (m_AimingState != AIM && m_MovementState != INVESTIGATING)
	{
		if ((Util::magnitude(soundArea->getPosition() - getPosition()) < soundArea->getRadius() + getSize().y))
		{
			m_MovementState = INVESTIGATING;
			focusPoint = soundArea->getPosition();
			m_InvestigationArea = soundArea->getPosition();
			return true;
		}
	}
	return false;
}

std::vector<soundWave*>* Character::getSoundWaves()
{
	return &m_Waves;
}

States Character::getAimingState()
{
	return m_AimingState;
}

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!m_bDead)
	{
		if (m_bDrawVision)
		{
			target.draw(m_VisionRays);
			if(m_CurrentSettings->debugActive()) 
				target.draw(m_VisionLines);
		}

		target.draw(m_Weapon1);
	}

	target.draw(m_MainSprite);

	if (!m_bDead)
	{
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
	else
	{
		target.draw(m_DeathImage);
	}
}

Character::~Character()
{
	for (int i = 0; i < m_Waves.size(); i++)
	{
		if (m_Waves.at(i) != NULL)
		{
			delete m_Waves.at(i);
			m_Waves.at(i) = NULL;
		}
	}
}