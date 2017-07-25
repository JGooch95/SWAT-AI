#include "../include/TextureLoader.h"

bool TextureLoader::m_bInstanceFlag = false;
TextureLoader* TextureLoader::m_TextureLocation = nullptr;

TextureLoader* TextureLoader::getInstance()
{
	//If the object doesnt exist create the object
	if (!m_bInstanceFlag)
	{
		m_TextureLocation = new TextureLoader();
		m_bInstanceFlag = true;
	}

	//Return the object location
	return m_TextureLocation;
}

void TextureLoader::loadTextures(std::vector<std::string> vsFileDirs)
{
	sf::Texture CurrentTexture;

	//Loads the textures into the vector of textures
	for (int i = 0; i < vsFileDirs.size(); ++i)
	{
		if (!CurrentTexture.loadFromFile(m_sBaseDirectory + vsFileDirs.at(i)))
		{
			//If the texture fails output error
			std::cout << vsFileDirs.at(i) << " failed to load" << "\n";
		}
		m_vTextures.push_back(CurrentTexture);
	}
}

void TextureLoader::setBaseDirectory(std::string sDir)
{
	m_sBaseDirectory = sDir;
}

sf::Texture* TextureLoader::getTexture(const int &kiIndex)
{
	return  std::vector<sf::Texture>::iterator(m_vTextures.begin() + kiIndex)._Ptr;
}