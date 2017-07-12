#pragma once

#include "MathUtils.h"
#include "Character.h"
#include "SFML\Graphics.hpp"

enum States { IDLE, SEARCH_SWEEP, SEARCH_SPIN, FOCUS, AIM, INVESTIGATING, PATROL, MOVE_TO_SPOT, MOVE_TO_PATROL };
enum AITypes {AddEnemyAI};

class Character;
class AIController: public sf::Drawable
{
	protected:
		Character* CurrentCharacter;
		AStar m_Pathfinder;  //!< The pathfinder used for navigation
		std::deque<Node*> m_Path; //!< Holds the current movement path
		std::deque<Node*> m_PatrolPath; //!< The path used for enemy patrols
		int patrolNode; //!< The last patrol node reached
		int patrolDirection; //!< The direction the patrol is followed in
		Character* m_CurrentTarget; //!< Holds the target the character is aiming towards
		float spinAmount; //!< The amount the character has spinned in the search spin state
		sf::Vector2f focusPoint; //!< The focus point used when in the focus aiming state
		int m_iAimingDirection; //!< Holds the direction the character aims towards when doing a sweep search
		States m_AimingState; //!< The current state of the aiming state machine
		States m_MovementState;  //!< The current state of the movement state machine
		sf::Vector2f m_InvestigationArea; //!< States where the character is investigating
		sf::VertexArray m_PathLine; //!< Shows the path the unit is following
		Map* m_CurrentMap; //!< Holds the data for the map
		Settings* m_CurrentSettings; //!< Holds the current settings
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Node m_PathNode;
		AIController();

		void init(Character* newCharacter);

		/// \brief Updates the states of data
		virtual void update();

		/// \brief Follows the path to the next node
		void move();

		/// \brief Sets the movement state of the AI
		/// \param States The state to switch to
		void setMovementState(States newState);

		/// \brief Sets the aiming state of the AI
		/// \param States The state to switch to
		void setAimingState(States newState);

		/// \brief Sets a target for the character
		/// \param newTarget The new target for the character
		void setTarget(Character* newTarget);

		void setPath(sf::Vector2f startPos, sf::Vector2f endPos);

		/// \brief Sets the patrol path to the nodes given
		/// \param viPathNodes Holds the indexes for the nodes to move to
		void setPatrolPath(std::vector<int> viPathNodes);

		virtual bool hearsSound(WaveEffect* soundArea);
		
		States getMovementState();

	
		States getAimingState();
};