#include "../include/TextureLoader.h"

TextureLoader::TextureLoader()
{

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