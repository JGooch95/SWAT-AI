#pragma once

#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

#include "TextureLoader.h"
#include "FontManager.h"
#include "Object.h"
#include "Button.h"
#include "ContentScreen.h"


/// \brief The menu screen
class Menu : public ContentScreen
{
	private:
		sf::Text m_TitleText; //!< Holds the title
		sf::RectangleShape m_Background; //!< Background for the overall screen
		sf::RectangleShape m_ContentBox; //!< Background for the content
		std::vector<Button*> m_Buttons;	 //!< Buttons to click on

		/// \brief Draws all of the menu's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		/// \param windowSize The size of the current window in context
		Menu(sf::Vector2u windowSize); 

		/// \brief Updates all objects in the menu
		/// \param mousePos The position of the mouse
		void update(sf::Vector2i mousePos);

		int processInput(sf::Event keyCode, sf::Vector2i mousePos);

		/// \brief Default deconstructor
		~Menu();
};