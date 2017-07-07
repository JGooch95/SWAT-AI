#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

class Settings
{
	private:
		static bool m_bInstanceFlag; //!< Holds whether the Settings have been created
		static Settings* m_SettingsLocation; //!< Holds the memory location of the object

		bool m_bDebug; //!< Holds whether the system is in debug mode or not
		sf::Vector2f m_WindowResolution;//!< Holds the pixel resolution of the game window
		float m_fVolume;//!< Holds the master volume for all of the sounds

	public:
		/// \brief Default constructor
		Settings();

		/// \brief Gets a pointer to the singleton object and creates one if it doesnt exist.
		/// \return returns the pointer to the object
		static Settings* getInstance();

		/// \brief Saves game settings in the file specified
		/// \param sDir Directory to store game settings
		void save(std::string sDir);

		/// \brief Loads game settings from the file specified
		/// \param sDir Directory to gather game settings from
		void load(std::string sDir);
		
		//Setters
		/// \brief Sets the debug state
		/// \param bSetting The state of debug being set
		void setDebug(bool bSetting); 

		/// \brief Sets the resolution of the window
		/// \param newRes The new resolution to set
		void setResolution(sf::Vector2f newRes);


		/// \brief Sets the master volume for the sound
		/// \param fNewVolume The volume level being set
		void setVolume(float fNewVolume);

		//Getters
		/// \brief Gets whether debug mode is active
		/// \return Returns whether debug is active
		bool debugActive();

		/// \brief Gets the current window resolution
		/// \return Returns the window pixel dimensions
		sf::Vector2f getResolution();

		/// \brief Gets the master volume for the sound
		/// \return Returns the master volume for the sound
		float getVolume();
};