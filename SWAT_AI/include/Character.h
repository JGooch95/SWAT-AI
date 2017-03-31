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
enum States { IDLE, SEARCH_SWEEP, FOCUS, AIM, INVESTIGATING, PATROL, STARTING_PATROL, MOVE_TO_SPOT };

class Character : public Object
{
	private:
		//UI
		Bar m_HealthBar;
		Bar m_AmmoBar;
		Object m_DeathImage;

		//Resources
		TextureLoader* m_Textures;
		SoundManager* m_SoundManager;

		//Sounds
		std::vector<soundWave*> m_Waves; //Holds soundwaves which are emitted
		sf::Sound m_StepSound; //The sound played every step
		sf::Sound m_LoadoutSound; //The loadout switching sound
		sf::Sound m_DeathSound; //The dying sound

		//Equipment
		Weapon m_Weapon1;
		std::vector<loadoutItem> m_vLoadout;

		//AI
		States m_AimingState;
		States m_MovementState;
		AStar m_Pathfinder;
		sf::Vector2f focusPoint;
		int m_iAimingDirection; //Holds the direction the character aims towards when doing a sweep search
		std::deque<Node*> m_Path; //Holds the current movement path
		std::deque<Node*> m_PatrolPath;
		Character* m_CurrentTarget; //Holds the target the character is aiming towards
		int patrolNode;
		int patrolDirection;

		//Debug lines
		sf::VertexArray m_OrientationLine; //Shows the direction the unit is facing
		sf::VertexArray m_MovementLine; //Shows the direction the unit is facing
		sf::VertexArray m_PathLine; //Shows the path the unit is following
		sf::VertexArray m_CollisionLine; //Shows the line used for shooting collisions
		sf::VertexArray m_VisionRays;

		//Other data
		classType m_CurrentClass; //Holds the class of the current unit
		Util::Limits m_HealthLevels; //Holds the characters health data
		float m_fMovementAngle; //Holds the angle at which the character is heading towards
		float m_fAimingAngle; //Holds the angle the character is aiming towards
		float fDistanceSinceStep; //Holds how many pixels the unit has moved in a step
		bool m_bDrawVision; //States whether vision cones are drawn
		bool m_bDead; //States whether the character is dead

		sf::Vector2f m_InvestigationArea; //States where the character is investigating

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Character(); //Default constructor

		void update(); //Updates the states of data
		void move(); //Follows the path to the next node
		void lookAt(sf::Vector2f position); //Looks towards the position stated.
		void lookAt(float fAngle); //Looks towards the angle given
		void visionCalculation(std::vector<sf::Vector2f>vEdges); //Calculates the vision cone
		float bulletChecks(std::vector<sf::Vector2f>vEdges); //Sends the edges to the gun to calculate bullet collisions
		bool lazerChecks(std::vector<sf::Vector2f> vEdges); //Checks if the weapons lazer has collided
		void setPatrolPath(std::vector<int> viPathNodes);

		//Setters
		void setHealth(float fLevel);
		void setPath(sf::Vector2f startPos, sf::Vector2f endPos); //Sets a new path to follow
		void setVision(bool bState);
		void setTarget(Character* newTarget);
		void setClass(classType classType);
		void setLoadoutItem(int iIndex, loadoutItem itemType);
		void setMovementState(States newState);

		//Getters
		float getRotation();
		int getLoadoutSize();
		Util::Limits getHealthData();
		Util::Limits getAmmoData();
		std::vector<sf::Vector2f> getCollisionLine(float fAngle); //Gets the line used for shooting collisions
		classType getClass();
		loadoutItem getLoadoutItem(int iIndex);
		loadoutItem getNextLoadoutItem(loadoutItem itemType);
		Weapon* getWeapon();
		bool reloading();
		bool isShooting();
		bool stepTaken();
		bool isDead();
		bool hearsSound(soundWave* soundArea);
		std::vector<soundWave*>* getSoundWaves();

		~Character();
};