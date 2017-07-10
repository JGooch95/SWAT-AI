#pragma once
#include <SFML\Graphics.hpp>

#include "../include/TextureLoader.h"
#include "../include/SoundManager.h"
#include "../include/FontManager.h"
#include "../include/Map.h"
#include "../include/Settings.h"

enum InputProcess {S_None, S_Options, S_Menu, S_Game, S_Editor, Exit };
class ContentScreen : public sf::Drawable
{
	private:
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	protected:
		//RESOURCES
		TextureLoader* m_Textures; //!< Contains all textures used in the game
		FontManager* m_Fonts; //!< Holds the location to all of the fonts
		Map* m_CurrentMap;  //!< Holds the location of the map data
		Settings* m_CurrentSettings;  //!< Holds the location of the settings
		SoundManager* m_SoundManager;  //!< Holds the location of all of the sounds

	public:
		ContentScreen();
		virtual void update(sf::Vector2i mousePos);
		virtual int processInput(sf::Event keyCode, sf::Vector2i mousePos);
};