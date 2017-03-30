#pragma once

#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

#include "TextureLoader.h"
#include "FontManager.h"
#include "Object.h"
#include "Button.h"

class Menu : public sf::Drawable
{
	private:
		//RESOURCES
		TextureLoader* m_Textures; //Contains all textures used in the game
		FontManager* m_Fonts;

		sf::Text m_TitleText; //Holds the title
		sf::RectangleShape m_Background; //Background for the overall screen
		sf::RectangleShape m_ContentBox; //Background for the content
		std::vector<Button*> m_Buttons;	 // Buttons to click on

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Menu(sf::Vector2u windowSize); //Constructor
		void update(sf::Vector2i mousePos); //Updates all objects in game
		int clickLeft(sf::Vector2i mousePos); //Processes left click
		~Menu(); //Deconstructor
};