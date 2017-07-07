#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "MathUtils.h"
#include "TextureLoader.h"
#include "SoundManager.h"

enum classType { Assault, Sniper, Support, Shotgunner };

/// \brief A weapon used by characters to attack other characters.
class Weapon : public Object
{
	private:
		//RESOURCES
		SoundManager* m_SoundManager; //!< Holds the location of all of the sounds
		TextureLoader* m_Textures;    //!< Holds the location of all of the textures

		//Sets the sound variables
		sf::Sound m_SilencedShotSound; //!< Silenced shooting sound
		sf::Sound m_ShotSound; //!< Unsilenced shooting sound
		sf::Sound m_ReloadSound; //!< Reload sound

		//Timers
		sf::Clock m_FireRateClock; //!< Holds how long its been since firing the weapon
		sf::Clock m_ReloadClock; //!< Holds how long the gun has been reloading

		//Statistics
		Util::Limits m_DamageLimits; //!< Holds the maximum and minimum damage levels
		Util::Limits m_RangeLimits; //!< Holds the maximum and minimum range levels
		Util::Limits m_AmmoLevels; //!< Holds the ammo levels
		bool m_bReloading; //!< States whether the gun is reloading or not
		bool m_bShooting; //!< States whether the gun is shooting or not
		float m_fReloadTime; //!< The amount of time it takes to reload
		float m_fAccuracy; //!< The accuracy percentage of the bullets when being shot
		float m_fWeaponVolume; //!< The distance of the sound wave emitted when shooting
		float m_fFireRate; //!< How long it takes before a bullet can be fired between shots

		Object muzzleFlash; //!< Holds the image for the muzzle flash of the weapon

		//Attatchments
		bool m_bLazer; //!< States whether a lazer is attatched
		bool m_bSilencer;  //!< States whether a silencer is attatched
		bool m_bScope;  //!< States whether a scope is attatched

		Object silencer; //!< Holds the silencer object
		Object scope; //!< Holds the scope object

		//Lines
		sf::VertexArray m_BulletRays; //!< Holds the bullet rays
		sf::VertexArray m_AimLine; //!< Holds the lazer pointer line

		/// \brief Draws all of the Weapon's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		/// \param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		Weapon();

		///\brief Aims towards a given location
		///\param location The location to point towards
		void aim(sf::Vector2f location);

		///\brief aims towards a given angle
		///\param fAngle The angle to aim towards 
		void aim(float fAngle);

		///\brief Updates the states of the data
		void update();

		///\brief Shoots a bullet out of the weapon if the gun can shoot
		void shoot();

		///\brief Refills the gun ammo after the reload period has finished
		void reload();

		//Setters
		/// \brief Sets the fire rate of the weapon
		/// \param fVal The time difference per shot
		void setFireRate(float fVal);

		/// \brief Sets the base Damage of the weapon
		/// \param Val The new damage of the weapon
		void setDamage(sf::Vector2f Val);

		/// \brief Sets the range of the weapon
		/// \param Val The new range of the weapon
		void setRange(sf::Vector2f Val);

		/// \brief Sets the intersect of the weapon lazer
		/// \param vect the new intersect of the weapon lazer
		void setIntersect(sf::Vector2f vect); 

		/// \brief Sets the ammo limit of the weapon
		/// \param fAmmoCount The new ammo limit of the weapon
		void setAmmoLimit(float fAmmoCount);

		/// \brief Sets the ammo count of the weapon
		/// \param fAmmo The new ammo count of the weapon
		void setAmmo(float fAmmo);

		/// \brief Sets the reload time of the weapon
		/// \param fTime The new reload time of the weapon
		void setReloadTime(float fTime);

		/// \brief Sets the accuracy of the weapon
		/// \param fLevel The new accuracy time of the weapon
		void setAccuracy(float fLevel);

		/// \brief Sets if the lazer is enabled
		/// \param bValue The state of the lazer
		void setLazer(bool bValue);

		/// \brief Sets if the silencer is enabled
		/// \param bValue The state of the silencer
		void setSilencer(bool bValue);

		/// \brief Sets if the scope is enabled
		/// \param bValue The state of the scope
		void setScope(bool bValue);

		/// \brief Sets the sound radius of the weapon
		/// \param fValue The new sound radius
		void setWeaponVolume(float fValue);

		/// \brief Sets the shot sound  of the weapon
		/// \param iIndex The index of the sound within the weapon
		void setShotSound(int iIndex);

		/// \brief Sets the reload sound  of the weapon
		/// \param iIndex The index of the sound within the weapon
		void setReloadSound(int iIndex);

		/// \brief Sets the weapon type of the weapon
		/// \param newClassType The type of weapon being switched to.
		void setWeaponType(classType newClassType);


		void setBullet(sf::Vector2f point);

		//Getters
		/// \brief Gets the damage of the weapon
		/// \return returns the damage of the weapon
		float getDamage();

		/// \brief Gets the fire rate of the weapon
		/// \return returns the rate of the weapon
		float getFireRate();

		/// \brief Gets the sound radius of the weapon
		/// \return returns the sound radius of the weapon
		float getWeaponVolume();

		/// \brief Gets whether the weapon is reloading
		/// \return returns whether the weapon is reloading
		bool reloading();

		/// \brief Gets the ammo levels of the weapon
		/// \return returns the ammo levels of the weapon
		Util::Limits getAmmoLevels();

		/// \brief Gets the position of the weapon
		/// \return returns the position  of the weapon
		sf::Vector2f getPosition();

		/// \brief Gets the intersection point of the weapons laser
		/// \return returns the intersection point of the weapons laser
		sf::Vector2f getIntersect(); 

		/// \brief Gets the end of the weapons barrel
		/// \return returns the end of the weapons barrel
		sf::Vector2f getWeaponEnd();

		/// \brief Gets whether the weapon is using a scope
		/// \return returns whether the weapon is using a scope
		bool usingScope();

		/// \brief Gets whether the weapon is shooting
		/// \return returns whether the weapon is shooting
		bool isShooting();

		/// \brief Gets whether the weapon is using a silencer
		/// \return returns whether the weapon is using a silencer
		bool isSilenced();

		sf::VertexArray getBullet();
};