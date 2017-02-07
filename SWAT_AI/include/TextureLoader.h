#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class TextureLoader
{
	private:
		std::vector<sf::Texture> m_Textures;
		std::string m_sBaseDirectory;
	public:
		TextureLoader();
		void setBaseDirectory(std::string sDir);
		void LoadTextures(std::vector<std::string> fileDirs);
		sf::Texture* getTexture(const int &k_iIndex);
};