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
#include "Settings.h"
#include "HUDWindow.h"
#include "SoundWave.h"

class Game : public sf::Drawable
{
	private:
		//UI
		sf::RectangleShape m_Toolbar;
		std::vector<HUDWindow*> m_vUnitUI;
		
		//Tools
		Map* m_CurrentMap;
		Settings* m_CurrentSettings;

		//RESOURCES
		TextureLoader* m_Textures; //Contains all textures used in the game

		//Objects
		std::vector<Character*> m_vCharacters; //Holds all of the characters
		std::vector<Character*> m_vUnits; //Holds the friendly units
		std::vector<Character*> m_vEnemies; //Holds the enemy units
		std::vector<Object*> m_vWalls; //Holds the walls
		std::vector<Entrance*> m_vDoors; //Holds the doors
		std::vector<Object*> m_Background;
		std::vector<sf::Vector2f> m_vEdges; //Holds the wall edges
		
		Button* exitButton; //Holds the button which exits to the menu

		//DEBUGGING LINES
		sf::VertexArray m_EdgeLines;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Game(sf::Vector2u windowSize); //Constructor
		void update(sf::Vector2i mousePos); //Updates all objects in game
		void characterInteractions(std::vector<Character*> charSet1, std::vector<Character*>  charSet2); //Processes interactions between 2 sets of characters
		int clickLeft(sf::Vector2i mousePos); //Processes left click
		void clickRight(sf::Vector2i mousePos); //Processes right click 
		std::vector<sf::Vector2f> edgeReduction(std::vector<sf::Vector2f> vXEdges, std::vector<sf::Vector2f> vYEdges); //Reduces the amounts of edges for each wall by combining adjacent edges and removing unreachable edges.
		~Game(); //Deconstructor
};