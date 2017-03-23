#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "MathUtils.h"

class Weapon : public Object
{
	private:
		sf::VertexArray m_AimLine; //Holds the lazer pointer line
		float m_fFireRate; //How many shots can be fired per second
		Util::Limits m_DamageLimits; //Holds the maximum and minimum damage levels
		Util::Limits m_RangeLimits; //Holds the maximum and minimum range levels
		Util::Limits m_AmmoLevels; //Holds the ammo levels

		sf::Clock m_FireRateClock; //Holds how long its been since firing the weapon
		sf::Clock m_ReloadClock;
		bool m_bReloading;
		float m_fReloadTime;
		float m_fAccuracy;
		bool m_bShooting;
		Object muzzleFlash;

		sf::VertexArray m_BulletRays;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Weapon(); //Constructor
		void aim(sf::Vector2f location); //Aims towards the location given
		void aim(float fAngle);
		void update(); //Updates the states of data
		void shoot();
		void reload();
		float bulletChecks(std::vector<sf::Vector2f>vEdges);

		//Setters
		void setFireRate(float fVal);
		void setDamage(sf::Vector2f Val);
		void setRange(sf::Vector2f Val);
		void setOrigin(sf::Vector2f Val);
		void setIntersect(sf::Vector2f vect); //Sets the lines intersect
		void setAmmoLimit(float fAmmoCount);
		void setAmmo(float fAmmo);
		void setReloadTime(float fTime);
		void setAccuracy(float fLevel);
		void setMuzzle(sf::Texture* tex2);

		//Getters
		float getDamage();
		float getFireRate();
		bool reloading();
		Util::Limits getAmmoLevels();
		sf::Vector2f getPosition();
		sf::Vector2f getIntersect(); //Returns where the lazer has intersected

};