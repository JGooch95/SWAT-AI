#include "../include/Entrance.h"

Entrance::Entrance()
{
	m_SoundManager = SoundManager::getInstance();

	setOpen(false);
	m_fOrientation = 0;
	m_iOpeningDirection = 1;
	m_OpeningSound.setBuffer(*m_SoundManager->getSound(5));
}

void Entrance::setOpen(bool bState)
{
	//If the door is being set to be open whilst its closed play the opening sound
	if (m_bOpen != true && bState == true)
	{
		m_OpeningSound.play();
	}
	
	//Sets the rotation of the door depending on if the door is opened or closed
	m_bOpen = bState;
	if (m_bOpen == true)
	{
		m_MainSprite.setRotation(m_fOrientation + (m_iOpeningDirection * 90));
	}
	else
	{
		m_MainSprite.setRotation(m_fOrientation);
	}
}

void Entrance::setOrientation(float fAngle)
{
	m_fOrientation = fAngle;
	setOpen(m_bOpen);
}

void Entrance::setDirection(int iDir)
{
	m_iOpeningDirection = iDir;
}

void Entrance::setTile(sf::Vector2u Tile)
{
	m_Tile = Tile;
}

bool Entrance::isOpen()
{
	return m_bOpen;
}

sf::Vector2u Entrance::getTile()
{
	return m_Tile;
}

