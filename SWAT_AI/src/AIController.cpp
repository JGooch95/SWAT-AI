#include "../include/AIController.h"

void AIController::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_PathLine);
}

AIController::AIController()
{
	m_CurrentMap = Map::getInstance();
	m_CurrentSettings = Settings::getInstance();
	m_MovementState = IDLE;
	m_AimingState = SEARCH_SWEEP;
	m_iPatrolNode = 0;
	m_iPatrolDirection = 1;
	m_iAimingDirection = 1;
	m_CurrentTarget = NULL;
	m_fSpinAmount = 0;
	m_Pathfinder.setup();

	m_PathLine.setPrimitiveType(sf::LinesStrip);
}

void AIController::init(Character* newCharacter)
{
	m_CurrentCharacter = newCharacter;
}

void AIController::update()
{
	//Moves the player 
	switch (m_AimingState)
	{
		case SEARCH_SWEEP:
		{
			float fCone = 30.0f; //Holds the breadth of the cone to turn between

								 //Sweeps between the cone extents
			if (m_CurrentCharacter->m_fAimingAngle > fCone)
			{
				m_CurrentCharacter->m_fAimingAngle = fCone;
				m_iAimingDirection *= -1;
			}
			if (m_CurrentCharacter->m_fAimingAngle < -fCone)
			{
				m_CurrentCharacter->m_fAimingAngle = -fCone;
				m_iAimingDirection *= -1;
			}

			//Aims at the new amgle
			m_CurrentCharacter->m_fAimingAngle += m_iAimingDirection;
			m_CurrentCharacter->lookAt(m_CurrentCharacter->m_fMovementAngle + m_CurrentCharacter->m_fAimingAngle);
			break;
		}
		case SEARCH_SPIN:
		{
			if (m_CurrentTarget == NULL)
			{
				//Spins 360 degrees from current angle
				m_fSpinAmount++;
				if (m_fSpinAmount >= 360)
				{
					m_fSpinAmount = 0;
					m_AimingState = SEARCH_SWEEP;
				}

				m_CurrentCharacter->m_fAimingAngle = m_fSpinAmount;
				m_CurrentCharacter->lookAt(m_CurrentCharacter->m_fMovementAngle + m_CurrentCharacter->m_fAimingAngle);
			}
			else
			{
				//If there is a target aim at them
				m_fSpinAmount = 0;
				m_AimingState = AIM;
			}
			break;
		}
		case AIM:
		{
			//If there is a target
			if (m_CurrentTarget != NULL)
			{
				m_CurrentCharacter->lookAt(m_CurrentTarget->getPosition()); //Aim at the target

				if (m_CurrentTarget->isDead()) //If the target is dead
				{
					m_CurrentTarget = NULL; //Clear the target
					m_MovementState = IDLE;
				}
				else
				{
					m_CurrentCharacter->shoot();
					m_MovementState = IDLE;
				}
			}
			else
			{
				m_AimingState = SEARCH_SWEEP; //Switch states
			}
			break;
		}
	}

	switch (m_MovementState)
	{
		case IDLE:
			//Move to any available paths
			if (m_Path.size() > 0)
			{
				m_MovementState = MOVE_TO_SPOT;
				m_PathNode = *m_Path.at(0);
			}
			break;
	}
}

void AIController::setPath(sf::Vector2f startPos, sf::Vector2f endPos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	m_Path = m_Pathfinder.findPath(startPos, endPos);
}

void AIController::move()
{
	if (m_MovementState == PATROL)
	{
		if (m_PatrolPath.size() > 0)
		{
			m_PathNode = *m_PatrolPath.at(m_iPatrolNode);

			if (m_PatrolPath.at(m_PatrolPath.size() - 1)->parent == NULL)
			{
				if (m_iPatrolNode >= m_PatrolPath.size() - 1)
				{
					m_iPatrolNode = m_PatrolPath.size() - 1;
					m_iPatrolDirection = -1;
				}
				else if (m_iPatrolNode <= 0)
				{
					m_iPatrolNode = 0;
					m_iPatrolDirection = 1;
				}
			}
			else if (m_iPatrolNode == m_PatrolPath.size() - 1)
			{
				m_iPatrolNode = 0;
			}

			m_iPatrolNode += m_iPatrolDirection;

			//Sets up the path line ready to start drawing a new path
			m_PathLine.clear();
			if (m_CurrentSettings->debugActive())
			{
				m_PathLine.resize(m_PatrolPath.size());
				for (int i = 0; i < m_PatrolPath.size(); ++i)
				{
					m_PathLine[i] = sf::Vertex(m_CurrentMap->getPosition() + sf::Vector2f(
						(((m_PatrolPath.at(i)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
						(((m_PatrolPath.at(i)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2))),
						sf::Color(0, 255, 0, 255));
				}
			}
		}
	}
	else
	{
		if (m_Path.size() > 0)
		{
			m_Path.pop_front();
			if (m_Path.size() > 0)
			{
				m_PathNode = *m_Path.at(0);
			}
		}

		//Sets up the path line ready to start drawing a new path
		m_PathLine.clear();
		if (m_CurrentSettings->debugActive())
		{
			m_PathLine.resize(m_Path.size());
			for (int i = 0; i < m_Path.size(); ++i)
			{
				m_PathLine[i] = sf::Vertex(m_CurrentMap->getPosition() + sf::Vector2f(
					(((m_Path.at(i)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
					(((m_Path.at(i)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2))),
					sf::Color(0, 255, 0, 255));
			}
		}
	}
}

void AIController::setMovementState(States newState)
{
	m_MovementState = newState;
}

void AIController::setAimingState(States newState)
{
	m_AimingState = newState;
}

void AIController::setTarget(Character * newTarget)
{
	m_CurrentTarget = newTarget;
	if (m_CurrentTarget != NULL) //If the target isnt empty then aim at the target
	{
		m_AimingState = AIM;
	}
}

void AIController::setPatrolPath(std::vector<int> viPathNodes)
{
	//Reads the patrol path loaded and creates the patrol path for use
	for (int i = 0; i < viPathNodes.size(); ++i)
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
				m_PatrolPath.at(i)->parent = m_PatrolPath.at(0);
			}
			else
			{
				m_PatrolPath.at(i)->parent = NULL;
			}
		}
	}
}

bool AIController::hearsSound(WaveEffect* soundArea)
{
	return false;
}

/// \brief Sets the movement state of the AI
/// \param States The state to switch to
States AIController::getMovementState()
{
	return m_MovementState;
}

/// \brief Sets the aiming state of the AI
/// \param States The state to switch to
States AIController::getAimingState()
{
	return m_AimingState;
}
