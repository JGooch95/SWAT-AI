#include "../include/EnemyAI.h"

EnemyAI::EnemyAI()
{
	m_fLastHealth = 0.0f;
}

void EnemyAI::update()
{
	//If damaged search around character
	if (m_CurrentCharacter->getHealthData().lower < m_fLastHealth && m_AimingState != AIM)
	{
		setAimingState(SEARCH_SPIN);
		setMovementState(IDLE);
	}
	m_fLastHealth = m_CurrentCharacter->getHealthData().lower; //Update health for next check

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

				m_CurrentCharacter->m_fAimingAngle = m_CurrentCharacter->m_fAimingAngle + 1;
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
				m_LastPos = m_CurrentTarget->getPosition();

				if (m_CurrentTarget->isDead()) //If the target is dead
				{
					m_CurrentTarget = NULL; //Clear the target
					m_MovementState = IDLE;
				}
				else
				{
					m_CurrentCharacter->shoot();
				}
			}
			else
			{
				//If the enemy goes out of view search their last seen location
				m_MovementState = INVESTIGATING;
				focusPoint = m_LastPos;
				m_InvestigationArea = m_LastPos;
				setPath(m_CurrentCharacter->getPosition(), m_InvestigationArea);
				move();
				m_AimingState = FOCUS; //Switch states
			}
			break;
		}
		case FOCUS:
		{
			//Focus on a specific location
			m_CurrentCharacter->lookAt(focusPoint);
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
			if (m_AimingState != SEARCH_SPIN ) //Dont move when spinning
			{
				//If there is a path follow it
				if (m_Path.size() > 0)
				{
					m_MovementState = MOVE_TO_SPOT;
					m_PathNode = *m_Path.at(0);
				}
				//If there is a patrol path
				else if (m_PatrolPath.size() > 0)
				{
					if (m_AimingState != AIM)
					{
						//Create a path to the patrol nodes
						sf::Vector2f destination = sf::Vector2f((((m_PatrolPath.at(m_iPatrolNode)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
							(((m_PatrolPath.at(m_iPatrolNode)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
						destination += m_CurrentMap->getPosition();
						setPath(m_CurrentCharacter->getPosition(), destination);

						m_MovementState = MOVE_TO_PATROL;
						if (m_Path.size() > 0)
						{
							m_PathNode = *m_Path.at(0);
						}
					}
				}
			}
			break;

		case PATROL:
		{
			//If the patrol is empty or there is a path go back to idle
			if (m_PatrolPath.size() <= 0 || m_Path.size() > 0)
			{
				m_MovementState = IDLE;
			}
			break;
		}

		case MOVE_TO_SPOT:
			//If the  path is empty go back to idle
			if (m_Path.empty())
			{
				m_MovementState = IDLE;
				m_AimingState = SEARCH_SWEEP;
			}
			break;

		case MOVE_TO_PATROL:
			//If the  path is empty start patrolling
			if (m_Path.empty())
			{
				m_MovementState = PATROL;
				m_PathNode = *m_PatrolPath.at(m_iPatrolNode);
			}
			break;

		case INVESTIGATING:
			//Head to the location
			if (m_AimingState != AIM)
			{
				m_AimingState = FOCUS;

				//When the location is reached search the area
				if (Util::magnitude(m_CurrentCharacter->getPosition() - m_InvestigationArea) < m_CurrentCharacter->getSize().y)
				{
					setAimingState(SEARCH_SPIN);
					setMovementState(IDLE);
				}
			}
			break;
	}
}

bool EnemyAI::hearsSound(WaveEffect* soundArea)
{
	//If a sound is heard then head to the location and investigate
	if (m_AimingState != AIM && m_AimingState != SEARCH_SPIN && m_MovementState != INVESTIGATING)
	{
		if ((Util::magnitude(soundArea->getPosition() - m_CurrentCharacter->getPosition()) < soundArea->getRadius() + m_CurrentCharacter->getSize().y))
		{
			m_MovementState = INVESTIGATING;
			focusPoint = soundArea->getPosition();
			m_InvestigationArea = soundArea->getPosition();
			setPath(m_CurrentCharacter->getPosition(), m_InvestigationArea);
			move();
			return true;
		}
	}
	return false;
}