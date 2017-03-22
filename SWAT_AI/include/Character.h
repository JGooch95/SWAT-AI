#pragma once
#include <SFML\Graphics.hpp>
#include <deque>
#include <set>
#include "AStar.h"
#include "Object.h"
#include "Bar.h"
#include "Weapon.h"

enum classType {Assault, Sniper, Support, Shotgunner};
class Character : public Object
{
	private:
		Weapon m_Weapon1;
		Bar m_HealthBar;

		classType currentClass;
		//Weapon data
		Bar m_AmmoBar;
		sf::Clock m_FireRateClock; //Holds how long its been since firing the weapon
		sf::Clock m_ReloadClock;
		bool m_bReloading;
		float m_fMovementAngle;
		float m_fAimingAngle;
		float m_fReloadTime;
		int m_iAimingDirection;
		bool bDrawVision;
		bool bShooting;

		float m_fAccuracy;

		//AI
		std::deque<Node*> m_Path; //Holds the current path
		enum States { SEARCH_SWEEP, AIM};
		States m_CurrentState;  

		Object muzzleFlash;

		//Debug lines
		sf::VertexArray m_OrientationLine; //Shows the direction the unit is facing
		sf::VertexArray m_MovementLine; //Shows the direction the unit is facing
		sf::VertexArray m_PathLine; //Shows the path the unit is following
		sf::VertexArray m_CollisionLine; //Shows the line used for shooting collisions
		sf::VertexArray m_VisionRays;
		sf::VertexArray m_BulletRays;

		Character* m_CurrentTarget;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		std::pair<bool, sf::Vector2f> findLowestIntersect(std::vector<sf::Vector2f>vEdges, std::vector<sf::Vector2f> newRay);

	public:
		Character(); //Default constructor

		void move(); //Follows the path to the next node
		void update(); //Updates the states of data
		float shoot(std::vector<sf::Vector2f>vEdges); //Shoots the characters gun and returns the damage dealt
		bool reloading();

		//Getters
		float getRotation(); 
		sf::Vector2f getHealthData();
		sf::Vector2f getAmmoData();
		std::vector<sf::Vector2f> getCollisionLine(float fAngle); //Gets the line used for shooting collisions

		//Setters
		void setHealth(float fLevel); 
		void setGunTexture(sf::Texture* tex2);
		void setPath(std::deque<Node*> newPath); //Sets a new path to follow
		void reload();
		void setVision(bool bState);

		void setTarget(Character* newTarget);

		void setMuzzle(sf::Texture* tex2);
		void setClass(classType classType, sf::Texture* GunTexture);
		classType getClass();
		void shoot();
		float bulletChecks(std::vector<sf::Vector2f>vEdges);
		bool checkVisionCone(sf::Vector2f position);

		//MAYBE CHANGE =============================================================
		void lookAt(sf::Vector2f position); //Looks towards the position stated.
		void lookAt(float fAngle);
		bool lazerChecks(std::vector<sf::Vector2f> vEdges); //Checks if the weapons lazer has collided
		void visionCalculation(std::vector<sf::Vector2f>vEdges);
		// =========================================================================
};