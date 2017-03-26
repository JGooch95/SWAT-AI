#include "../include/SoundManager.h"

bool SoundManager::m_bInstanceFlag = false;
SoundManager* SoundManager::soundLocation = nullptr;

SoundManager* SoundManager::getInstance()
{
	if (!m_bInstanceFlag)
	{
		soundLocation = new SoundManager();
		m_bInstanceFlag = true;
	}
	return soundLocation;
}

void SoundManager::loadSounds(std::vector<std::string> newSounds)
{
	for (int i = 0; i < newSounds.size(); i++)
	{
		buffer.push_back(new sf::SoundBuffer());
		if (!buffer.at(i)->loadFromFile(newSounds.at(i)))
		{
			std::cout << newSounds.at(i) << " failed" << "\n";
		}
	}
}

sf::SoundBuffer* SoundManager::getSound(int iIndex)
{
	return buffer.at(iIndex);
}