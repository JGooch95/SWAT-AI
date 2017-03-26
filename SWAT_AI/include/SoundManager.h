#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

class SoundManager
{
	private:
		static bool m_bInstanceFlag;
		std::vector<sf::SoundBuffer*> buffer;

	public:
		static SoundManager* soundLocation;
		static SoundManager* getInstance();

		void loadSounds(std::vector<std::string> newSounds);

		sf::SoundBuffer* getSound(int iIndex);
};