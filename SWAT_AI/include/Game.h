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
		sf::RectangleShape toolbar;
		std::vector<sf::RectangleShape*> unitScreen;
		std::vector<Bar*> UIBars;
		std::vector<sf::Text*> UIText;
		sf::Font currentFont;
		std::vector<Button> Button1;

		//Tools
		AStar m_Pathfinder;
		Map m_CurrentMap;

		TextureLoader m_Textures; //Contains all textures used in the game

		//Objects
		std::vector<Character*> m_Characters; //Holds all of the characters
		std::vector<Character*> m_Units; //Holds the friendly units
		std::vector<Character*> m_Enemies; //Holds the enemy units
		std::vector<Object*> m_Walls; //Holds the walls
		std::vector<Entrance*> m_Doors; //Holds the doors
		sf::VertexArray m_EdgeLines;
		std::vector<sf::Vector2f> m_Edges; //Holds the wall edges

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Game(sf::Vector2u windowSize); //Constructor

		void update(sf::Vector2i mousePos); //Updates all objects in game

		void clickRight(sf::Vector2i mousePos); //Processes right click
		void clickLeft(sf::Vector2i mousePos); //Processes left click
		void characterInteractions(std::vector<Character*> Chars1, std::vector<Character*>  Chars2);

		~Game(); //Deconstructor
};