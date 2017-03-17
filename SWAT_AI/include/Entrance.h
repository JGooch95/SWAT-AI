#pragma once
#include "../include/Object.h"

class Entrance : public Object
{
	private:
		bool m_bOpen;
		float m_fOrientation;
		int m_iOpeningDirection;
		sf::Vector2f m_Tile;

	public:
		Entrance();
		void setOpen(bool bState);
		bool isOpen();
		void setOrientation(float fAngle);
		void setDirection(int iDir);
		void setTile(sf::Vector2f Tile);
		sf::Vector2f getTile();
};
