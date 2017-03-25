#pragma once

#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

#include "TextureLoader.h"
#include "Object.h"
#include "Button.h"

class Menu : public sf::Drawable
{
	private:
		//RESOURCES
		TextureLoader* m_Textures; //Contains all textures used in the game
		sf::Font m_CurrentFont;
		sf::Text m_CurrentText;

		sf::RectangleShape m_Background;
		std::vector<Button*> m_Buttons;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Menu(sf::Vector2u windowSize); //Constructor
		void update(sf::Vector2i mousePos); //Updates all objects in game
		int clickLeft(sf::Vector2i mousePos); //Processes left click
		~Menu(); //Deconstructor
};