#include "../include/FontManager.h"

bool  FontManager::m_bInstanceFlag = false;
FontManager*  FontManager::m_FontLocation = nullptr;

FontManager*  FontManager::getInstance()
{
	//If the object doesnt exist create the object
	if (!m_bInstanceFlag)
	{
		m_FontLocation = new  FontManager();
		m_bInstanceFlag = true;
	}

	//Return the object location
	return m_FontLocation;
}

void FontManager::loadFonts(std::vector<std::string> vsNewFonts)
{
	//For every directory
	for (int i = 0; i < vsNewFonts.size(); ++i)
	{
		//Load the file and store it
		m_vFonts.push_back(new sf::Font());
		if (!m_vFonts.at(i)->loadFromFile(vsNewFonts.at(i)))
		{
			std::cout << vsNewFonts.at(i) << " failed to load" << "\n";
		}
	}
}

sf::Font* FontManager::getFont(int iIndex)
{
	return m_vFonts.at(iIndex);
}