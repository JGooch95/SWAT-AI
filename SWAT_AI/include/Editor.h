#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "Button.h"
#include "TextureLoader.h"

class Editor : public sf::Drawable
{
	private:
		sf::RectangleShape m_Toolbar;
		sf::RectangleShape m_Sidebar;
		Button* exitButton;
		std::vector<sf::Text> UIText;
		std::vector<Button*> gridButtons;

		std::vector<Button*> objectButtons;
		std::vector<Button*> toolButtons;
		std::vector<Button*> floorButtons;

		std::vector<std::vector<Object*>> m_Items;
		std::vector<std::vector<Object*>> m_FloorTiles;

		bool bEditingFloor;

		TextureLoader* m_Textures; //Contains all textures used in the game
		sf::Font m_CurrentFont;
		Map* m_CurrentMap;
		Settings* m_CurrentSettings;

		char cCurrentTool;
		std::vector<std::vector<char>> m_vcLevelBits; //Holds the map layout loaded from the file
		std::vector<std::vector<char>> m_vcFloorBits; 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Editor(sf::Vector2u windowSize);
		void update(sf::Vector2i mousePos); //Updates all objects in game
		int clickLeft(sf::Vector2i mousePos); //Processes left click
		void enableDebug();

		void saveMap();
		~Editor();
};