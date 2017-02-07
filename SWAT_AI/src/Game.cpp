#include "../include/Game.h"

Game::Game(sf::Vector2u WindowSize)
{
	//Sets up the map
	sf::Vector2f GridDimensions = sf::Vector2f(20, 20);
	m_CurrentMap.Setup(WindowSize,
		sf::Vector2f(WindowSize.x / GridDimensions.x, WindowSize.y / GridDimensions.y),
		GridDimensions
	);
	m_CurrentMap.Load("./Assets/Maps/1.txt");

	//Loads the main textures into the software
	m_Textures.LoadTextures(std::vector<std::string>{
		"Assets/Sprites/Unit.png",
		"Assets/Sprites/Wood.jpg",
		"Assets/Sprites/Sniper.png"
	});

	//Creates new units
	for (int i = 0; i < 1; i++)
	{
		m_Units.push_back(new Character);
		m_Units.at(i)->setTexture(m_Textures.getTexture(0)); //Sets the unit texture
		m_Units.at(i)->setGunTexture(m_Textures.getTexture(2)); //Sets the unit texture
		m_Units.at(i)->linkMap(&m_CurrentMap); //Sets up the grid values for units
	}

	//For every bit within the map Check the object type location and create it
	for (int i = 0; i < m_CurrentMap.getMapData().size(); i++)
	{
		for (int j = 0; j < m_CurrentMap.getMapData().at(i).size(); j++)
		{
			//Walls
			if (m_CurrentMap.getMapData().at(i).at(j) == 'W')
			{
				m_Walls.push_back(new Object);
				m_Walls.at(m_Walls.size() - 1)->setTexture(m_Textures.getTexture(1)); //Sets the unit texture
				m_Walls.at(m_Walls.size() - 1)->linkMap(&m_CurrentMap); //Sets up the grid values for units
				m_Walls.at(m_Walls.size() - 1)->setPosition(sf::Vector2f(j * m_CurrentMap.getTileSize().x, i * m_CurrentMap.getTileSize().y));
				m_Walls.at(m_Walls.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x, m_CurrentMap.getTileSize().y));
			}
		}
	}

	//Sets up the pathfinder
	m_Pathfinder.Setup(&m_CurrentMap);
}

void Game::Update(sf::Vector2i MousePos)
{
	for (int i = 0; i < m_Units.size(); i++)
	{
		m_Units.at(i)->lookAt((sf::Vector2f)MousePos - sf::Vector2f(8,30)); //Points the unit towards the mouse
		m_Units.at(i)->update();
	}
}

void Game::ClickRight(sf::Vector2i MousePos)
{
	m_Pathfinder.SetupLists(); //Sets up the pathfinder for a new path
	m_Units.at(0)->setPath(m_Pathfinder.FindPath(m_Units.at(0)->getPosition(), (sf::Vector2f)MousePos)); //Sets a path towards the clicked area
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_CurrentMap); //Draws the Grid

	for (int i = 0; i < m_Walls.size(); i++)
	{
		target.draw(*m_Walls.at(i)); //Draws the units
	}
	target.draw(*m_Units.at(0)); //Draws the units
}