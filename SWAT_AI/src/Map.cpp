#include "../include/Map.h"

Map::Map()
{
}

void Map::setup(sf::FloatRect Area, sf::Vector2f newGridDims)
{
	//Sets the main variables
	m_Position = sf::Vector2f(Area.left, Area.top);
	m_WindowSize = sf::Vector2u(Area.width, Area.height);
	m_TileSize = sf::Vector2f(Area.width / newGridDims.x, Area.height / newGridDims.y);
	m_GridDimensions = newGridDims;
	setupGrid();
}

void Map::load(std::string sDir)
{
	std::ifstream mapFile;
	std::string sLine;
	int iLongestLineLength = 0;

	mapFile.open(sDir); //Open the map file

	if (mapFile.is_open()) //If the map is open
	{
		while (sLine!="EOF") //Repeat until end of file reached
		{  
			getline(mapFile, sLine); //Get the next line

			if (sLine != "EOF") //If this is not the end of the file
			{
				m_vcLevelBits.resize(m_vcLevelBits.size() + 1); //Adds another row

				if (sLine.length() > iLongestLineLength) //If the length of the line is longer than the longest current line
				{
					iLongestLineLength = sLine.length(); //It becomes the new longest
				}

				//For every bit read it into the data vector
				for (int i = 0; i < sLine.length(); i++)
				{
					m_vcLevelBits.at(m_vcLevelBits.size() - 1).push_back(sLine.at(i));
				}
			}
		}
	}
	else
	{
		std::cout << "Map File: " << sDir << " could not be opened." << "\n";
	}

	m_GridDimensions.x = iLongestLineLength;
	m_GridDimensions.y = m_vcLevelBits.size();
	m_TileSize = sf::Vector2f(m_WindowSize.x / m_GridDimensions.x, m_WindowSize.y / m_GridDimensions.y);
	setupGrid();
}

void Map::setupGrid()
{
	//Sets up the grid
	m_Grid.setPrimitiveType(sf::Lines);
	sf::Vector2f m_GridCount(m_GridDimensions.x * 2, m_GridDimensions.y * 2);
	m_Grid.resize(m_GridCount.x + m_GridCount.y);

	//X axis.
	for (int i = 0; i < m_GridCount.x; i += 2)
	{
		m_Grid[i] = sf::Vector2f(m_Position.x + (m_TileSize.x * (i / 2.0f)), m_Position.y + 0); //Places the first vertex
		m_Grid[i + 1] = sf::Vector2f(m_Position.x + (m_TileSize.x * (i / 2.0f)), m_Position.y + m_WindowSize.y); //Places the second vertex
	}

	//Y axis.
	for (int i = 0; i < m_GridCount.y; i += 2)
	{
		m_Grid[i + m_GridCount.x] = sf::Vector2f(m_Position.x + 0, m_Position.y + (m_TileSize.y * (i / 2.0f))); //Places the first vertex
		m_Grid[i + 1 + m_GridCount.x] = sf::Vector2f(m_Position.x + m_WindowSize.x, m_Position.y + (m_TileSize.y * (i / 2.0f))); //Places the second vertex
	}
}

sf::Vector2f Map::getTileSize()
{
	return m_TileSize;
}
sf::Vector2f Map::getGridDims()
{
	return m_GridDimensions;
}
sf::Vector2u Map::getWindowSize()
{
	return m_WindowSize;
}

std::vector<std::vector<char>> Map::getMapData()
{
	return m_vcLevelBits;
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Grid);
}

void  Map::setPosition(sf::Vector2f pos)
{
	m_Position = pos;
}

sf::Vector2f  Map::getPosition()
{
	return m_Position;
}