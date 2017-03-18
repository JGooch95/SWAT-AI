#include "../include/Game.h"

Game::Game(sf::Vector2u windowSize)
{
	//Sets up the map
	sf::Vector2f GridDimensions = sf::Vector2f(20, 20);

	m_CurrentMap.setup(
		sf::FloatRect(sf::Vector2f(0, 50), sf::Vector2f(windowSize) - sf::Vector2f(400,50)),
		GridDimensions
	);

	toolbar.setPosition(0, 0);
	toolbar.setFillColor(sf::Color(70,70, 70,255));
	toolbar.setSize(sf::Vector2f(windowSize.x, 50));

	m_CurrentMap.load("./Assets/Maps/1.txt"); 

	//Loads the main textures into the software
	m_Textures.loadTextures(std::vector<std::string>{
		"Assets/Sprites/Unit.png",
		"Assets/Sprites/Wood.jpg",
		"Assets/Sprites/Sniper.png",
		"Assets/Sprites/Enemy.png"
	});

	currentFont.loadFromFile("Assets/Fonts/arial.ttf");

	std::vector<sf::Vector2f> m_EdgesX; //Holds the wall edges
	std::vector<sf::Vector2f> m_EdgesY; //Holds the wall edges

	//Adds borders of the window to the edge checks
	m_EdgesY.push_back(sf::Vector2f(m_CurrentMap.getPosition().x, m_CurrentMap.getPosition().y));
	m_EdgesY.push_back(sf::Vector2f(m_CurrentMap.getPosition().x + m_CurrentMap.getWindowSize().x, m_CurrentMap.getPosition().y));

	m_EdgesY.push_back(sf::Vector2f(m_CurrentMap.getPosition().x, m_CurrentMap.getPosition().y + m_CurrentMap.getWindowSize().y));
	m_EdgesY.push_back(sf::Vector2f(m_CurrentMap.getPosition().x + m_CurrentMap.getWindowSize().x, m_CurrentMap.getPosition().y + m_CurrentMap.getWindowSize().y));

	m_EdgesX.push_back(sf::Vector2f(m_CurrentMap.getPosition().x, m_CurrentMap.getPosition().y));
	m_EdgesX.push_back(sf::Vector2f(m_CurrentMap.getPosition().x, m_CurrentMap.getPosition().y + m_CurrentMap.getWindowSize().y));

	m_EdgesX.push_back(sf::Vector2f(m_CurrentMap.getPosition().x + m_CurrentMap.getWindowSize().x, m_CurrentMap.getPosition().y));
	m_EdgesX.push_back(sf::Vector2f(m_CurrentMap.getPosition().x + m_CurrentMap.getWindowSize().x, m_CurrentMap.getPosition().y + m_CurrentMap.getWindowSize().y));

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
				m_Walls.at(m_Walls.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f(j * m_CurrentMap.getTileSize().x, i * m_CurrentMap.getTileSize().y));
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
				if (m_CurrentMap.getMapData().at(i).at(j + 1) != 'W' && j < m_CurrentMap.getMapData().at(i).size())
				{
					//Right Edge
					m_EdgesX.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left + m_Walls.at(m_Walls.size() - 1)->getRect().width, m_Walls.at(m_Walls.size() - 1)->getRect().top));
					m_EdgesX.push_back(sf::Vector2f(m_Walls.at(m_Walls.size() - 1)->getRect().left + m_Walls.at(m_Walls.size() - 1)->getRect().width, m_Walls.at(m_Walls.size() - 1)->getRect().top + m_Walls.at(m_Walls.size() - 1)->getRect().height));
				}
			}
			if (m_CurrentMap.getMapData().at(i).at(j) == 'E')
			{
				m_Enemies.push_back(new Character);
				m_Enemies.at(m_Enemies.size()-1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f((j * m_CurrentMap.getTileSize().x) + (m_CurrentMap.getTileSize().x /2), (i * m_CurrentMap.getTileSize().y) + (m_CurrentMap.getTileSize().y / 2)));
				m_Enemies.at(m_Enemies.size()-1)->setTexture(m_Textures.getTexture(3)); //Sets the unit texture
				m_Enemies.at(m_Enemies.size()-1)->setGunTexture(m_Textures.getTexture(2)); //Sets the unit texture
				m_Enemies.at(m_Enemies.size()-1)->linkMap(&m_CurrentMap); //Sets up the grid values for units
				m_Characters.push_back(m_Enemies.at(m_Enemies.size() - 1));
			}
			if (m_CurrentMap.getMapData().at(i).at(j) == 'P')
			{
				m_Units.push_back(new Character);
				m_Units.at(m_Units.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f((j * m_CurrentMap.getTileSize().x) + (m_CurrentMap.getTileSize().x / 2), (i * m_CurrentMap.getTileSize().y) + (m_CurrentMap.getTileSize().y / 2)));
				m_Units.at(m_Units.size() - 1)->setTexture(m_Textures.getTexture(0)); //Sets the unit texture
				m_Units.at(m_Units.size() - 1)->setGunTexture(m_Textures.getTexture(2)); //Sets the unit texture
				m_Units.at(m_Units.size() - 1)->linkMap(&m_CurrentMap); //Sets up the grid values for units
				m_Units.at(m_Units.size() - 1)->setVision(true);
				m_Characters.push_back(m_Units.at(m_Units.size() - 1));
			}
			
			if (m_CurrentMap.getMapData().at(i).at(j) == 'D')
			{
				m_Doors.push_back(new Entrance); 
				m_Doors.at(m_Doors.size() - 1)->setTexture(m_Textures.getTexture(1));
				m_Doors.at(m_Doors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x, m_CurrentMap.getTileSize().y/4));

				if ((m_CurrentMap.getMapData().at(i).at(j - 1) == 'W') && j > 0)
				{
					m_Doors.at(m_Doors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x, m_CurrentMap.getTileSize().y / 4));
					m_Doors.at(m_Doors.size() - 1)->setOrigin(sf::Vector2f(0, (m_Doors.at(m_Doors.size() - 1)->getRect().height / 2)));
					m_Doors.at(m_Doors.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f((j * m_CurrentMap.getTileSize().x), (i * m_CurrentMap.getTileSize().y) + (m_CurrentMap.getTileSize().y / 2)));
				}
				else if ((m_CurrentMap.getMapData().at(i).at(j + 1) == 'W') && j < m_CurrentMap.getMapData().at(i).size())
				{
					m_Doors.at(m_Doors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x, m_CurrentMap.getTileSize().y / 4));
					m_Doors.at(m_Doors.size() - 1)->setOrigin(sf::Vector2f(0, (m_Doors.at(m_Doors.size() - 1)->getRect().height / 2)));
					m_Doors.at(m_Doors.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f(((j+1) * m_CurrentMap.getTileSize().x), (i * m_CurrentMap.getTileSize().y) + (m_CurrentMap.getTileSize().y / 2)));
					m_Doors.at(m_Doors.size() - 1)->setOrientation(180);
					m_Doors.at(m_Doors.size() - 1)->setDirection(-1);
				}
				else if ((m_CurrentMap.getMapData().at(i - 1).at(j) == 'W') && i > 0)
				{
					m_Doors.at(m_Doors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x / 4, m_CurrentMap.getTileSize().y));
					m_Doors.at(m_Doors.size() - 1)->setOrigin(sf::Vector2f(m_Doors.at(m_Doors.size() - 1)->getRect().width / 2, 0));
					m_Doors.at(m_Doors.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f((j * m_CurrentMap.getTileSize().x) + (m_CurrentMap.getTileSize().x / 2), (i * m_CurrentMap.getTileSize().y)));
				}
				else if ((m_CurrentMap.getMapData().at(i + 1).at(j) == 'W') && i < m_CurrentMap.getMapData().size())
				{
					m_Doors.at(m_Doors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x / 4, m_CurrentMap.getTileSize().y));
					m_Doors.at(m_Doors.size() - 1)->setOrigin(sf::Vector2f(m_Doors.at(m_Doors.size() - 1)->getRect().width / 2, 0));
					m_Doors.at(m_Doors.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f((j * m_CurrentMap.getTileSize().x) + (m_CurrentMap.getTileSize().x / 2), ((i +1) * m_CurrentMap.getTileSize().y)));
					m_Doors.at(m_Doors.size() - 1)->setOrientation(180);
					m_Doors.at(m_Doors.size() - 1)->setDirection(-1);
				}
				else
				{
					m_Doors.at(m_Doors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap.getTileSize().x / 4, m_CurrentMap.getTileSize().y));
					m_Doors.at(m_Doors.size() - 1)->setOrigin(sf::Vector2f(m_Doors.at(m_Doors.size() - 1)->getRect().width / 2, 0));
					m_Doors.at(m_Doors.size() - 1)->setPosition(m_CurrentMap.getPosition() + sf::Vector2f((j * m_CurrentMap.getTileSize().x) + (m_CurrentMap.getTileSize().x / 2), ((i + 1) * m_CurrentMap.getTileSize().y)));
				}
				m_Doors.at(m_Doors.size() - 1)->setTile(sf::Vector2u(j, i));
				m_Doors.at(m_Doors.size() - 1)->setOpen(false);
				m_Doors.at(m_Doors.size() - 1)->linkMap(&m_CurrentMap);
			}
		}
	}

	for (int i = 0; i < m_Units.size(); i++)
	{
		unitScreen.push_back(new sf::RectangleShape);
		sf::Vector2f newSize(windowSize.x - m_CurrentMap.getWindowSize().x, ((windowSize.y - toolbar.getSize().y - (5* (m_Units.size()-1))) / m_Units.size()));
		if (newSize.y > windowSize.y / 4)
		{
			newSize.y = windowSize.y / 4;
		}
		unitScreen.at(unitScreen.size() - 1)->setSize(newSize);
		unitScreen.at(unitScreen.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap.getPosition().x + m_CurrentMap.getWindowSize().x, (toolbar.getSize().y + (i * unitScreen.at(unitScreen.size() - 1)->getSize().y)) + (i*5)));
		unitScreen.at(unitScreen.size() - 1)->setFillColor(sf::Color(120,120, 120,255));
		
		UIBars.push_back(new Bar);
		UIBars.at(UIBars.size() - 1)->setSize(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getSize().x - (unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f),
														   unitScreen.at(unitScreen.size() - 1)->getSize().y/10));
		UIBars.at(UIBars.size() - 1)->setPosition(unitScreen.at(unitScreen.size() - 1)->getPosition() + ((unitScreen.at(unitScreen.size() - 1)->getSize() / 5.0f)/2.0f));
		UIBars.at(UIBars.size() - 1)->setLevelColor(sf::Color(0,255,0,255));
		UIBars.at(UIBars.size() - 1)->setBarColor(sf::Color(255, 0, 0, 255));

		UIText.push_back(new sf::Text(std::to_string((int)m_Units.at(i)->getHealthData().x) + "/" + std::to_string((int)m_Units.at(i)->getHealthData().y), currentFont, 20));
		UIText.at(UIText.size() - 1)->setPosition(UIBars.at(UIBars.size() - 1)->getPosition() + sf::Vector2f((UIBars.at(UIBars.size() - 1)->getSize().x / 2) - (UIText.at(UIText.size() - 1)->getLocalBounds().width / 2), 0));
		UIText.at(UIText.size() - 1)->setFillColor(sf::Color(0, 0, 0, 255));

		UIBars.push_back(new Bar);
		UIBars.at(UIBars.size() - 1)->setSize(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getSize().x - (unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f),
			unitScreen.at(unitScreen.size() - 1)->getSize().y / 10));
		UIBars.at(UIBars.size() - 1)->setPosition(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getPosition().x + ((unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f) / 2.0f),
															   unitScreen.at(unitScreen.size() - 1)->getPosition().y + (2*(unitScreen.at(unitScreen.size() - 1)->getSize().y / 5.0f))));
		UIBars.at(UIBars.size() - 1)->setLevelColor(sf::Color(255, 255, 0, 255));
		UIBars.at(UIBars.size() - 1)->setBarColor(sf::Color(255, 0, 0, 255));
		UIBars.at(UIBars.size() - 1)->setLimit(m_Units.at(i)->getAmmoData().y);

		UIText.push_back(new sf::Text(std::to_string((int)m_Units.at(i)->getAmmoData().x) + "/" + std::to_string((int)m_Units.at(i)->getAmmoData().y), currentFont, 20));
		UIText.at(UIText.size() - 1)->setPosition(UIBars.at(UIBars.size() - 1)->getPosition() + sf::Vector2f((UIBars.at(UIBars.size() - 1)->getSize().x / 2) - (UIText.at(UIText.size() - 1)->getLocalBounds().width/2), 0));
		UIText.at(UIText.size() - 1)->setFillColor(sf::Color(0,0,0,255));

	}
	std::vector<float> XValues;
	std::vector<float> YValues;
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
	//Calculating Door edges for lazers
	std::vector<sf::Vector2f> Edges; 
	for (int j = 0; j < m_Doors.size(); j++)
	{
		std::vector<sf::Vector2f> tempEdges = m_Doors.at(j)->getEdges();
		Edges.insert(std::end(Edges), std::begin(tempEdges), std::end(tempEdges));
	}
	Edges.insert(std::end(Edges), std::begin(m_Edges), std::end(m_Edges));

	//Perform character checks
	for (int i = 0; i < m_Characters.size(); i++)
	{
		m_Characters.at(i)->update();
		m_Characters.at(i)->lazerChecks(Edges);
		m_Characters.at(i)->visionCalculation(Edges);
	}

	for (int i = 0; i < m_Units.size(); i++)
	{
		UIBars.at(i*2)->setLevel(m_Units.at(i)->getHealthData().x);
		UIBars.at((i*2)+1)->setLevel(m_Units.at(i)->getAmmoData().x);

		UIText.at(i * 2)->setString(std::to_string((int)m_Units.at(i)->getHealthData().x) + "/" + std::to_string((int)m_Units.at(i)->getHealthData().y));
		UIText.at((i * 2) + 1)->setString(std::to_string((int)m_Units.at(i)->getAmmoData().x) + "/" + std::to_string((int)m_Units.at(i)->getAmmoData().y));

		UIText.at(i * 2)->setPosition(UIBars.at(i * 2)->getPosition() + sf::Vector2f((UIBars.at(i * 2)->getSize().x / 2) - (UIText.at(i * 2)->getLocalBounds().width / 2), 0));
		UIText.at((i * 2)+1)->setPosition(UIBars.at((i * 2) + 1)->getPosition() + sf::Vector2f((UIBars.at((i * 2) + 1)->getSize().x / 2) - (UIText.at((i * 2) + 1)->getLocalBounds().width / 2), 0));
	}
	//Perform checks between characters
	characterInteractions(m_Units, m_Enemies);
	characterInteractions(m_Enemies, m_Units);

	//Door checks
	for (int i = 0; i < m_Doors.size(); i++)
	{
		bool OpenDoor = false;
		for (int j = 0; j < m_Characters.size(); j++)
		{
			sf::Vector2u CharacterTile(((int)m_Characters.at(j)->getPosition().x - m_CurrentMap.getPosition().x) / (int)m_CurrentMap.getTileSize().x,
									   ((int)m_Characters.at(j)->getPosition().y - m_CurrentMap.getPosition().y) / (int)m_CurrentMap.getTileSize().y);

			if (CharacterTile == sf::Vector2u(m_Doors.at(i)->getTile().x + 1, m_Doors.at(i)->getTile().y) ||
				CharacterTile == sf::Vector2u(m_Doors.at(i)->getTile().x - 1, m_Doors.at(i)->getTile().y) ||
				CharacterTile == sf::Vector2u(m_Doors.at(i)->getTile().x, m_Doors.at(i)->getTile().y + 1) ||
				CharacterTile == sf::Vector2u(m_Doors.at(i)->getTile().x, m_Doors.at(i)->getTile().y - 1) ||
				CharacterTile == m_Doors.at(i)->getTile())
			{
				OpenDoor = true;
			}
		}
		m_Doors.at(i)->setOpen(OpenDoor);
	}
}

void Game::characterInteractions(std::vector<Character*> Chars1, std::vector<Character*>  Chars2)
{
	bool bSameCharacters = (Chars1 == Chars2); //Holds whether the character containers are the same
	bool bSeenCharacter = false; //Holds whether a characters been seen by the current characters

	//For every character in the first container
	for (int i = 0; i < Chars1.size(); i++)
	{
		bSeenCharacter = false;
		//Check against every character in the second container
		for (int j = 0; j < Chars2.size(); j++)
		{
			//Dont allow checks on itself
			if (i != j || !bSameCharacters)
			{
				//Check lazer collision
				if (Chars1.at(i)->lazerChecks({ Chars2.at(j)->getCollisionLine(Chars1.at(i)->getRotation()).at(0),
												Chars2.at(j)->getCollisionLine(Chars1.at(i)->getRotation()).at(1)}))
				{
					bSeenCharacter = true;
					//Shoot if the character has health
					if (Chars2.at(j)->getHealthData().x > 0)
					{
						Chars1.at(i)->setTarget(Chars2.at(j));
						Chars2.at(j)->setHealth(Chars2.at(j)->getHealthData().x - Chars1.at(i)->shoot());
					}
				}
			}
		}
		//If no characters have been seen then remove targets
		if (bSeenCharacter == false)
		{
			Chars1.at(i)->setTarget(NULL);
		}
	}
}
void Game::clickRight(sf::Vector2i mousePos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	if (m_Units.size() > 0)
	{
		m_Units.at(0)->setPath(m_Pathfinder.findPath(m_Units.at(0)->getPosition(), (sf::Vector2f)mousePos)); //Sets a path towards the clicked area
	}
}

void Game::clickLeft(sf::Vector2i mousePos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	if (m_Enemies.size() > 0)
	{
		m_Enemies.at(0)->setPath(m_Pathfinder.findPath(m_Enemies.at(0)->getPosition(), (sf::Vector2f)mousePos)); //Sets a path towards the clicked area
	}
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_CurrentMap); //Draws the Grid

	for (int i = 0; i < m_Walls.size(); i++)
	{
		target.draw(*m_Walls.at(i)); //Draws the units
	}
	for (int i = 0; i < m_Doors.size(); i++)
	{
		target.draw(*m_Doors.at(i)); //Draws the units
	}
	for (int i = 0; i < m_Characters.size(); i++)
	{
		target.draw(*m_Characters.at(i)); //Draws the characters
	}
	target.draw(m_EdgeLines);
	for (int i = 0; i < unitScreen.size(); i++)
	{
		target.draw(*unitScreen.at(i));
	}
	for (int i = 0; i < UIBars.size(); i++)
	{
		target.draw(*UIBars.at(i));
	}
	for (int i = 0; i < UIText.size(); i++)
	{
		target.draw(*UIText.at(i));
	}
	target.draw(toolbar);
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
	for (int i = 0; i < m_Doors.size(); i++)
	{
		delete(m_Doors.at(i));
		m_Doors.at(i) = NULL;
	}
}