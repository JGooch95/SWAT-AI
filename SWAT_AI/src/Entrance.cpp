#include "../include/Entrance.h"

Entrance::Entrance()
{
	//m_MainSprite.setOrigin();
	setOpen(false);
	m_fOrientation = 0;
	m_iOpeningDirection = 1;
}

void Entrance::setOpen(bool bState)
{
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

void Entrance::setDirection(int iDir)
{
	m_iOpeningDirection = iDir;
}

void Entrance::setOrientation(float fAngle)
{
	m_fOrientation = fAngle;
	setOpen(m_bOpen);
}

bool Entrance::isOpen()
{
	return m_bOpen;
}

void Entrance::setTile(sf::Vector2f Tile)
{
	m_Tile = Tile;
}

sf::Vector2f Entrance::getTile()
{
	return m_Tile;
}

