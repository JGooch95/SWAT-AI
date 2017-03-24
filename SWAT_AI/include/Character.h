#pragma once
#include <SFML\Graphics.hpp>
#include <deque>
#include <set>
#include "AStar.h"
#include "Object.h"
#include "Bar.h"
#include "Weapon.h"
#include "MathUtils.h"

enum classType {Assault, Sniper, Support, Shotgunner};
enum loadoutItem {Lazer, Silencer, Scope, None};

class Character : public Object
{
	private:
		//UI
		Bar m_HealthBar;
		Bar m_AmmoBar;

		Weapon m_Weapon1;
		classType currentClass;
		std::vector<loadoutItem> loadout;
		Util::Limits m_HealthLevels;

		float m_fMovementAngle;
		float m_fAimingAngle;
		int m_iAimingDirection;

		bool m_bDrawVision;

		//AI
		std::deque<Node*> m_Path; //Holds the current path
		enum States { SEARCH_SWEEP, AIM};
		States m_CurrentState;  

		//Debug lines
		sf::VertexArray m_OrientationLine; //Shows the direction the unit is facing
		sf::VertexArray m_MovementLine; //Shows the direction the unit is facing
		sf::VertexArray m_PathLine; //Shows the path the unit is following
		sf::VertexArray m_CollisionLine; //Shows the line used for shooting collisions
		sf::VertexArray m_VisionRays;

		Character* m_CurrentTarget;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Character(); //Default constructor

		void update(); //Updates the states of data
		void move(); //Follows the path to the next node
		void lookAt(sf::Vector2f position); //Looks towards the position stated.
		void lookAt(float fAngle);
		void visionCalculation(std::vector<sf::Vector2f>vEdges);
		float bulletChecks(std::vector<sf::Vector2f>vEdges);
		bool checkVisionCone(sf::Vector2f position);
		bool lazerChecks(std::vector<sf::Vector2f> vEdges); //Checks if the weapons lazer has collided
		void setupTextures();

		//Setters
		void setHealth(float fLevel);
		void setGunTexture(sf::Texture* tex2);
		void setPath(std::deque<Node*> newPath); //Sets a new path to follow
		void setVision(bool bState);
		void setTarget(Character* newTarget);
		void setMuzzle(sf::Texture* tex2);
		void setClass(classType classType, sf::Texture* gunTexture);
		void setLoadoutItem(int iIndex, loadoutItem itemType);

		//Getters
		float getRotation();
		bool reloading();
		Util::Limits getHealthData();
		Util::Limits getAmmoData();
		std::vector<sf::Vector2f> getCollisionLine(float fAngle); //Gets the line used for shooting collisions
		classType getClass();
		loadoutItem getLoadoutItem(int iIndex);
		int getLoadoutSize();
		loadoutItem getNextLoadoutItem(loadoutItem itemType);
};