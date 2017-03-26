#include "../include/Weapon.h"

Weapon::Weapon()
{
	//Sets up the AimLine
	m_AimLine = sf::VertexArray(sf::Lines, 2);
	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y - 2000.0f);
	for (int i = 0; i < m_AimLine.getVertexCount(); i++)
	{
		m_AimLine[i].color = sf::Color(255, 0, 0, 255);
	}

	m_Textures = TextureLoader::getInstance();

	//Sets up the default texture data
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 25, 50));
	//m_MainSprite.setOrigin(m_MainSprite.getLocalBounds().width / 2, -m_MainSprite.getLocalBounds().height / 2 + 10);

	//Sets the default fire rate
	m_fFireRate = 0.2f;
	
	//Sets the default limits
	m_RangeLimits.lower = 200.0f;
	m_RangeLimits.upper = 500.0f;

	m_DamageLimits.lower = 1.0f;
	m_DamageLimits.upper = 5.0f;

	m_AmmoLevels.lower = 30.0f;
	m_AmmoLevels.upper = 30.0f;

	m_BulletRays.setPrimitiveType(sf::Lines);
	m_BulletRays.resize(2);
	for (int i = 0; i < m_BulletRays.getVertexCount(); i++)
	{
		m_BulletRays[i].color = sf::Color(255, 255, 0, 255);
	}

	m_bShooting = false;

	m_ReloadClock.restart();
	m_FireRateClock.restart();
	m_SoundManager = SoundManager::getInstance();
	m_ShotSound.setBuffer(*m_SoundManager->getSound(3));
	m_SilencedShotSound.setBuffer(*m_SoundManager->getSound(0));
	m_ReloadSound.setBuffer(*m_SoundManager->getSound(6));
}

void Weapon::aim(sf::Vector2f location)
{
	sf::Vector2f vect = location - m_MainSprite.getPosition();

	//Finds the unit vector
	vect /= Util::magnitude(vect);

	//Sets the rotation of the sprite and adjusts the aimline according to the rotation
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

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + vect;
}

void Weapon::aim(float fAngle)
{
	//Sets the rotation of the sprite and adjusts the aimline according to the rotation
	m_MainSprite.setRotation(fAngle);

	if (m_bSilencer)
	{
		silencer.setRotation(fAngle);
	}
	if (m_bScope)
	{
		scope.setRotation(fAngle);
	}

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + Util::rotateVect(sf::Vector2f(2000, 2000), fAngle);
}

void Weapon::update()
{
	m_bShooting = false;

	muzzleFlash.setOrigin(sf::Vector2f(muzzleFlash.getSize().x / 2, muzzleFlash.getSize().y));
	muzzleFlash.setSize(getSize() * (1.0f / 3.0f));

	if (m_bSilencer)
	{
		//Sets the position of the silencer to the position of the gun(Center of the character)
		silencer.setPosition(sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y));
		//Sets the size to be the guns silencer to be the size of the gun ( reduced size in x)
		silencer.setSize(sf::Vector2f(getSize().x * (1.0f / 4.0f), getSize().y / 3.0f));

		//Sets the origin to the guns (half the silencer size, origin + the size of the gun)
		silencer.setOrigin(sf::Vector2f((silencer.getSize().x / 2.0f), (getOrigin().y - getSize().y)));
	}

	if (m_bScope)
	{
		scope.setPosition(sf::Vector2f(m_MainSprite.getPosition().x, m_MainSprite.getPosition().y));
		scope.setSize(sf::Vector2f(getSize().x  /3.0f,getSize().x  / 4.0f));
		scope.setOrigin(sf::Vector2f(scope.getSize().x / 2.0f, ((-getSize().y) + getOrigin().y)/2));
	}

	//Reloading
	if (m_bReloading)
	{
		if (m_ReloadClock.getElapsedTime().asSeconds() >= m_fReloadTime)
		{
			setAmmo(getAmmoLevels().upper);
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
	if (!m_bReloading)
	{
		//Otherwise if the gun can shoot then shoot
		if (m_FireRateClock.getElapsedTime().asSeconds() >= m_fFireRate)
		{
			m_bShooting = true;
			sf::Vector2f RotVect(getIntersect() - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse
			RotVect /= Util::magnitude(RotVect);

			float extraLength(0);

			if (m_bSilencer)
			{
				m_SilencedShotSound.play();
				extraLength = silencer.getSize().y;
			}
			else
			{
				m_ShotSound.play();
			}

			sf::Vector2f WeaponDist = (getSize().y  + extraLength) * RotVect; // Finds the end of the weapon
			RotVect *= getSize().y  / 2; //Gets the edge of the character from the center

			m_BulletRays[0].position = getPosition() + RotVect + WeaponDist; //Sets the point to shoot from
			
			//Sets the muzzle flash to point towards out of the gun
			muzzleFlash.setPosition(getPosition() + RotVect + WeaponDist);
			muzzleFlash.setRotation(Util::getAngle(RotVect) + 90);

			//Finds the direction the bullet will head towards
			float fRotAngle = Util::getAngle(getIntersect() - m_BulletRays[0].position);

			float fAccuracyBoost = 0.0f;

			if (m_bLazer)
			{
				fAccuracyBoost += 0.4f;
			}

			if (m_bScope)
			{
				fAccuracyBoost += 0.1f;
			}

			float fOverallAccuracy = std::min( m_fAccuracy + fAccuracyBoost, 1.0f);
			fOverallAccuracy = std::max(0.0f, fOverallAccuracy);

			//Offsetting the bullet dependant on the accuracy
			float fAccuracyAngle = 0;
			if (fOverallAccuracy != 1)
			{
				//Ranges between -10 degrees and 10 degrees
				int iAccuracyPercent = fOverallAccuracy * 100;
				fAccuracyAngle = 10.0f * (((rand() % ((100 - iAccuracyPercent) * 2)) - (100 - iAccuracyPercent)) / 100.0f);
			}

			RotVect = Util::rotateVect(sf::Vector2f(2000.0f, 2000.0f), fRotAngle + fAccuracyAngle - 90.0f);
			m_BulletRays[1].position = m_BulletRays[0].position + RotVect;

			setAmmo(m_AmmoLevels.lower - 1); //Increment the ammo
			m_FireRateClock.restart();
		}
	}
}

void Weapon::reload()
{
	m_ReloadSound.play();
	m_bReloading = true;
	m_ReloadClock.restart();
}

void Weapon::setupTextures()
{
	silencer.setTexture(m_Textures->getTexture(15));
	scope.setTexture(m_Textures->getTexture(19));
}

float Weapon::bulletChecks(std::vector<sf::Vector2f>vEdges)
{
	if (m_bShooting)
	{
		std::pair<bool, sf::Vector2f> lowestIntersect = Util::findLowestIntersect(vEdges, std::vector<sf::Vector2f>{ m_BulletRays[0].position, m_BulletRays[1].position});

		m_BulletRays[1].position = lowestIntersect.second;
		if (lowestIntersect.first)
		{
			return getDamage();
		}
	}
	return 0;
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
	m_RangeLimits.lower = Val.x;
	m_RangeLimits.upper = Val.y;
}

void Weapon::setOrigin(sf::Vector2f Val)
{
	m_MainSprite.setOrigin(Val);
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

void Weapon::setMuzzle(sf::Texture* tex2)
{
	muzzleFlash.setTexture(tex2);
}

void Weapon::setLazer(bool bValue)
{
	m_bLazer = bValue;
}

void Weapon::setSilencer(bool bValue)
{
	m_bSilencer = bValue;
	/*
	if (isSilenced())
	{
		m_ShotSound.setBuffer(*m_SoundManager->getSound(0));
	}*/
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

//Getters
float Weapon::getDamage()
{
	float fRangeReduction = 0.0f;
	float fRangeIncrease = 0.0f;

	if (m_bSilencer)
	{
		fRangeReduction =  m_RangeLimits.upper*0.2f;
	}
	if (m_bScope)
	{
		fRangeIncrease = m_RangeLimits.upper*0.3f;
	}

	float fOverallMaxRange =  std::max(0.0f, m_RangeLimits.upper - fRangeReduction + fRangeIncrease);
	float fOverallMinRange = std::max(0.0f, m_RangeLimits.lower - fRangeReduction + fRangeIncrease);

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
	sf::Vector2f RotVect(getIntersect() - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse
	RotVect /= Util::magnitude(RotVect);

	float extraLength(0);

	if (m_bSilencer)
	{
		extraLength = silencer.getSize().y *silencer.getScale().y;
	}

	sf::Vector2f WeaponDist = ((getSize().y * getScale().y) + extraLength) * RotVect; // Finds the end of the weapon
	RotVect *= (m_MainSprite.getLocalBounds().height * m_MainSprite.getScale().y) / 2; //Gets the edge of the character from the center

	return getPosition() + RotVect + WeaponDist; //Sets the point to shoot from
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

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_bLazer)
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