#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <iostream>

class Map : public sf::Drawable
{
	private:
		static bool m_bInstanceFlag;
		sf::Vector2f m_Position; //Holds the position where the map starts
		sf::Vector2f m_TileSize; //Holds the size of each tile on the grid
		sf::Vector2f m_GridDimensions; //Holds how many tiles there are in each axis
		sf::Vector2u m_WindowSize; //Holds the current window size

		sf::VertexArray m_Grid; //Holds the lines for the grid
		std::vector<std::vector<char>> m_vcLevelBits; //Holds the map layout loaded from the file

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Map(); //Constructor

		static Map* mapLocation;
		static Map* getInstance();

		void setup(sf::FloatRect area, sf::Vector2f newGridDims);
		void load(std::string sDir);
		void setupGrid();
		void setDimensions(sf::Vector2f newGridDims);

		//Setters
		void setPosition(sf::Vector2f pos);
		void setLevelBits(std::vector<std::vector<char>> vcNewBits);

		//Getters
		std::vector<std::vector<char>> getMapData();
		sf::Vector2f getTileSize();
		sf::Vector2f getGridDims();
		sf::Vector2u getWindowSize();
		sf::Vector2f getPosition();
};