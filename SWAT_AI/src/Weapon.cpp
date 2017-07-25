#include "../include/Weapon.h"

Weapon::Weapon()
{
	m_Textures = TextureLoader::getInstance();
	m_SoundManager = SoundManager::getInstance();

	//Sets up the AimLine
	m_AimLine = sf::VertexArray(sf::Lines, 2);
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - 2000.0f);
	for (int i = 0; i < m_AimLine.getVertexCount(); ++i)
	{
		m_AimLine[i].color = sf::Color(255, 0, 0, 255);
	}
	//Sets up the bullets
	m_BulletRays.setPrimitiveType(sf::Lines);
	m_BulletRays.resize(2);
	for (int i = 0; i < m_BulletRays.getVertexCount(); ++i)
	{
		m_BulletRays[i].color = sf::Color(255, 255, 0, 255);
	}

	//Sets up the default texture data
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 25, 50));
	muzzleFlash.setTexture(m_Textures->getTexture(11));
	scope.setTexture(m_Textures->getTexture(19));
	silencer.setColor(sf::Color(70, 70, 70, 255));

	//Sets the default fire rate
	m_fFireRate = 0.2f;
	
	//Sets the default limits
	m_RangeLimits.lower = 200.0f;
	m_RangeLimits.upper = 500.0f;

	m_DamageLimits.lower = 1.0f;
	m_DamageLimits.upper = 5.0f;

	m_AmmoLevels.lower = 30.0f;
	m_AmmoLevels.upper = 30.0f;

	m_bShooting = false;
	
	//Sets up the sounds
	m_ShotSound.setBuffer(*m_SoundManager->getSound(3));
	m_SilencedShotSound.setBuffer(*m_SoundManager->getSound(0));
	m_ReloadSound.setBuffer(*m_SoundManager->getSound(6));

	m_ShotSound.setVolume(m_CurrentSettings->getVolume());
	m_SilencedShotSound.setVolume(m_CurrentSettings->getVolume());
	m_ReloadSound.setVolume(m_CurrentSettings->getVolume());

	//Restarts the timers
	m_ReloadClock.restart();
	m_FireRateClock.restart();
}

void Weapon::aim(sf::Vector2f location)
{
	//Finds the unit vector pointing in the direction of the target
	sf::Vector2f vect = location - m_MainSprite.getPosition();
	vect /= Util::magnitude(vect);

	//Sets the rotation of the sprites to point towards the target
	m_MainSprite.setRotation(Util::getAngle(vect) - 90);
	if (m_bSilencer)
	{
		silencer.setRotation(Util::getAngle(vect) - 90);
	}
	if (m_bScope)
	{
		scope.setRotation(Util::getAngle(vect) - 90);
	}

	vect *= 2000.0f; //Multiplies the vector by the desired length

	//Creates an aiming line using the newly calculated vector
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + vect;
}

void Weapon::aim(float fAngle)
{
	//Sets the rotation of the sprites to point towards the target angle
	m_MainSprite.setRotation(fAngle);
	if (m_bSilencer)
	{
		silencer.setRotation(fAngle);
	}
	if (m_bScope)
	{
		scope.setRotation(fAngle);
	}

	//Creates an aiming lineby calculating a vector pointing in that direction
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + Util::rotateVect(sf::Vector2f(2000, 2000), fAngle);
}

void Weapon::update()
{
	m_bShooting = false;

	if (m_bSilencer)
	{
		//Moves the silencer to the end of the gun
		silencer.setPosition(sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y));
		silencer.setSize(sf::Vector2f(getSize().x * (1.0f / 4.0f), getSize().y / 3.0f));
		silencer.setOrigin(sf::Vector2f((silencer.getSize().x / 2.0f), (getOrigin().y - getSize().y)));
	}

	if (m_bScope)
	{
		//Moves the scope to the center of the gun
		scope.setPosition(sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y));
		scope.setSize(sf::Vector2f(getSize().x  /3.0f,getSize().x  / 4.0f));
		scope.setOrigin(sf::Vector2f(scope.getSize().x / 2.0f, ((-getSize().y) + getOrigin().y)/2));
	}

	//Reloading
	if (m_bReloading)
	{
		if (m_ReloadClock.getElapsedTime().asSeconds() >= m_fReloadTime) // If the reload has ended
		{
			setAmmo(getAmmoLevels().upper); //Fill up the ammo
			m_bReloading = false;
		}
	}
	else if (getAmmoLevels().lower <= 0) //If there is no ammo then reload
	{
		reload();
	}
}

void Weapon::shoot()
{
	//Otherwise if the gun can shoot then shoot
	if (!m_bReloading && m_FireRateClock.getElapsedTime().asSeconds() >= m_fFireRate)
	{
		m_bShooting = true;

		//Finds the vector between the character's center and the aiming point
		sf::Vector2f RotVect(getIntersect() - m_MainSprite.getPosition());
		RotVect /= Util::magnitude(RotVect);
	
		float fExtraLength = 0; //Holds an extension to the gun length if a silencer is attatched
		if (m_bSilencer)
		{
			m_SilencedShotSound.play(); //Plays a silenced weapon sound
			fExtraLength = silencer.getSize().y;
		}
		else
		{
			m_ShotSound.play(); // Plays an unsilenced sound
		}
	
		m_BulletRays[0].position = getWeaponEnd(); //Sets the point to shoot from
		
		//Sets the muzzle flash to point out of the gun
		muzzleFlash.setOrigin(sf::Vector2f(muzzleFlash.getSize().x / 2, muzzleFlash.getSize().y));
		muzzleFlash.setSize(getSize() * (1.0f / 3.0f));
		muzzleFlash.setPosition(getWeaponEnd());
		muzzleFlash.setRotation(Util::getAngle(RotVect) + 90);
	
		//Finds the direction the bullet will head towards
		float fRotAngle = Util::getAngle(getIntersect() - m_BulletRays[0].position);
	
		//Performs an increased accuracy if attachments are on the gun
		float fAccuracyBoost = 0.0f;	
		if (m_bLazer)
		{
			fAccuracyBoost += 0.4f;
		}
		if (m_bScope)
		{
			fAccuracyBoost += 0.1f;
		}
	
		//keeps the accuracy between 0 and 1
		float fOverallAccuracy = std::min( m_fAccuracy + fAccuracyBoost, 1.0f);
		fOverallAccuracy = std::max(0.0f, fOverallAccuracy);
	
		//Offsetting the bullet dependant on the accuracy
		float fAccuracyAngle = 0;
		if (fOverallAccuracy != 1)
		{
			//Ranges between -10 degrees and 10 degrees
			int iAccuracyPercent = fOverallAccuracy * 100;
			//Acuracy levels reduce the cone at which bullets can spawn
			fAccuracyAngle = 10.0f * (((rand() % ((100 - iAccuracyPercent) * 2)) - (100 - iAccuracyPercent)) / 100.0f);
		}
		
		//Positions the bullet vector ready for collision checks
		RotVect = Util::rotateVect(sf::Vector2f(2000.0f, 2000.0f), fRotAngle + fAccuracyAngle - 90.0f);
		m_BulletRays[1].position = m_BulletRays[0].position + RotVect;
	
		setAmmo(m_AmmoLevels.lower - 1); //Increment the ammo
		m_FireRateClock.restart(); //Resets the firerate
	}
}

void Weapon::reload()
{
	if (m_AmmoLevels.lower != m_AmmoLevels.upper)
	{
		setAmmo(0);
		//Plays a reload sound and starts the reload duration
		m_ReloadSound.play();
		m_bReloading = true;
		m_ReloadClock.restart();
	}
}

//Setters
void Weapon::setFireRate(float fVal)
{
	m_fFireRate = fVal;
}

void Weapon::setDamage(sf::Vector2f Val)
{
	m_DamageLimits.lower = Val.x;
	m_DamageLimits.upper = Val.y;
}

void Weapon::setRange(sf::Vector2f Val)
{
	m_RangeLimits.lower = (Val.x / 50) * m_CurrentMap->getTileSize().x;
	m_RangeLimits.upper = (Val.y / 50) * m_CurrentMap->getTileSize().x;
}

void Weapon::setIntersect(sf::Vector2f vect)
{
	m_AimLine[1].position = vect;
}

void Weapon::setAmmoLimit(float fAmmoCount)
{
	m_AmmoLevels.upper = fAmmoCount;
}

void Weapon::setAmmo(float fAmmo)
{
	if (fAmmo < 0)
	{
		fAmmo = 0;
	}
	m_AmmoLevels.lower = fAmmo;
}

void Weapon::setReloadTime(float fTime)
{
	m_fReloadTime = fTime;
}

void Weapon::setAccuracy(float fLevel)
{
	m_fAccuracy = fLevel;
}

void Weapon::setLazer(bool bValue)
{
	m_bLazer = bValue;
}

void Weapon::setSilencer(bool bValue)
{
	m_bSilencer = bValue;
}

void  Weapon::setScope(bool bValue)
{
	m_bScope = bValue;
}

void Weapon::setWeaponVolume(float fValue)
{
	m_fWeaponVolume = fValue;
}

void Weapon::setShotSound(int iIndex)
{
	m_ShotSound.setBuffer(*m_SoundManager->getSound(iIndex));
}
void Weapon::setReloadSound(int iIndex)
{
	m_ReloadSound.setBuffer(*m_SoundManager->getSound(iIndex));
}

void Weapon::setWeaponType(classType newClassType)
{
	//Sets up the weapon depending on the class given
	switch (newClassType)
	{
		case Assault:
			setDamage(sf::Vector2f(1.0f, 5.0f));
			setFireRate(0.2f);
			setRange(sf::Vector2f(200.0f, 500.0f));
			setAmmoLimit(30);
			setAmmo(0);
			setReloadTime(2.0f);
			setAccuracy(0.7f);
			setWeaponVolume(500);
			setShotSound(1);
			setReloadSound(6);
			m_MainSprite.setTexture(m_Textures->getTexture(7));
			break;

		case Support:
			setDamage(sf::Vector2f(1.0f, 3.0f));
			setFireRate(0.05f);
			setRange(sf::Vector2f(200.0f, 500.0f));
			setAmmoLimit(100);
			setAmmo(0);
			setReloadTime(5.0f);
			setAccuracy(0.3f);
			setWeaponVolume(700);
			setShotSound(0);
			setReloadSound(6);
			m_MainSprite.setTexture(m_Textures->getTexture(5));
			break;

		case Sniper:
			setDamage(sf::Vector2f(2.0f, 50.0f));
			setFireRate(1.0f);
			setRange(sf::Vector2f(400.0f, 1000.0f));
			setAmmoLimit(1);
			setAmmo(0);
			setReloadTime(4.0f);
			setAccuracy(0.8f);
			setWeaponVolume(800);
			setShotSound(2);
			setReloadSound(9);
			m_MainSprite.setTexture(m_Textures->getTexture(4));
			break;

		case Shotgunner:
			setDamage(sf::Vector2f(0.5f, 60.0f));
			setFireRate(2.0f);
			setRange(sf::Vector2f(0.0f, 250.0f));
			setAmmoLimit(8);
			setAmmo(0);
			setReloadTime(2.5f);
			setAccuracy(0.5f);
			setWeaponVolume(500);
			setShotSound(3);
			setReloadSound(8);
			m_MainSprite.setTexture(m_Textures->getTexture(6));
			break;
	}
}
//Getters
float Weapon::getDamage()
{
	//Calculates the alteration in range from the attatchments
	float fRangeAlter = 0.0f;
	if (m_bSilencer)
	{
		fRangeAlter -=  m_RangeLimits.upper*0.2f;
	}
	if (m_bScope)
	{
		fRangeAlter += m_RangeLimits.upper*0.3f;
	}

	//Calculates the altered ranges whilst capping them at 0
	float fOverallMaxRange =  std::max(0.0f, m_RangeLimits.upper + fRangeAlter);
	float fOverallMinRange = std::max(0.0f, m_RangeLimits.lower + fRangeAlter);

	//Finds the distance between the target and the character
	float fDist = Util::magnitude(m_AimLine[1].position - m_MainSprite.getPosition());
	float fDamage = m_DamageLimits.upper;

	//If the distance is further than the range then lower the damage
	if (fDist >  fOverallMaxRange)
	{
		fDist = fDist - fOverallMaxRange;
		fDamage = m_DamageLimits.upper * (1-(fDist / fOverallMaxRange));
	}
	//If the distance is lower than the range then lower the damge
	else if (fDist < fOverallMinRange)
	{
		fDamage = m_DamageLimits.upper * (fDist / fOverallMinRange);
	}

	//limit the damage to the lowest damage if surpassed
	if (fDamage < m_DamageLimits.lower)
	{
		fDamage = m_DamageLimits.lower;
	}

	return fDamage;
}

float Weapon::getFireRate()
{
	return m_fFireRate;
}

float Weapon::getWeaponVolume()
{
	if (m_bSilencer)
	{
		return m_fWeaponVolume * 0.6f;
	}
	else
	{
		return m_fWeaponVolume;
	}
}

bool Weapon::reloading()
{
	return (m_ReloadClock.getElapsedTime().asSeconds() < m_fReloadTime);
}

Util::Limits Weapon::getAmmoLevels()
{
	return m_AmmoLevels;
}

sf::Vector2f Weapon::getPosition()
{
	return m_MainSprite.getPosition();
}

sf::Vector2f Weapon::getIntersect()
{
	return m_AimLine[1].position;
}

sf::Vector2f  Weapon::getWeaponEnd()
{
	sf::Vector2f RotVect(getIntersect() - m_MainSprite.getPosition()); //Finding the vector between the character's center and the aiming position
	RotVect /= Util::magnitude(RotVect);

	//Checks for the extra length of the silencer.
	float fExtraLength = 0;
	if (m_bSilencer)
	{
		fExtraLength = silencer.getSize().y;
	}

	sf::Vector2f WeaponDist = ((-m_MainSprite.getOrigin().y) + m_MainSprite.getSize().y  + fExtraLength) * RotVect; // Finds the end of the weapon

	return getPosition() + WeaponDist; //Sets the point to shoot from
}

bool Weapon::usingScope()
{
	return m_bScope;
}

bool Weapon::isShooting()
{
	return m_bShooting;
}

bool Weapon::isSilenced()
{
	return m_bSilencer;
}

sf::VertexArray Weapon::getBullet()
{
	return m_BulletRays;
}

void Weapon::setBullet(sf::Vector2f point)
{
	m_BulletRays[1] = point;
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_bLazer || m_CurrentSettings->debugActive())
	{
		target.draw(m_AimLine);
	}
	target.draw(m_MainSprite);
	if (m_bSilencer)
	{
		target.draw(silencer);
	}
	if (m_bScope)
	{
		target.draw(scope);
	}
	if (m_bShooting)
	{
		target.draw(m_BulletRays);
		target.draw(muzzleFlash);
	}
}