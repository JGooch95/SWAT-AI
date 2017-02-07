#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <iostream>

class Map : public sf::Drawable
{
	private:
		sf::Vector2f m_TileSize;
		sf::Vector2f m_GridDimensions;
		sf::Vector2u m_WindowSize;
		sf::VertexArray m_Grid;
		std::vector<std::vector<char>> m_LevelBits;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Map();
		void Setup(sf::Vector2u WindowSize, sf::Vector2f NewTileSize, sf::Vector2f NewGridDims);
		void Load(std::string sDir);
		void SetupGrid();
		std::vector<std::vector<char>> getMapData();
		sf::Vector2f getTileSize();
		sf::Vector2f getGridDims();
		sf::Vector2u getWindowSize();
};