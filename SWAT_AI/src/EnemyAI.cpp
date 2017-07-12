#include "../include/EnemyAI.h"

EnemyAI::EnemyAI()
{
	fLastHealth = 0.0f;
}

void EnemyAI::update()
{
	//If damaged search around character
	if (CurrentCharacter->getHealthData().lower < fLastHealth && m_AimingState != AIM)
	{
		setAimingState(SEARCH_SPIN);
		setMovementState(IDLE);
	}
	fLastHealth = CurrentCharacter->getHealthData().lower;

	//Moves the player 
	switch (m_AimingState)
	{
		case SEARCH_SWEEP:
		{
			float fCone = 30.0f; //Holds the breadth of the cone to turn between

								 //Sweeps between the cone extents
			if (CurrentCharacter->m_fAimingAngle > fCone)
			{
				CurrentCharacter->m_fAimingAngle = fCone;
				m_iAimingDirection *= -1;
			}
			if (CurrentCharacter->m_fAimingAngle < -fCone)
			{
				CurrentCharacter->m_fAimingAngle = -fCone;
				m_iAimingDirection *= -1;
			}

			//Aims at the new amgle
			CurrentCharacter->m_fAimingAngle += m_iAimingDirection;
			CurrentCharacter->lookAt(CurrentCharacter->m_fMovementAngle + CurrentCharacter->m_fAimingAngle);
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

				CurrentCharacter->m_fAimingAngle = CurrentCharacter->m_fAimingAngle + 1;
				CurrentCharacter->lookAt(CurrentCharacter->m_fMovementAngle + CurrentCharacter->m_fAimingAngle);
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
				CurrentCharacter->lookAt(m_CurrentTarget->getPosition()); //Aim at the target
				lastPos = m_CurrentTarget->getPosition();

				if (m_CurrentTarget->isDead()) //If the target is dead
				{
					m_CurrentTarget = NULL; //Clear the target
					m_MovementState = IDLE;
				}
				else
				{
					CurrentCharacter->shoot();
				}
			}
			else
			{
				m_MovementState = INVESTIGATING;
				focusPoint = lastPos;
				m_InvestigationArea = lastPos;
				setPath(CurrentCharacter->getPosition(), m_InvestigationArea);
				move();

				m_AimingState = FOCUS; //Switch states
			}
			break;
		}
		case FOCUS:
		{
			CurrentCharacter->lookAt(focusPoint);
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
			if (m_AimingState != SEARCH_SPIN )
			{
				if (m_Path.size() > 0)
				{
					m_MovementState = MOVE_TO_SPOT;
					m_PathNode = *m_Path.at(0);
				}
				else if (m_PatrolPath.size() > 0)
				{
					if (m_AimingState != AIM)
					{
						sf::Vector2f destination = sf::Vector2f((((m_PatrolPath.at(patrolNode)->index % (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2)),
							(((m_PatrolPath.at(patrolNode)->index / (int)m_CurrentMap->getGridDims().x) * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
						destination += m_CurrentMap->getPosition();
						setPath(CurrentCharacter->getPosition(), destination);

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
			if (m_PatrolPath.size() <= 0 || m_Path.size() > 0)
			{
				m_MovementState = IDLE;
			}
			break;
		}

		case MOVE_TO_SPOT:
			if (m_Path.empty())
			{
				m_MovementState = IDLE;
				m_AimingState = SEARCH_SWEEP;
			}
			break;

		case MOVE_TO_PATROL:
			if (m_Path.empty())
			{
				m_MovementState = PATROL;
				m_PathNode = *m_PatrolPath.at(patrolNode);
			}
			break;

		case INVESTIGATING:
			if (m_AimingState != AIM)
			{
				m_AimingState = FOCUS;
				if (Util::magnitude(CurrentCharacter->getPosition() - m_InvestigationArea) < CurrentCharacter->getSize().y)
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
	if (m_AimingState != AIM && m_AimingState != SEARCH_SPIN && m_MovementState != INVESTIGATING)
	{
		if ((Util::magnitude(soundArea->getPosition() - CurrentCharacter->getPosition()) < soundArea->getRadius() + CurrentCharacter->getSize().y))
		{
			m_MovementState = INVESTIGATING;
			focusPoint = soundArea->getPosition();
			m_InvestigationArea = soundArea->getPosition();
			setPath(CurrentCharacter->getPosition(), m_InvestigationArea);
			move();
			return true;
		}
	}
	return false;
}