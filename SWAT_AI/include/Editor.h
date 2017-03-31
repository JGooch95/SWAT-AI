#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "Button.h"
#include "TextureLoader.h"

enum buttonTypes {ObjectButton, ToolButton, FloorButton};
enum EditTypes {CharacterEdit, FloorEdit, ObjectEdit};

class Editor : public sf::Drawable
{
	private:
		//Resources
		FontManager* m_Fonts;
		Map* m_CurrentMap;
		Settings* m_CurrentSettings;

		//UI
		sf::RectangleShape m_Toolbar; //The background of the toolbar containing the exit and save buttons
		sf::RectangleShape m_Sidebar; //The background of the tools
		Button* m_ExitButton; //Holds the button which exits to the menu
		Button* m_SaveButton; //Holds the button which allows saving
		std::vector<sf::Text> m_vUIText; //Holds the tool labels

		std::vector<Button*> m_vGridButtons; //Buttons used for increasing and decreasing the size of the grid
		std::vector<Button*> m_vObjectButtons; //Buttons used for placing objects
		std::vector<Button*> m_vToolButtons; //Buttons used for altering objects
		std::vector<Button*> m_vFloorButtons; //Buttons used for placing floor objects

		std::vector<std::vector<Object*>> m_vItems; //Holds the item bits
		std::vector<std::vector<Object*>> m_vFloorTiles; //Holds the floor bits

		EditTypes m_Editing; //States whether the floor is being altered

		TextureLoader* m_Textures; //Contains all textures used in the game

		char m_cCurrentTool; //Holds the character of the bit being placed
		std::vector<std::vector<char>> m_vcLevelBits; //Holds the map layout loaded from the file
		std::vector<std::vector<char>> m_vcFloorBits; //Holds the floor layout loaded from the file
		std::vector<std::vector<int>> m_viPaths;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Editor(sf::Vector2u windowSize); //Constructor
		void createButtons(std::vector<Button*>* vButtonSet, int iUIindex, std::vector<int> iTexIndex);
		void update(sf::Vector2i mousePos); //Updates all objects in game
		int clickLeft(sf::Vector2i mousePos); //Processes left click
		void saveMap(); //Saves the map to a file
		~Editor();
};