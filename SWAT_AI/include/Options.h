#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Settings.h"
#include "Button.h"
#include "TextureLoader.h"
#include "FontManager.h"
#include "Slider.h"
#include "SoundManager.h"
#include "ContentScreen.h"

class Options : public ContentScreen
{
	private:
		sf::Sound m_TestSound; //!< Holds the sound for the sound slider

		sf::Text m_TitleText; //!< Holds the main title
		std::vector<sf::Text> m_vTitles; //!< Holds the titles for the sliders
		std::vector<sf::Text> m_vDataText; //!< Holds the data text for the sliders

		sf::RectangleShape m_Background; //!< Background for the overall screen
		sf::RectangleShape m_ContentBox; //!< Background for the content

		std::vector<Button*> m_vUI;       //!< Holds all UI
		std::vector<Button*> m_vButtons;	 //!< Buttons to click on
		std::vector<Slider*> m_vSliders;	 //!< Sliders to drag

		int m_iActiveSlider; //!< States the active slider being used

		float m_fButtonGap; //!< Holds the distance between each object

		std::vector<sf::Vector2f> resolutionList; //!< Holds all video resolutions to choose

		/// \brief Draws all of the game's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	public:
		/// \brief Constructor
		/// \param windowSize Holds the current window size	   
		Options(sf::Vector2u windowSize);

		/// \brief updates all objects in scene
		/// \param mousePos holds the position of the mouse 
		void update(sf::Vector2i mousePos);

		int processInput(sf::Event keyCode, sf::Vector2i mousePos);

		/// \brief Loads the list of video resolutions
		/// \param sDir Holds the directory holding the list of resolutions 
		void loadResolutionList(std::string sDir);

		/// \brief Default Deconstructor
		~Options();


};