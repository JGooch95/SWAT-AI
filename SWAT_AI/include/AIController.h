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
		Character* m_CurrentCharacter; //!< Holds a pointer to the character bein manipulated
		AStar m_Pathfinder;  //!< The pathfinder used for navigation

		std::deque<Node*> m_Path; //!< Holds the current movement path

		//Movement
		std::deque<Node*> m_PatrolPath; //!< The path used for enemy patrols
		int m_iPatrolNode; //!< The last patrol node reached
		int m_iPatrolDirection; //!< The direction the patrol is followed in
		States m_MovementState;  //!< The current state of the movement state machine
		sf::Vector2f m_InvestigationArea; //!< States where the character is investigating
		sf::VertexArray m_PathLine; //!< Shows the path the unit is following

		//Aiming
		Character* m_CurrentTarget; //!< Holds the target the character is aiming towards
		float m_fSpinAmount; //!< The amount the character has spinned in the search spin state
		sf::Vector2f focusPoint; //!< The focus point used when in the focus aiming state
		int m_iAimingDirection; //!< Holds the direction the character aims towards when doing a sweep search
		States m_AimingState; //!< The current state of the aiming state machine

		Map* m_CurrentMap; //!< Holds the data for the map
		Settings* m_CurrentSettings; //!< Holds the current settings

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Node m_PathNode; //!< Holds the next node for  the character to head to

		/// \brief Default constructor
		AIController(); 

		/// \brief connects the Character to the AI Controller and sets it up
		/// \param newCharacter The character having AI added to it
		void init(Character* newCharacter);

		/// \brief Updates the states of data
		virtual void update();

		/// \brief Follows the path to the next node
		void move();

		//Setters
		/// \brief Sets the movement state of the AI
		/// \param States The state to switch to
		void setMovementState(States newState);

		/// \brief Sets the aiming state of the AI
		/// \param States The state to switch to
		void setAimingState(States newState);

		/// \brief Sets a target for the character
		/// \param newTarget The new target for the character
		void setTarget(Character* newTarget);

		/// \brief Sets a path for the AI to follow
		/// \param startPos The start of the path
		/// \param endPos The end of the path
		void setPath(sf::Vector2f startPos, sf::Vector2f endPos);

		/// \brief Sets the patrol path to the nodes given
		/// \param viPathNodes Holds the indexes for the nodes to move to
		void setPatrolPath(std::vector<int> viPathNodes);

		///\brief checks if the AI character hears the sound and acts accordingly
		///\param soundArea The sound being checked
		virtual bool hearsSound(WaveEffect* soundArea);
		
		//Getters
		States getMovementState();
		States getAimingState();
};