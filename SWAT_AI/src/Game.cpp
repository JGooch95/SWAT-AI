#include "../include/Game.h"

Game::Game(sf::Vector2u windowSize)
{
	//Sets up the map
	sf::Vector2f GridDimensions = sf::Vector2f(20, 20);
	m_CurrentMap.setup(windowSize,
		sf::Vector2f(windowSize.x / GridDimensions.x, windowSize.y / GridDimensions.y),
		GridDimensions
	);
	m_CurrentMap.load("./Assets/Maps/1.txt");

	//Loads the main textures into the software
	m_Textures.loadTextures(std::vector<std::string>{
		"Assets/Sprites/Unit.png",
		"Assets/Sprites/Wood.jpg",
		"Assets/Sprites/Sniper.png",
		"Assets/Sprites/Enemy.png"
	});

	//Creates new units
	for (int i = 0; i < 1; i++)
	{
		m_Units.push_back(new Character);
		m_Units.at(i)->setTexture(m_Textures.getTexture(0)); //Sets the unit texture
		m_Units.at(i)->setGunTexture(m_Textures.getTexture(2)); //Sets the unit texture
		m_Units.at(i)->linkMap(&m_CurrentMap); //Sets up the grid values for units
		m_Characters.push_back(m_Units.at(i));
	}
	//Creates new units
	for (int i = 0; i < 1; i++)
	{
		m_Enemies.push_back(new Character);
		m_Enemies.at(i)->setTexture(m_Textures.getTexture(3)); //Sets the unit texture
		m_Enemies.at(i)->setGunTexture(m_Textures.getTexture(2)); //Sets the unit texture
		m_Enemies.at(i)->linkMap(&m_CurrentMap); //Sets up the grid values for units
		m_Characters.push_back(m_Enemies.at(i));
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

	for (int i = 0; i < m_Walls.size(); i++)
	{
		//Left Edge
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top));
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));

		//Top Edge
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top));
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top));

		//Right Edge
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top));
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));

		//Bottom Edge
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));
		m_Edges.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));
	}

	//Sets up the pathfinder
	m_Pathfinder.setup(&m_CurrentMap);
}

void Game::update(sf::Vector2i mousePos)
{
	for (int i = 0; i < m_Characters.size(); i++)
	{
		//m_Characters.at(i)->lookAt(45.0f);//(sf::Vector2f)mousePos - sf::Vector2f(8,30)); //Points the unit towards the mouse
		m_Characters.at(i)->update();
		m_Characters.at(i)->lazerChecks(m_Edges);
	}

	for (int i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Units.at(0)->lazerChecks({m_Enemies.at(i)->getCollisionLine(m_Units.at(i)->getRotation()).at(0),
										m_Enemies.at(i)->getCollisionLine(m_Units.at(i)->getRotation()).at(1)}))
		{
			m_Units.at(0)->setTarget(m_Enemies.at(i));
			m_Enemies.at(i)->setHealth(m_Enemies.at(i)->getHealth() - m_Units.at(0)->shoot());
		}
		else
		{
			m_Units.at(0)->setTarget(NULL);
		}
	}
}

void Game::clickRight(sf::Vector2i mousePos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	m_Units.at(0)->setPath(m_Pathfinder.findPath(m_Units.at(0)->getPosition(), (sf::Vector2f)mousePos)); //Sets a path towards the clicked area
}

void Game::clickLeft(sf::Vector2i mousePos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	m_Enemies.at(0)->setPath(m_Pathfinder.findPath(m_Enemies.at(0)->getPosition(), (sf::Vector2f)mousePos)); //Sets a path towards the clicked area
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_CurrentMap); //Draws the Grid

	for (int i = 0; i < m_Walls.size(); i++)
	{
		target.draw(*m_Walls.at(i)); //Draws the units
	}
	for (int i = 0; i < m_Characters.size(); i++)
	{
		target.draw(*m_Characters.at(i)); //Draws the characters
	}
}

Game::~Game()
{
	for (int i = 0; i < m_Characters.size(); i++)
	{
		delete(m_Characters.at(i));
		m_Characters.at(i) = NULL;
	}
	for (int i = 0; i < m_Walls.size(); i++)
	{
		delete(m_Walls.at(i));
		m_Walls.at(i) = NULL;
	}
}