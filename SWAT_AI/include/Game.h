#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Character.h"
#include "TextureLoader.h"
#include "Object.h"
#include "Entrance.h"
#include "Map.h"
#include "AStar.h"
#include "Button.h"

class Game : public sf::Drawable
{
	private:
		//UI
		sf::RectangleShape m_Toolbar;

		//UI BOX--------------------------------------------------
		std::vector<sf::RectangleShape*> unitScreen;
		std::vector<Bar*> UIBars;
		std::vector<sf::Text*> UIText;
		std::vector<Button> Button1;
		std::vector<Object> Icons;
		//--------------------------------------------------------

		//Tools
		AStar m_Pathfinder;
		Map m_CurrentMap;

		//RESOURCES
		TextureLoader m_Textures; //Contains all textures used in the game
		sf::Font m_CurrentFont;

		//Objects
		std::vector<Character*> m_vCharacters; //Holds all of the characters
		std::vector<Character*> m_vUnits; //Holds the friendly units
		std::vector<Character*> m_vEnemies; //Holds the enemy units
		std::vector<Object*> m_vWalls; //Holds the walls
		std::vector<Entrance*> m_vDoors; //Holds the doors
		std::vector<sf::Vector2f> m_vEdges; //Holds the wall edges
		
		//DEBUGGING LINES
		sf::VertexArray m_EdgeLines;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Game(sf::Vector2u windowSize); //Constructor
		void update(sf::Vector2i mousePos); //Updates all objects in game
		void characterInteractions(std::vector<Character*> charSet1, std::vector<Character*>  charSet2); //Processes interactions between 2 sets of characters
		void clickLeft(sf::Vector2i mousePos); //Processes left click
		void clickRight(sf::Vector2i mousePos); //Processes right click
		~Game(); //Deconstructor
};