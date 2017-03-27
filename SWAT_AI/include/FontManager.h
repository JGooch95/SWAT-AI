#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
class FontManager
{
private:
	static bool m_bInstanceFlag; //Holds whether the font manager has been created
	static FontManager* m_FontLocation; //Holds the memory location of the object
	std::vector<sf::Font*> m_vFonts; //Holds the font data

public:
	static FontManager* getInstance(); //Gets the pointer to the object

	void loadFonts(std::vector<std::string> vsNewFonts); //Loads every Font
	sf::Font* getFont(int iIndex); //Returns the font at the index given
};