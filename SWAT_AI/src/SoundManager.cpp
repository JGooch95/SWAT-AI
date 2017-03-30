#include "../include/SoundManager.h"

bool SoundManager::m_bInstanceFlag = false;
SoundManager* SoundManager::m_SoundLocation = nullptr;

SoundManager* SoundManager::getInstance()
{
	//If the object doesnt exist create the object
	if (!m_bInstanceFlag)
	{
		m_SoundLocation = new SoundManager();
		m_bInstanceFlag = true;
	}

	//Return the object location
	return m_SoundLocation;
}

void SoundManager::loadSounds(std::vector<std::string> vsNewSounds)
{
	//For every directory
	for (int i = 0; i < vsNewSounds.size(); i++)
	{
		//Load the file and store it
		m_vBuffers.push_back(sf::SoundBuffer());
		if (!m_vBuffers.at(i).loadFromFile(vsNewSounds.at(i)))
		{
			std::cout << vsNewSounds.at(i) << " failed to load" << "\n";
		}
	}
}

sf::SoundBuffer* SoundManager::getSound(int iIndex)
{
	return &m_vBuffers.at(iIndex);
}