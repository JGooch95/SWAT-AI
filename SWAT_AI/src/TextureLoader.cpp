#include "../include/TextureLoader.h"

TextureLoader::TextureLoader()
{

}

bool TextureLoader::m_bInstanceFlag = false;
TextureLoader* TextureLoader::loaderLocation = nullptr;

TextureLoader* TextureLoader::getInstance()
{
	if (!m_bInstanceFlag)
	{
		loaderLocation = new TextureLoader();
		m_bInstanceFlag = true;
	}
	return loaderLocation;
}

void TextureLoader::loadTextures(std::vector<std::string> vsFileDirs)
{
	sf::Texture CurrentTexture;

	//Loads the textures into the vector of textures
	for (int i = 0; i < vsFileDirs.size(); i++)
	{
		if (!CurrentTexture.loadFromFile(m_sBaseDirectory + vsFileDirs.at(i)))
		{
			//If the texture fails output error
			sf::err() << "Texture not loaded";
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