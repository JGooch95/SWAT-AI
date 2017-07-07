#pragma once

#include <SFML\Graphics.hpp>
#include "Object.h"
#include "Button.h"
#include "TextureLoader.h"
#include <iostream>

enum buttonTypes {ObjectButton, ToolButton, FloorButton};
enum EditTypes {CharacterEdit, FloorEdit, ObjectEdit};

struct EnemyData
{
	sf::Vector2i position;
	std::vector<sf::Vector2i> path;
};

/// \brief The editor screen of the game
class Editor : public sf::Drawable
{
	private:
		//Resources
		FontManager* m_Fonts;  //!< Holds the location of all of the fonts
		Map* m_CurrentMap;  //!< Holds the location of the map data
		Settings* m_CurrentSettings;  //!< Holds the location of the settings
		TextureLoader* m_Textures; //!< Contains all textures used in the game

		//UI
		sf::RectangleShape m_Toolbar; //!< The background of the toolbar containing the exit and save buttons
		sf::RectangleShape m_Sidebar; //!< The background of the tools
		Button* m_ExitButton; //!< Holds the button which exits to the menu
		Button* m_SaveButton; //!< Holds the button which allows saving
		std::vector<sf::Text> m_vUIText; //!< Holds the tool labels

		std::vector<std::vector<Object*>> m_vItems; //!< Holds the item objects
		std::vector<std::vector<Object*>> m_vFloorTiles; //!< Holds the floor bits

		std::vector<Button*> m_vGridButtons; //!< Buttons used for increasing and decreasing the size of the grid
		std::vector<Button*> m_vObjectButtons; //!< Buttons used for placing objects
		std::vector<Button*> m_vToolButtons; //!< Buttons used for altering objects
		std::vector<Button*> m_vFloorButtons; //!< Buttons used for placing floor objects

		std::vector<sf::CircleShape> m_vEnemyCircles; //!< Holds the UI for the paths of the character having their path edited
		sf::RectangleShape Selector; //!< Holds the square used for the selector UI
		sf::VertexArray PathLine; //!< Holds the line of the patrol path the character selected has

		//AI path tools
		EnemyData* iSelectedEnemy; //!< Holds the memory location of the selected enemy using the select tool
		std::vector<EnemyData> m_vEnemyPaths; //!< Holds the patrol paths of the characters
		
		//Layer / Tool Identifiers
		EditTypes m_Editing; //!< States which layer is being edited
		char m_cCurrentTool; //!< Holds the character of the bit being placed

		std::vector<std::vector<char>> m_vcLevelBits; //!< Holds the map layout 
		std::vector<std::vector<char>> m_vcFloorBits; //!< Holds the floor layout

		/// \brief Draws all of the editor's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		/// \param windowSize The size of the window in context
		Editor(sf::Vector2u windowSize); 

		/// \brief Creates buttons for the editor
		/// \param vButtonSet Holds a pointer to the list of buttons being altered
		/// \param iUIindex Holds the index of the label assosciated with the buttons
		/// \param iTexIndex Holds the indexes of all of the button textures
		void createButtons(std::vector<Button*>* vButtonSet, int iUIindex, std::vector<int> iTexIndex);

		/// \brief Updates all objects in the editor
		/// \param mousePos The current position of the mouse
		void update(sf::Vector2i mousePos);

		/// \brief Processes left click
		/// \param mousePos The current position of the mouse
		/// \return Returns an action if necessary
		int clickLeft(sf::Vector2i mousePos); 

		/// \brief Saves the map to a file
		void saveMap(); 

		void loadMap(std::string sDir);

		/// \brief Updates the UI for the path editor
		void updatePathUI();

		/// \brief Default deconstructor
		~Editor();
};