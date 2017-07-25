#include "../include/Map.h"

bool Map::m_bInstanceFlag = false;
Map* Map::m_MapLocation = nullptr;

Map* Map::getInstance()
{
	//If the object doesnt exist create the object
	if (!m_bInstanceFlag)
	{
		m_MapLocation = new Map();
		m_bInstanceFlag = true;
	}

	//Return the object location
	return m_MapLocation;
}


Map::Map()
{
	m_Grid.setPrimitiveType(sf::Lines);
}

void Map::setup(sf::FloatRect area, sf::Vector2f newGridDims)
{
	//Sets the main variables
	m_Position = sf::Vector2f(area.left, area.top);
	m_WindowSize = sf::Vector2u(area.width, area.height);
	setDimensions(newGridDims);
}

void Map::load(MapType newMap, std::string sDir)
{
	//Clears the current map depending on the map type
	switch (newMap)
	{
		case ObjectMap:
			m_vcLevelBits.clear();
			break;

		case FloorMap:
			m_vcFloorBits.clear();
			break;
	}

	std::ifstream mapFile;
	mapFile.open(sDir); //Open the map file

	if (mapFile.is_open()) //If the file opened correctly
	{
		//Initialise reading variables
		int iLongestLineLength = 0;
		std::string sLine;

		while (!mapFile.eof()) //while the end of file hasnt been reached
		{
			getline(mapFile, sLine); //Get the next line

			//Increments the map row
			switch (newMap)
			{
				case ObjectMap:
					m_vcLevelBits.resize(m_vcLevelBits.size() + 1);
					break;
				case FloorMap:
					m_vcFloorBits.resize(m_vcFloorBits.size() + 1);
					break;
			}
			
			if (sLine.length() > iLongestLineLength) //If the length of the line is longer than the longest current line
			{
				iLongestLineLength = sLine.length(); //It becomes the new longest
			}

			//For every bit read it into the data vector
			for (int i = 0; i < sLine.length(); ++i)
			{
				//Adds the bit to the row of the appropriate vector
				switch (newMap)
				{
					case ObjectMap:
						m_vcLevelBits.at(m_vcLevelBits.size() - 1).push_back(sLine.at(i));
						break;
					case FloorMap:
						m_vcFloorBits.at(m_vcFloorBits.size() - 1).push_back(sLine.at(i));
						break;
				}
			}
		}
		//Sets up the variables from the map and sets up the grid
		m_GridDimensions = sf::Vector2f(iLongestLineLength, m_vcLevelBits.size());
		m_TileSize = sf::Vector2f(m_WindowSize.x / m_GridDimensions.x, m_WindowSize.y / m_GridDimensions.y);
		setupGrid();
	}
	else
	{
		//Ouptut an error
		std::cout << "Map File: " << sDir << " could not be opened." << "\n";
	}

	mapFile.close();
}

void Map::setupGrid()
{
	//Sets up the grid size
	sf::Vector2f m_GridCount(m_GridDimensions.x * 2, m_GridDimensions.y * 2);
	m_Grid.resize(m_GridCount.x + m_GridCount.y);

	//Adds the lines incrementally on each axis
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

void Map::setDimensions(sf::Vector2f newGridDims)
{
	//Sets the dimensions to be above 1
	newGridDims.x = std::max(1.0f, newGridDims.x);
	newGridDims.y = std::max(1.0f, newGridDims.y);

	//Scales the tiles to be in proportion to the window size
	m_TileSize = sf::Vector2f(m_WindowSize.x / newGridDims.x, m_WindowSize.y / newGridDims.y); 
	m_GridDimensions = newGridDims;
	setupGrid();
}

//Setters
void  Map::setPosition(sf::Vector2f pos)
{
	m_Position = pos;
}

void Map::setLevelBits(std::vector<std::vector<char>> vcNewBits)
{
	m_vcLevelBits = vcNewBits;
}

void Map::setFloorBits(std::vector<std::vector<char>> vcNewBits)
{
	m_vcFloorBits = vcNewBits;
}

//Getters
std::vector<std::vector<char>> Map::getMapData()
{
	return m_vcLevelBits;
}

std::vector<std::vector<char>> Map::getFloorData()
{
	return m_vcFloorBits;
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

sf::Vector2f Map::getPosition()
{
	return m_Position;
}

void Map::clearEdges(std::vector<std::pair<sf::Vector2f*, sf::Vector2f*>>* EdgeVector)
{
	//Clears the pointers for each point of an edge
	for (int i = 0; i < EdgeVector->size(); ++i)
	{
		if (EdgeVector->at(i).first != NULL)
		{
			EdgeVector->at(i).first = NULL;
		}

		if (EdgeVector->at(i).second != NULL)
		{
			EdgeVector->at(i).second = NULL;
		}
	}
	EdgeVector->clear();
}

void Map::clearCorners(std::vector<sf::Vector2f*>* CornerVector)
{
	//Clears the pointer of each corner
	for (int i = 0; i < CornerVector->size(); ++i)
	{
		if (CornerVector->at(i) != NULL)
		{
			delete(CornerVector->at(i));
			CornerVector->at(i) = NULL;
		}
	}
	CornerVector->clear();
}

void Map::clearAllEdgeData()
{
	clearCorners(&m_vTempCorners);
	clearCorners(&m_vWallCorners);

	//Sets the pointers to null as all edges are sorted
	for (int i = 0; i < m_vCorners.size(); ++i)
	{
		if (m_vCorners.at(i) != NULL)
		{
			m_vCorners.at(i) = NULL;
		}
	}
	m_vCorners.clear();

	clearEdges(&m_vTempEdges);
	clearEdges(&m_vWallEdges);
	clearEdges(&m_vEdges);
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Grid);
}

Map::~Map()
{
	clearAllEdgeData();
}