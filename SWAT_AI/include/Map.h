#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <iostream>

enum MapType {ObjectMap, FloorMap};
class Map : public sf::Drawable
{
	private:
		static bool m_bInstanceFlag; //Holds whether the map has been created
		static Map* m_MapLocation; //Holds the memory location of the object

		sf::Vector2f m_Position; //Holds the position where the map starts
		sf::Vector2f m_TileSize; //Holds the size of each tile on the grid
		sf::Vector2f m_GridDimensions; //Holds how many tiles there are in each axis
		sf::Vector2u m_WindowSize; //Holds the size of the area containing the map

		sf::VertexArray m_Grid; //Holds the lines for the grid
		std::vector<std::vector<char>> m_vcLevelBits; //Holds the map layout loaded from the file
		std::vector<std::vector<char>> m_vcFloorBits; //Holds the floor layout loaded from the file

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Map(); //Constructor
		static Map* getInstance();  //Gets the pointer to the object

		void setup(sf::FloatRect area, sf::Vector2f newGridDims);//Performs the main setup of the map
		void load(MapType newMap, std::string sDir); //Loads a map file containing the layout
		void setupGrid(); //Sets up the grid for drawing it to the screen

		//Setters
		void setPosition(sf::Vector2f pos);
		void setLevelBits(std::vector<std::vector<char>> vcNewBits);
		void setFloorBits(std::vector<std::vector<char>> vcNewBits);
		void setDimensions(sf::Vector2f newGridDims);

		//Getters
		std::vector<std::vector<char>> getMapData();
		std::vector<std::vector<char>> getFloorData();
		sf::Vector2f getTileSize();
		sf::Vector2f getGridDims();
		sf::Vector2u getWindowSize();
		sf::Vector2f getPosition();
};