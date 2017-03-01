#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class TextureLoader
{
	private:
		std::vector<sf::Texture> m_Textures; //Container of textures
		std::string m_sBaseDirectory; //Directory where the textures are kept
	public:
		TextureLoader(); //Constructor
		void loadTextures(std::vector<std::string> fileDirs); //Loads the textures from the direcotries given
		sf::Texture* getTexture(const int &kiIndex); //Gets the texture at the index given
		void setBaseDirectory(std::string sDir); //Sets the directory holding the textures
};