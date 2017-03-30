#pragma once
#include "../include/Object.h"
#include "SoundManager.h"

class Entrance : public Object
{
	private:
		SoundManager* m_SoundManager; //A link to the sounds
		bool m_bOpen; //States whether the door is open
		float m_fOrientation; //Holds the current orientation
		int m_iOpeningDirection; //Holds the direction at which the door rotates
		sf::Sound m_OpeningSound; // Holds the sound of the door opening
		sf::Vector2u m_Tile; //Holds the tile the door is sitting on

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
