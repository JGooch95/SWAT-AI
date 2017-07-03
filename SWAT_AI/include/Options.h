#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Settings.h"
#include "Button.h"
#include "TextureLoader.h"
#include "FontManager.h"
#include "Slider.h"
#include "SoundManager.h"

class Options : public sf::Drawable
{
	private:
		Settings* m_CurrentSettings;  //!< Holds the location of the settings

		TextureLoader* m_Textures; //!< Contains all textures used in the game
		FontManager* m_Fonts; //!< Holds the location to all of the fonts

		sf::Text m_TitleText; //!< Holds the title

		SoundManager* m_SoundManager;  //!< Holds the location of all of the sounds
		sf::Sound m_TestSound; 

		std::vector<sf::Text> m_Titles; //!< Holds the title
		std::vector<sf::Text> m_DataText; //!< Holds the title

		sf::RectangleShape m_Background; //!< Background for the overall screen
		sf::RectangleShape m_ContentBox; //!< Background for the content

		std::vector<Button*> m_UI;
		std::vector<Button*> m_Buttons;	 //!< Buttons to click on
		std::vector<Slider*> m_Sliders;	 //!< Sliders to drag
		int ActiveSlider;

		std::vector<sf::Vector2f> resolutionList;

		/// \brief Draws all of the game's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	public:
		Options(sf::Vector2u windowSize);
		void update(sf::Vector2i mousePos);
		int clickLeft(sf::Vector2i mousePos);
		~Options();


};