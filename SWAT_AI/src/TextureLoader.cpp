#include "../include/TextureLoader.h"

TextureLoader::TextureLoader()
{

}

void TextureLoader::loadTextures(std::vector<std::string> fileDirs)
{
	sf::Texture CurrentTexture;

	//Loads the textures into the vector of textures
	for (int i = 0; i < fileDirs.size(); i++)
	{
		if (!CurrentTexture.loadFromFile(m_sBaseDirectory + fileDirs.at(i)))
		{
			//If the texture fails output error
			sf::err() << "Texture not loaded";
		}
		m_Textures.push_back(CurrentTexture);
	}
}

void TextureLoader::setBaseDirectory(std::string sDir)
{
	m_sBaseDirectory = sDir;
}

sf::Texture* TextureLoader::getTexture(const int &kiIndex)
{
	return  std::vector<sf::Texture>::iterator(m_Textures.begin() + kiIndex)._Ptr;
}