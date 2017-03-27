#pragma once
#include "Object.h"
#include "../include/FontManager.h"

class Button : public Object
{
	private:
		sf::Text m_ButtonText; //Holds the text on the button
		FontManager* m_FontManager; //Holds the font used
		sf::RectangleShape m_Backdrop; //Holds the background
		sf::Color m_DefaultColor; //Holds the base color of the background
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Button(); //Constructor
		bool hovering(sf::Vector2i pos); // Returns whether the mouse is on the button

		//Setters
		void setText(std::string sValue);
		void setBackgroundColor(sf::Color newColor);
};
