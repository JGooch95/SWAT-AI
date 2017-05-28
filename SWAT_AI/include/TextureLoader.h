#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

/// \brief A container of textures to hold them in one location
class TextureLoader
{
	private:
		static bool m_bInstanceFlag; //!< Holds whether the texture loader has been created
		static TextureLoader* m_TextureLocation; //!< Holds the memory location of the object

		std::vector<sf::Texture> m_vTextures; //!< Container of textures
		std::string m_sBaseDirectory; //!< Directory where the textures are kept

	public:
		/// \brief Gets a pointer to the singleton object and creates one if it doesnt exist.
		/// \return returns the pointer to the object
		static TextureLoader* getInstance();

		/// \brief Loads the textures from the directories given
		/// \param fileDirs The directories to the textures
		void loadTextures(std::vector<std::string> fileDirs);

		//Setters
		/// \brief Sets the base directory of the texture locations
		/// \param iIndex The base directory of the texture locations
		void setBaseDirectory(std::string sDir);

		//Getters
		/// \brief Returns the texture at the index given
		/// \param iIndex the index of the texture
		/// \return returns a pointer to the texture
		sf::Texture* getTexture(const int &kiIndex);

};