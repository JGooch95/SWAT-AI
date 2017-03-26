#pragma once
#include "../include/Object.h"
#include "SoundManager.h"

class Entrance : public Object
{
	private:
		bool m_bOpen;
		float m_fOrientation;
		int m_iOpeningDirection;
		sf::Sound m_OpeningSound;
		SoundManager* m_SoundManager;
		sf::Vector2u m_Tile;

	public:
		Entrance(); //Constructor

		//Setters
		void setOpen(bool bState);
		void setOrientation(float fAngle);
		void setDirection(int iDir);
		void setTile(sf::Vector2u Tile);

		//Getters
		bool isOpen();
		sf::Vector2u getTile();
};
