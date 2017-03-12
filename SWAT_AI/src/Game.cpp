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

	std::vector<sf::Vector2f> m_EdgesX; //Holds the wall edges
	std::vector<sf::Vector2f> m_EdgesY; //Holds the wall edges

	//Adds borders of the window to the edge checks
	m_EdgesY.push_back(sf::Vector2f(0, 0));
	m_EdgesY.push_back(sf::Vector2f(windowSize.x, 0));

	m_EdgesY.push_back(sf::Vector2f(0, windowSize.y));
	m_EdgesY.push_back(sf::Vector2f(windowSize.x, windowSize.y));

	m_EdgesX.push_back(sf::Vector2f(0, 0));
	m_EdgesX.push_back(sf::Vector2f(0, windowSize.y));

	m_EdgesX.push_back(sf::Vector2f(windowSize.x, 0));
	m_EdgesX.push_back(sf::Vector2f(windowSize.x, windowSize.y));

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

			
				if (m_CurrentMap.getMapData().at(i - 1).at(j) != 'W' && i > 0)
				{
					//Top Edge
					m_EdgesY.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left, m_Walls.at(m_Walls.size() - 1)->getRect().top));
					m_EdgesY.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left + m_Walls.at(m_Walls.size() - 1)->getRect().width, m_Walls.at(m_Walls.size() - 1)->getRect().top));
				}
				if (m_CurrentMap.getMapData().at(i + 1).at(j) != 'W' && i < m_CurrentMap.getMapData().at(i).size())
				{
					//Bottom Edge
					m_EdgesY.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left, m_Walls.at(m_Walls.size() - 1)->getRect().top + m_Walls.at(m_Walls.size() - 1)->getRect().height));
					m_EdgesY.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left + m_Walls.at(m_Walls.size() - 1)->getRect().width, m_Walls.at(m_Walls.size() - 1)->getRect().top + m_Walls.at(m_Walls.size() - 1)->getRect().height));
				}
				
				if (m_CurrentMap.getMapData().at(i).at(j-1) != 'W' && j > 0)
				{
					//Left Edge
					m_EdgesX.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left, m_Walls.at(m_Walls.size() - 1)->getRect().top));
					m_EdgesX.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left, m_Walls.at(m_Walls.size() - 1)->getRect().top + m_Walls.at(m_Walls.size() - 1)->getRect().height));
				}
				if (m_CurrentMap.getMapData().at(i).at(j + 1) != 'W' && j < m_CurrentMap.getMapData().size())
				{
					//Right Edge
					m_EdgesX.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left + m_Walls.at(m_Walls.size() - 1)->getRect().width, m_Walls.at(m_Walls.size() - 1)->getRect().top));
					m_EdgesX.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left + m_Walls.at(m_Walls.size() - 1)->getRect().width, m_Walls.at(m_Walls.size() - 1)->getRect().top + m_Walls.at(m_Walls.size() - 1)->getRect().height));
				}
			}
		}
	}

	/*
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
	}*/

	//std::vector<sf::Vector2f> m_EdgesX; //Holds the wall edges
	//std::vector<sf::Vector2f> m_EdgesY; //Holds the wall edges
	/*
	for (int i = 0; i < m_Walls.size(); i++)
	{
		//Left Edge
		m_EdgesX.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top));
		m_EdgesX.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));

		//Right Edge
		m_EdgesX.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top));
		m_EdgesX.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));
	}

	for (int i = 0; i < m_Walls.size(); i++)
	{
		//Top Edge
		m_EdgesY.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top));
		m_EdgesY.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top));

		//Bottom Edge
		m_EdgesY.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));
		m_EdgesY.push_back(sf::Vector2f(m_Walls.at(i)->getRect().left + m_Walls.at(i)->getRect().width, m_Walls.at(i)->getRect().top + m_Walls.at(i)->getRect().height));
	}*/


	std::vector<float> XValues;
	std::vector<float> YValues;
	float currentX;
	float currentY;
	std::vector<std::vector<sf::Vector2f>> ComparissonVectsX;
	std::vector<std::vector<sf::Vector2f>> ComparissonVectsY;
	bool bFoundMatch = false;

	//For every edge
	for (int i = 0; i < m_EdgesX.size(); i++)
	{
		bFoundMatch = false;
		if (XValues.size() == 0) //If there are no current x values add one
		{
			XValues.push_back(m_EdgesX.at(i).x);
		}
		else
		{
			for (int j = 0; j < XValues.size(); j++) //For every current x value found
			{
				if (m_EdgesX.at(i).x == XValues.at(j)) //If the x matches to the current Xs
				{
					//A match has been found
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the xs match add the x to the list of xs
			{
				XValues.push_back(m_EdgesX.at(i).x);
			}
		}
	}

	for (int i = 0; i < m_EdgesY.size(); i++)
	{
		bFoundMatch = false;
		if (YValues.size() == 0) //If there are no current x values add one
		{
			YValues.push_back(m_EdgesY.at(i).y);
		}
		else
		{
			for (int j = 0; j < YValues.size(); j++) //For every current x value found
			{
				if (m_EdgesY.at(i).y == YValues.at(j)) //If the x matches to the current Xs
				{
					//A match has been found
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the xs match add the x to the list of xs
			{
				YValues.push_back(m_EdgesY.at(i).y);
			}
		}
	}
	ComparissonVectsX.resize(XValues.size());
	bool inVect;
	for (int i = 0; i < XValues.size(); i++)
	{
		for (int j = 0; j < m_EdgesX.size(); j++)
		{
			if (m_EdgesX.at(j).x == XValues.at(i)) //If the x matches to the current Xs
			{
				ComparissonVectsX.at(i).push_back(m_EdgesX.at(j));
			}
		}
	}

	ComparissonVectsY.resize(YValues.size());
	for (int i = 0; i < YValues.size(); i++)
	{
		for (int j = 0; j < m_EdgesY.size(); j++)
		{
			if (m_EdgesY.at(j).y == YValues.at(i)) //If the x matches to the current Xs
			{
				ComparissonVectsY.at(i).push_back(m_EdgesY.at(j));
			}
		}
	}

	bool currentlyEqual = true;
	int j = 0;
	int begin = 0;
	int end = 0;
	std::vector<sf::Vector2f> Edges;

	
	for (int i = 0; i < ComparissonVectsX.size(); i++)
	{
		begin = 0;
		end = 0;
		for (int j = 0; j < ComparissonVectsX.at(i).size(); j++)
		{
			while (currentlyEqual)
			{
				if (j + 2 < ComparissonVectsX.at(i).size())
				{
					if (ComparissonVectsX.at(i).at(j + 1).y == ComparissonVectsX.at(i).at(j + 2).y)
					{
						j+=2;
					}
					else
					{
						j++;
						end = j;
						currentlyEqual = false;
					}
				}
				else if (j + 2 == ComparissonVectsX.at(i).size())
				{
					end = j+ 1;
					currentlyEqual = false;
				}
				else
				{
					end = j;
					currentlyEqual = false;
				}
			}
			if (j < ComparissonVectsX.at(i).size() - 1)
			{
				Edges.push_back(ComparissonVectsX.at(i).at(begin));
				Edges.push_back(ComparissonVectsX.at(i).at(end));
				begin = j + 1;
			}
			//j++;
			currentlyEqual = true;
		}
		
	}

	
	for (int i = 0; i < ComparissonVectsY.size(); i++)
	{
		begin = 0;
		end = 0;
		for (int j = 0; j < ComparissonVectsY.at(i).size(); j++)
		{
			while (currentlyEqual)
			{
				if (j + 2 < ComparissonVectsY.at(i).size())
				{
					if (ComparissonVectsY.at(i).at(j + 1).x == ComparissonVectsY.at(i).at(j + 2).x)
					{
						j += 2;
					}
					else
					{
						j++;
						end = j;
						currentlyEqual = false;
					}
				}
				else if (j + 2 == ComparissonVectsY.at(i).size())
				{
					end = j + 1;
					currentlyEqual = false;
				}
				else
				{
					end = j;
					currentlyEqual = false;
				}
			}
			if (j < ComparissonVectsY.at(i).size() - 1)
			{
				Edges.push_back(ComparissonVectsY.at(i).at(begin));
				Edges.push_back(ComparissonVectsY.at(i).at(end));
				begin = j + 1;
			}
			//j++;
			currentlyEqual = true;
		}

	}

	m_Edges = Edges;
	m_EdgeLines.resize(m_Edges.size());
	m_EdgeLines.setPrimitiveType(sf::Lines);

	for (int i = 0; i < m_Edges.size(); i++)
	{
		m_EdgeLines[i] = sf::Vertex(m_Edges.at(i), sf::Color(255,0,0,255));
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
		m_Characters.at(i)->visionCalculation(m_Edges);
	}

	for (int i = 0; i < m_Units.size(); i++)
	{
		if (m_Enemies.at(0)->lazerChecks({ m_Units.at(i)->getCollisionLine(m_Enemies.at(i)->getRotation()).at(0),
			m_Units.at(i)->getCollisionLine(m_Enemies.at(i)->getRotation()).at(1) }))
		{
			m_Enemies.at(0)->setTarget(m_Units.at(i));
			m_Units.at(i)->setHealth(m_Units.at(i)->getHealth() - m_Enemies.at(0)->shoot());
		}
		else
		{
			m_Enemies.at(0)->setTarget(NULL);
		}
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
	for (int i = 0; i < 1; i++)
	{
		target.draw(*m_Characters.at(i)); //Draws the characters
	}
	target.draw(m_EdgeLines);
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