#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class TextureLoader
{
	private:
		static bool m_bInstanceFlag;
		std::vector<sf::Texture> m_vTextures; //Container of textures
		std::string m_sBaseDirectory; //Directory where the textures are kept

	public:
		TextureLoader(); //Constructor
		static TextureLoader* loaderLocation;
		static TextureLoader* getInstance();

		void loadTextures(std::vector<std::string> fileDirs); //Loads the textures from the direcotries given

		//Setters
		void setBaseDirectory(std::string sDir); //Sets the directory holding the textures

		//Getters
		sf::Texture* getTexture(const int &kiIndex); //Gets the texture at the index given

};