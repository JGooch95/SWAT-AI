#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

class SoundManager
{
	private:
		static bool m_bInstanceFlag; //Holds whether the sound manager has been created
		static SoundManager* m_SoundLocation; //Holds the memory location of the object
		std::vector<sf::SoundBuffer*> m_vBuffers; //Holds the sound data

	public:
		static SoundManager* getInstance(); //Gets the pointer to the object

		void loadSounds(std::vector<std::string> vsNewSounds); //Loads every sound to the buffers
		sf::SoundBuffer* getSound(int iIndex); //Returns the sound at the index given
};