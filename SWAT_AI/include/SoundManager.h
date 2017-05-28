#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

/// \brief A container of sounds to hold them in one location
class SoundManager
{
	private:
		static bool m_bInstanceFlag; //!< Holds whether the sound manager has been created
		static SoundManager* m_SoundLocation; //!< Holds the memory location of the object
		std::vector<sf::SoundBuffer> m_vBuffers; //!< Holds the sound data

	public:
		/// \brief Gets a pointer to the singleton object and creates one if it doesnt exist.
		/// \return returns the pointer to the object
		static SoundManager* getInstance();

		/// \brief Loads the sounds from the directories given
		/// \param vsNewSounds The directories to the sounds
		void loadSounds(std::vector<std::string> vsNewSounds);

		/// \brief Returns the sound at the index given
		/// \param iIndex the index of the sound
		/// \return returns a pointer to the sound
		sf::SoundBuffer* getSound(int iIndex); 
};