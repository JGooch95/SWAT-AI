#pragma once
#include "../include/Object.h"
#include "SoundManager.h"

/// \brief An openable object which allows entry when open
class Entrance : public Object
{
	private:
		SoundManager* m_SoundManager; //!< A link to the sounds
		bool m_bOpen; //!< States whether the door is open
		float m_fOrientation; //!< Holds the current orientation
		int m_iOpeningDirection; //!< Holds the direction at which the door rotates
		sf::Sound m_OpeningSound; //!< Holds the sound of the door opening
		sf::Vector2u m_Tile; //!< Holds the tile the door is sitting on

	public:
		/// \brief Default constructor
		Entrance();

		//Setters
		/// \brief Sets the door to be open or closed
		/// \param bState States if the door is open or closed.
		void setOpen(bool bState);

		/// \brief Sets the orientaion of the door
		/// \param fAngle The new angle of the door
		void setOrientation(float fAngle);

		/// \brief Sets the direction the door swings open
		/// \param iDir The direction the door swings
		void setDirection(int iDir);

		/// \brief Sets the Tiles the door is sitting on
		/// \param Tile The tile being set
		void setTile(sf::Vector2u Tile);

		//Getters
		/// \brief Gets whether the door is open
		/// \return returns whether the door is open
		bool isOpen();

		/// \brief Gets the tile the door is on
		/// \return returns the tile the door is on
		sf::Vector2u getTile();
};
