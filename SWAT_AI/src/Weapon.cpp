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

	//Sets up the default texture data
	m_MainSprite.setTextureRect(sf::IntRect(0, 0, 25, 50));
	m_MainSprite.setOrigin(m_MainSprite.getLocalBounds().width / 2, -m_MainSprite.getLocalBounds().height / 2 + 10);

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

	m_ReloadClock.restart();
	m_FireRateClock.restart();
	bShooting = false;
}

void Weapon::aim(sf::Vector2f location)
{
	//Finds the unit vector
	float fMagnitude = sqrtf(pow(location.x, 2.0f) + pow(location.y, 2.0f));
	location /= fMagnitude;

	float fRotAngle = -atan2f(location.x, location.y) * (180.0f / 3.14f); // Finding the angle of the vector given

	location *= 2000.0f; //Multiplies the vector by the desired length

	//Sets the rotation of the sprite and adjusts the aimline according to the rotation
	m_MainSprite.setRotation(fRotAngle);

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + location;
}

void Weapon::aim(float fAngle)
{
	//Points a vector towards the angle given
	sf::Vector2f RotVect(2000.0f * cos((fAngle + 90.0f) * (3.14159265359 / 180)),
						 2000.0f * sin((fAngle + 90.0f) * (3.14159265359 / 180))); 


	//Sets the rotation of the sprite and adjusts the aimline according to the rotation
	m_MainSprite.setRotation(fAngle);

	m_AimLine[0].position = m_MainSprite.getPosition();
	m_AimLine[1].position = m_MainSprite.getPosition() + RotVect;
}

void Weapon::update()
{
	bShooting = false;
	m_MainSprite.setScale(m_CurrentMap->getTileSize().x / 25, m_CurrentMap->getTileSize().y / 50);

	muzzleFlash.setOrigin(sf::Vector2f(muzzleFlash.getSize().x / 2, muzzleFlash.getSize().y));
	muzzleFlash.setSize(sf::Vector2f(m_MainSprite.getLocalBounds().width * m_MainSprite.getScale().x, m_MainSprite.getLocalBounds().height* m_MainSprite.getScale().y) * (1.0f / 3.0f));

	if (m_bReloading)
	{
		if (m_ReloadClock.getElapsedTime().asSeconds() >= m_fReloadTime)
		{
			setAmmo(getAmmoLevels().upper);
			m_bReloading = false;
		}
	}
	else
	{
		//If there is no ammo then reload
		if (getAmmoLevels().lower <= 0)
		{
			reload();
		}
	}
}

void Weapon::shoot()
{
	if (!m_bReloading)
	{
		//Otherwise if the gun can shoot then shoot
		if (m_FireRateClock.getElapsedTime().asSeconds() >= m_fFireRate)
		{
			bShooting = true;

			sf::Vector2f RotVect(getIntersect() - m_MainSprite.getPosition()); //Finding the vector between the character's center and the mouse
			float fMagnitude = sqrtf(pow(RotVect.x, 2.0f) + pow(RotVect.y, 2.0f));
			RotVect /= fMagnitude;

			sf::Vector2f WeaponDist = (getSize().y * getScale().y) * RotVect; // Finds the end of the weapon
			RotVect *= (m_MainSprite.getLocalBounds().height * m_MainSprite.getScale().y) / 2; //Gets the edge of the character from the center

			m_BulletRays[0].position = getPosition() + RotVect + WeaponDist; //Sets the point of shooting

			float fRotAngle = atan2f(RotVect.y, RotVect.x) * (180.0f / 3.14f); // Finding the angle of the weapon

																			   //Sets the muzzle flash to point towards out of the gun
			muzzleFlash.setPosition(getPosition() + RotVect + WeaponDist);
			muzzleFlash.setRotation(fRotAngle + 90);

			//Gets the vector from the weapon to the lazer point
			RotVect = getIntersect() - m_BulletRays[0].position;
			fRotAngle = atan2f(RotVect.y, RotVect.x) * (180.0f / 3.14f); // Finding the angle of the vector

																		 //Offsetting the vector dependant on the accuracy
			float AccuracyAngle = 0;
			if (m_fAccuracy != 1)
			{
				//Ranges between -10 degrees and 10 degrees
				int b = m_fAccuracy * 100;
				AccuracyAngle = 10.0f * (((rand() % ((100 - b) * 2)) - (100 - b)) / 100.0f);
			}

			//Finds the collision point
			RotVect = m_BulletRays[0].position + sf::Vector2f(2000.0f*cos((fRotAngle + AccuracyAngle) * (3.14159265359f / 180.0f)),
				2000.0f* sin((fRotAngle + AccuracyAngle) * (3.14159265359f / 180.0f)));
			m_BulletRays[1].position = RotVect;

			m_FireRateClock.restart();

			setAmmo(m_AmmoLevels.lower - 1);
		}
	}
}

void Weapon::reload()
{
	m_bReloading = true;
	m_ReloadClock.restart();
}

float Weapon::bulletChecks(std::vector<sf::Vector2f>vEdges)
{
	if (bShooting)
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

//Getters
float Weapon::getDamage()
{
	//Finds the distance between the target and the character
	float dist = Util::magnitude(m_AimLine[1].position - m_MainSprite.getPosition());
	float damage = m_DamageLimits.upper;

	//If the distance is further than the range then lower the damage
	if (dist > m_RangeLimits.upper)
	{
		dist = dist - m_RangeLimits.upper;
		damage = m_DamageLimits.upper * (1-(dist / m_RangeLimits.upper));
	}
	//If the distance is lower than the range then lower the damge
	else if (dist < m_RangeLimits.lower)
	{
		damage = m_DamageLimits.upper * (dist / m_RangeLimits.lower);
	}

	//limit the damage to the lowest damage if surpassed
	if (damage < m_DamageLimits.lower)
	{
		damage = m_DamageLimits.lower;
	}

	return damage;
}

float Weapon::getFireRate()
{
	return m_fFireRate;
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

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_AimLine);
	target.draw(m_MainSprite);
	if (bShooting)
	{
		target.draw(m_BulletRays);
		target.draw(muzzleFlash);
	}
}