#pragma once
#include <SFML\Graphics.hpp>
#include <deque>
#include <set>
#include "AStar.h"
#include "Object.h"
#include "Bar.h"
#include "Weapon.h"
#include "MathUtils.h"
#include "TextureLoader.h"
#include "SoundWave.h"

enum loadoutItem {Lazer, Silencer, Scope, None};
enum States { IDLE, SEARCH_SWEEP, SEARCH_SPIN, FOCUS, AIM, INVESTIGATING, PATROL, MOVE_TO_SPOT };

/// \brief An AI character which performs the main actions within the game
class Character : public Object
{
	private:
		//UI
		Bar m_HealthBar; //!< The health bar
		Bar m_AmmoBar; //!< The ammo bar
		Object m_DeathImage; //!< The image used when the character dies

		//Resources
		TextureLoader* m_Textures;  //!< Holds the location of all of the textures
		SoundManager* m_SoundManager;  //!< Holds the location of all of the sounds
		 
		//Sounds
		std::vector<soundWave*> m_Waves; //!< Holds soundwaves which are emitted
		sf::Sound m_StepSound; //!< The sound played every step
		sf::Sound m_LoadoutSound; //!< The loadout switching sound
		sf::Sound m_DeathSound; //!< The dying sound

		//Equipment
		Weapon m_Weapon1; //!< The charcter's weapon
		std::vector<loadoutItem> m_vLoadout; //!< The equipment the character has equipped

		//AI
		States m_AimingState; //!< The current state of the aiming state machine
		States m_MovementState;  //!< The current state of the movement state machine
		AStar m_Pathfinder;  //!< The pathfinder used for navigation
		sf::Vector2f focusPoint; //!< The focus point used when in the focus aiming state
		int m_iAimingDirection; //!< Holds the direction the character aims towards when doing a sweep search
		std::deque<Node*> m_Path; //!< Holds the current movement path
		std::deque<Node*> m_PatrolPath; //!< The path used for enemy patrols
		Character* m_CurrentTarget; //!< Holds the target the character is aiming towards
		int patrolNode; //!< The last patrol node reached
		int patrolDirection; //!< The direction the patrol is followed in
		float spinAmount; //!< The amount the character has spinned in the search spin state

		//Debug lines
		sf::VertexArray m_OrientationLine; //!< Shows the direction the unit is facing
		sf::VertexArray m_MovementLine; //!< Shows the direction the unit is facing
		sf::VertexArray m_PathLine; //!< Shows the path the unit is following
		sf::VertexArray m_CollisionLine; //!< Shows the line used for shooting collisions
		sf::VertexArray m_VisionRays; //!< Holds the character's vision cone
		sf::VertexArray m_VisionLines; //!< Holds the character's vision cone rays

		//Other data
		classType m_CurrentClass; //!< Holds the class of the current unit
		Util::Limits m_HealthLevels; //!< Holds the characters health data
		float m_fMovementAngle; //!< Holds the angle at which the character is heading towards
		float m_fAimingAngle; //!< Holds the angle the character is aiming towards
		float fDistanceSinceStep; //!< Holds how many pixels the unit has moved in a step
		bool m_bDrawVision; //!< States whether vision cones are drawn
		bool m_bDead; //!< States whether the character is dead

		sf::Vector2f m_InvestigationArea; //!< States where the character is investigating

		/// \brief Draws all of the characters's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
	    ///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		Character();

		/// \brief Updates the states of data
		void update();

		/// \brief Follows the path to the next node
		void move();

		/// \brief Looks towards the position stated.
		/// \param position The position to look towards
		void lookAt(sf::Vector2f position);

		/// \brief Looks towards the angle given
		/// \param fAngle The angle to look towards
		void lookAt(float fAngle); 

		/// \brief Performs ray collisions with the edges given to produce a cone of vision
		/// \param vEdges The vector of edges being checked
		void visionCalculation();

		/// \brief Checks if the weapons lazer has collided
		/// \param vEdges The vector of edges being checked
		/// \return Retuns if the lazer hit anything
		int rayChecks(std::vector<Character*>  vCharSet, int iType);

		/// \brief Checks if the lazer has collided with any walls
		void lazerEdgeChecks();

		/// \brief Checks if the bullet has collided with any walls
		void bulletEdgeChecks();

		/// \brief Sets the patrol path to the nodes given
		/// \param viPathNodes Holds the indexes for the nodes to move to
		void setPatrolPath(std::vector<int> viPathNodes);

		//Setters
		/// \brief Sets the health of the character
		/// \param fLevel The amount of health
		void setHealth(float fLevel);

		/// \brief Sets the path for the character to follow
		/// \param startPos The staring position of the path
		/// \param endPos The end position of the path
		void setPath(sf::Vector2f startPos, sf::Vector2f endPos); //Sets a new path to follow

		/// \brief Sets the whether the vision is active
		/// \param bState The new state of the vision
		void setVision(bool bState);

		/// \brief Sets a target for the character
		/// \param newTarget The new target for the character
		void setTarget(Character* newTarget);

		/// \brief Sets a new class for the character
		/// \param classType The new class for the character
		void setClass(classType classType);

		/// \brief Sets the loadout item of one item slot
		/// \param iIndex The index of the loadout slot being changed
		/// \param itemType The item that is being switched to
		void setLoadoutItem(int iIndex, loadoutItem itemType);

		/// \brief Sets the movement state of the AI
		/// \param States The state to switch to
		void setMovementState(States newState);

		/// \brief Sets the aiming state of the AI
		/// \param States The state to switch to
		void setAimingState(States newState);

		//Getters
		/// \brief Gets the rotation of the character
		/// \return returns the rotation of the character
		float getRotation();

		/// \brief Gets the amount of loadout slots available
		/// \return returns the amount of loadout slots available
		int getLoadoutSize();

		/// \brief Gets the health levels
		/// \return returns the health levels
		Util::Limits getHealthData();

		/// \brief Gets the ammo levels
		/// \return returns the ammo levels
		Util::Limits getAmmoData();

		/// \brief Gets the collision line for shooting collisions
		/// \param fAngle The angle of the collision line
		/// \return returns the collision line
		std::pair<sf::Vector2f, sf::Vector2f> getCollisionLine(float fAngle);

		/// \brief Gets the characters class type
		/// \return returns the characters class type
		classType getClass();

		/// \brief Gets the loadout at the given index
		/// \param iIndex the index of the loadout slot
		/// \return returns the loadout at the given index
		loadoutItem getLoadoutItem(int iIndex);

		/// \brief Gets the next loadout item in the cycle
		/// \param itemType The current itemType
		/// \return returns the new loadout item
		loadoutItem getNextLoadoutItem(loadoutItem itemType);

		/// \brief Gets the characters weapon
		/// \return returns the characters weapon
		Weapon* getWeapon();

		/// \brief States if the character is reloading
		/// \return returns if the character is reloading
		bool reloading();

		/// \brief States if the character is shooting
		/// \return returns if the character is shooting
		bool isShooting();

		/// \brief States if the character has taken a step
		/// \return returns if the character has taken a step
		bool stepTaken();

		/// \brief States if the character is dead
		/// \return returns if the character is dead
		bool isDead();

		/// \brief States whether the sound given has been heard
		/// \param soundArea The sound being analysed
		/// \return returns whether the sound given has been heard
		bool hearsSound(soundWave* soundArea);

		/// \brief Gets the characters emitted sound waves
		/// \return returns the characters emitted sound waves
		std::vector<soundWave*>* getSoundWaves();

		/// \brief Gets the state of the aiming state machine
		/// \return returns the state of the aiming state machine
		States getAimingState();

		/// \brief Default deconstructor
		~Character();
};