#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

class TextureLoader
{
	private:
		static bool m_bInstanceFlag; //Holds whether the texture loader has been created
		static TextureLoader* m_TextureLocation; //Holds the memory location of the object

		std::vector<sf::Texture> m_vTextures; //Container of textures
		std::string m_sBaseDirectory; //Directory where the textures are kept

	public:
		static TextureLoader* getInstance(); //Gets the pointer to the object
		void loadTextures(std::vector<std::string> fileDirs); //Loads the textures from the directories given

		//Setters
		void setBaseDirectory(std::string sDir); //Sets the directory holding the textures

		//Getters
		sf::Texture* getTexture(const int &kiIndex); //Gets the texture at the index given

};