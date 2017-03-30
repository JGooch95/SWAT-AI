#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "MathUtils.h"
#include "TextureLoader.h"
#include "SoundManager.h"

enum classType { Assault, Sniper, Support, Shotgunner };
class Weapon : public Object
{
	private:
		//RESOURCES
		SoundManager* m_SoundManager;
		TextureLoader* m_Textures;

		//Sets the sound variables
		sf::Sound m_SilencedShotSound; //Silenced shooting sound
		sf::Sound m_ShotSound; //Unsilenced shooting sound
		sf::Sound m_ReloadSound; //Reload sound

		//Timers
		sf::Clock m_FireRateClock; //Holds how long its been since firing the weapon
		sf::Clock m_ReloadClock; //Holds how long the gun has been reloading

		//Statistics
		Util::Limits m_DamageLimits; //Holds the maximum and minimum damage levels
		Util::Limits m_RangeLimits; //Holds the maximum and minimum range levels
		Util::Limits m_AmmoLevels; //Holds the ammo levels
		bool m_bReloading; //States whether the gun is reloading or not
		bool m_bShooting; //States whether the gun is shooting or not
		float m_fReloadTime; //The amount of time it takes to reload
		float m_fAccuracy; //The accuracy percentage of the bullets when being shot
		float m_fWeaponVolume; //The distance of the sound wave emitted when shooting
		float m_fFireRate; //How long it takes before a bullet can be fired between shots

		Object muzzleFlash;

		//Attatchments
		bool m_bLazer; //States whether a lazer is attatched
		bool m_bSilencer;  //States whether a silencer is attatched
		bool m_bScope;  //States whether a scope is attatched

		Object silencer; //Holds the silencer object
		Object scope; //Holds the scope object

		//Lines
		sf::VertexArray m_BulletRays; //Holds the bullet rays
		sf::VertexArray m_AimLine; //Holds the lazer pointer line

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Weapon(); //Constructor
		void aim(sf::Vector2f location); //Aims towards the location given
		void aim(float fAngle); //Aims towards the angle given
		void update(); //Updates the states of data
		void shoot(); //Shoots a bullet out of the weapon if the gun can shoot
		void reload(); //Refills the gun ammo after the reload period has finished
		float bulletChecks(std::vector<sf::Vector2f>vEdges); //Checks collision for the bullets

		//Setters
		void setFireRate(float fVal);
		void setDamage(sf::Vector2f Val);
		void setRange(sf::Vector2f Val);
		void setIntersect(sf::Vector2f vect); //Sets the lines intersect
		void setAmmoLimit(float fAmmoCount);
		void setAmmo(float fAmmo);
		void setReloadTime(float fTime);
		void setAccuracy(float fLevel);
		void setLazer(bool bValue);
		void setSilencer(bool bValue);
		void setScope(bool bValue);
		void setWeaponVolume(float fValue);
		void setShotSound(int iIndex);
		void setReloadSound(int iIndex);
		void setWeaponType(classType newClassType);

		//Getters
		float getDamage();
		float getFireRate();
		float getWeaponVolume();
		bool reloading();
		Util::Limits getAmmoLevels();
		sf::Vector2f getPosition();
		sf::Vector2f getIntersect(); //Returns where the lazer has intersected
		sf::Vector2f getWeaponEnd();
		bool usingScope();
		bool isShooting();
		bool isSilenced();
};