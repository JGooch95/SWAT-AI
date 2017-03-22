#include "../include/Entrance.h"

Entrance::Entrance()
{
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

