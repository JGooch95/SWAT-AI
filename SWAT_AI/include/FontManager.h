#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

/// \brief A container of fonts to hold them in one location
class FontManager
{
private:
	static bool m_bInstanceFlag; //!< Holds whether the font manager has been created
	static FontManager* m_FontLocation; //!< Holds the memory location of the object
	std::vector<sf::Font*> m_vFonts; //!< Holds the font data

public:
	/// \brief Gets a pointer to the singleton object and creates one if it doesnt exist.
	/// \return returns the pointer to the object
	static FontManager* getInstance(); //Gets the pointer to the object

	/// \brief Loads the fonts from the directories given
	/// \param vsNewFonts The directories to the fonts
	void loadFonts(std::vector<std::string> vsNewFonts); //Loads every Font

	/// \brief Returns the font at the index given
	/// \param iIndex the index of the font
	/// \return returns a pointer to the font
	sf::Font* getFont(int iIndex); //Returns the font at the index given
};