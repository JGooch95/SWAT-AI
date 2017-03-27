#include "../include/Game.h"

Game::Game(sf::Vector2u windowSize)
{
	//Sets up the toolbar 
	m_Toolbar.setPosition(0, 0);
	m_Toolbar.setFillColor(sf::Color(70,70, 70,255));
	m_Toolbar.setSize(sf::Vector2f(windowSize.x, windowSize.y / 20));

	//Sets up the default map
	m_CurrentMap = Map::getInstance();
	m_CurrentSettings = Settings::getInstance();
	m_Textures = TextureLoader::getInstance();

	m_CurrentMap->setup(
		sf::FloatRect(sf::Vector2f(0, m_Toolbar.getSize().y), sf::Vector2f(windowSize) - sf::Vector2f(windowSize.x/3, m_Toolbar.getSize().y)),
		sf::Vector2f(20, 20)
	);
	m_CurrentMap->load("./Assets/Maps/CustomMap.txt"); //Loads a map from a file
	m_CurrentMap->loadFloor("./Assets/Maps/CustomMapFloor.txt");

	m_CurrentFont.loadFromFile("Assets/Fonts/arial.ttf"); //Loads the main font

	for (int i = 0; i < m_CurrentMap->getFloorData().size(); i++)
	{
		for (int j = 0; j < m_CurrentMap->getFloorData().at(i).size(); j++)
		{
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'G')
			{
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size()-1)->setTexture(m_Textures->getTexture(14));
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));
			}
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'F')
			{
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(25));
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));
			}
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'C')
			{
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(26));
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));
			}
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'K')
			{
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(27));
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));
			}
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'B')
			{
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(28));
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));
			}
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'R')
			{
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(29));
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));
			}
		}
	}

	//Holds the edges of the walls to reduce the amount
	std::vector<sf::Vector2f> vEdgesX; 
	std::vector<sf::Vector2f> vEdgesY; 

	//Adds borders of the window to the edge checks
	vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y));
	vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y));

	vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));
	vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));

	vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y));
	vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));

	vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y));
	vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));

	//For every bit within the map Check the object type location and create it
	for (int i = 0; i < m_CurrentMap->getMapData().size(); i++)
	{
		for (int j = 0; j < m_CurrentMap->getMapData().at(i).size(); j++)
		{
			//Walls
			if (m_CurrentMap->getMapData().at(i).at(j) == 'W' || m_CurrentMap->getMapData().at(i).at(j) == 'B')
			{
				m_vWalls.push_back(new Object);
				if (m_CurrentMap->getMapData().at(i).at(j) == 'W')
				{
					m_vWalls.at(m_vWalls.size() - 1)->setTexture(m_Textures->getTexture(0)); //Sets the unit texture
				}
				if (m_CurrentMap->getMapData().at(i).at(j) == 'B')
				{
					m_vWalls.at(m_vWalls.size() - 1)->setTexture(m_Textures->getTexture(30));
				}
				m_vWalls.at(m_vWalls.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y));
				m_vWalls.at(m_vWalls.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));

				if (i > 0)
				{
					if (m_CurrentMap->getMapData().at(i - 1).at(j) != 'W' && m_CurrentMap->getMapData().at(i - 1).at(j) != 'B')
					{
						//Top Edge
						vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
						vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					}
				}
				if (i < m_CurrentMap->getMapData().size() - 1)
				{
					if (m_CurrentMap->getMapData().at(i + 1).at(j) != 'W' && m_CurrentMap->getMapData().at(i + 1).at(j) != 'B')
					{
						//Bottom Edge
						vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
						vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
					}
				}
				
				if (j > 0)
				{
					if (m_CurrentMap->getMapData().at(i).at(j - 1) != 'W' && m_CurrentMap->getMapData().at(i).at(j - 1) != 'B')
					{
						//Left Edge
						vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
						vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
					}
				}
				if (j < m_CurrentMap->getMapData().at(i).size() - 1)
				{
					if (m_CurrentMap->getMapData().at(i).at(j + 1) != 'W' && m_CurrentMap->getMapData().at(i).at(j + 1) != 'B')
					{
						//Right Edge
						vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
						vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
					}
				}
			}

			//Characters
			if (m_CurrentMap->getMapData().at(i).at(j) == 'E' || m_CurrentMap->getMapData().at(i).at(j) == 'P')
			{
				//Enemies specifics
				if (m_CurrentMap->getMapData().at(i).at(j) == 'E')
				{
					m_vEnemies.push_back(new Character);
					m_vEnemies.at(m_vEnemies.size() - 1)->setTexture(m_Textures->getTexture(2)); //Sets the unit texture
					m_vEnemies.at(m_vEnemies.size() - 1)->setClass(Assault, m_Textures->getTexture(7));
					m_vCharacters.push_back(m_vEnemies.at(m_vEnemies.size() - 1));
				}

				//Friendly units specifics
				else if (m_CurrentMap->getMapData().at(i).at(j) == 'P')
				{
					m_vUnits.push_back(new Character);
					m_vUnits.at(m_vUnits.size() - 1)->setTexture(m_Textures->getTexture(1)); //Sets the unit texture
					m_vUnits.at(m_vUnits.size() - 1)->setClass(Sniper, m_Textures->getTexture(4));
					m_vUnits.at(m_vUnits.size() - 1)->setVision(true);
					m_vCharacters.push_back(m_vUnits.at(m_vUnits.size() - 1));
				}

				//Sets generic character data
				m_vCharacters.at(m_vCharacters.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setSize(sf::Vector2f(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y), std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setOrigin(sf::Vector2f(m_vCharacters.at(m_vCharacters.size() - 1)->getRect().width/2.0f, (m_vCharacters.at(m_vCharacters.size() - 1)->getRect().height / 2)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setMuzzle(m_Textures->getTexture(11));
				m_vCharacters.at(m_vCharacters.size() - 1)->setupTextures();
			}

			//Doors
			if (m_CurrentMap->getMapData().at(i).at(j) == 'D')
			{
				m_vDoors.push_back(new Entrance); 
				m_vDoors.at(m_vDoors.size() - 1)->setTexture(m_Textures->getTexture(0));

				bool bNearWall = false;
				if (j > 0)
				{
					if ((m_CurrentMap->getMapData().at(i).at(j - 1) == 'W') || m_CurrentMap->getMapData().at(i).at(j - 1) == 'B')
					{
						bNearWall = true;
						m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y / 4));
						m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(0, (m_vDoors.at(m_vDoors.size() - 1)->getSize().y / 2.0f)));
						m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
					}
				}
				if (j < m_CurrentMap->getMapData().at(i).size() -1)
				{
					if ((m_CurrentMap->getMapData().at(i).at(j + 1) == 'W') || m_CurrentMap->getMapData().at(i).at(j + 1) == 'B')
					{
						bNearWall = true;
						m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y / 4));
						m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(0, (m_vDoors.at(m_vDoors.size() - 1)->getSize().y / 2.0f)));
						m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f(((j + 1) * m_CurrentMap->getTileSize().x), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
						m_vDoors.at(m_vDoors.size() - 1)->setOrientation(180);
						m_vDoors.at(m_vDoors.size() - 1)->setDirection(-1);
					}
				}
				if (i > 0)
				{
					if ((m_CurrentMap->getMapData().at(i - 1).at(j) == 'W') || m_CurrentMap->getMapData().at(i - 1).at(j) == 'B')
					{
						bNearWall = true;
						m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x / 4, m_CurrentMap->getTileSize().y));
						m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(m_vDoors.at(m_vDoors.size() - 1)->getSize().x / 2.0f, 0));
						m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), (i * m_CurrentMap->getTileSize().y)));
					}
				}
				if (i < m_CurrentMap->getMapData().size()-1)
				{
					if ((m_CurrentMap->getMapData().at(i + 1).at(j) == 'W') || m_CurrentMap->getMapData().at(i + 1).at(j) == 'B')
					{
						bNearWall = true;
						m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x / 4, m_CurrentMap->getTileSize().y));
						m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(m_vDoors.at(m_vDoors.size() - 1)->getSize().x / 2.0f, 0));
						m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), ((i + 1) * m_CurrentMap->getTileSize().y)));
						m_vDoors.at(m_vDoors.size() - 1)->setOrientation(180);
						m_vDoors.at(m_vDoors.size() - 1)->setDirection(-1);
					}
				}

				if(!bNearWall)
				{
					m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x / 4, m_CurrentMap->getTileSize().y));
					m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(m_vDoors.at(m_vDoors.size() - 1)->getSize().x / 2.0f, 0));
					m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), ((i + 1) * m_CurrentMap->getTileSize().y)));
				}
				m_vDoors.at(m_vDoors.size() - 1)->setTile(sf::Vector2u(j, i));
				m_vDoors.at(m_vDoors.size() - 1)->setOpen(false);
			}
		}
	}

	sf::Vector2f newSize(windowSize.x - m_CurrentMap->getWindowSize().x, ((windowSize.y - m_Toolbar.getSize().y - (5 * (m_vUnits.size() - 1))) / m_vUnits.size()));
	if (newSize.y > windowSize.y / 4)
	{
		newSize.y = windowSize.y / 4;
	}

	for (int i = 0; i < m_vUnits.size(); i++)
	{
		m_UnitUI.push_back(new HUDWindow);
		m_UnitUI.at(m_UnitUI.size() - 1)->setSize(newSize);
		m_UnitUI.at(m_UnitUI.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, (m_Toolbar.getSize().y + (i * m_UnitUI.at(m_UnitUI.size() - 1)->getSize().y)) + (i * 5)));
		m_UnitUI.at(m_UnitUI.size() - 1)->setClassTexture(m_Textures->getTexture(3));
					
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarLevels(HealthBar, m_vUnits.at(i)->getHealthData());
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarIcon(HealthBar, m_Textures->getTexture(12));		
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarText(HealthBar, std::to_string((int)m_vUnits.at(i)->getHealthData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().upper));
					
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarLevels(AmmoBar, m_vUnits.at(i)->getAmmoData());
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarIcon(AmmoBar, m_Textures->getTexture(13));
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarText(AmmoBar, std::to_string((int)m_vUnits.at(i)->getAmmoData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().upper));
					
		m_UnitUI.at(m_UnitUI.size() - 1)->setFont(&m_CurrentFont);

		m_UnitUI.at(m_UnitUI.size() - 1)->setLoadoutTexture(0, m_Textures->getTexture(15));
		m_UnitUI.at(m_UnitUI.size() - 1)->setLoadoutTexture(1, m_Textures->getTexture(15));

		m_UnitUI.at(m_UnitUI.size() - 1)->scaleUI();
	}

	m_vEdges = edgeReduction(vEdgesX, vEdgesY);

	m_EdgeLines.resize(m_vEdges.size());
	m_EdgeLines.setPrimitiveType(sf::Lines);
	for (int i = 0; i < m_vEdges.size(); i++)
	{
		m_EdgeLines[i] = sf::Vertex(m_vEdges.at(i), sf::Color(255,0,0,255));
	}
	exitButton = new Button();
	exitButton->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
	exitButton->setPosition(sf::Vector2f(m_Toolbar.getPosition().x + m_Toolbar.getSize().x - exitButton->getSize().x, m_Toolbar.getPosition().y));
	exitButton->setTexture(m_Textures->getTexture(20));
	m_Pathfinder.setup();
}

void Game::update(sf::Vector2i mousePos)
{
	for (int i = 0; i < waves.size(); i++)
	{
		if (waves.at(i) != NULL)
		{
			waves.at(i)->update();
			if (waves.at(i)->isDone())
			{
				delete waves.at(i);
				waves.at(i) = NULL;
			}
		}
	}
	std::vector<sf::Vector2f> vEdgesToCheck; //Holds all of the edges to be checked this frame

	//Adds all of the door edges to the checked edges.
	for (int j = 0; j < m_vDoors.size(); j++)
	{
		std::vector<sf::Vector2f> tempEdges = m_vDoors.at(j)->getEdges();
		vEdgesToCheck.insert(std::end(vEdgesToCheck), std::begin(tempEdges), std::end(tempEdges));
	}
	vEdgesToCheck.insert(std::end(vEdgesToCheck), std::begin(m_vEdges), std::end(m_vEdges)); //Adds all of the wall edges to the end of the checked edges

	//Perform character checks
	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		m_vCharacters.at(i)->update(); //Update data and states
		m_vCharacters.at(i)->lazerChecks(vEdgesToCheck); //Perform checks for the aiming
		m_vCharacters.at(i)->visionCalculation(vEdgesToCheck); //Perform checks for the vision cone
		m_vCharacters.at(i)->bulletChecks(vEdgesToCheck); //Perform checks for any of the shot bullets

		if (m_vCharacters.at(i)->stepTaken())
		{
			sf::Vector2u CharacterTile(((int)m_vCharacters.at(i)->getPosition().x - m_CurrentMap->getPosition().x) / (int)m_CurrentMap->getTileSize().x,
			((int)m_vCharacters.at(i)->getPosition().y - m_CurrentMap->getPosition().y) / (int)m_CurrentMap->getTileSize().y);

			if (m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'B' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'R' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'G')
			{
				waves.push_back(new soundWave(40, 3.0f, 1.0f, m_vCharacters.at(i)->getPosition()));
			}
			else if (m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'C' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'K' ||
				m_CurrentMap->getFloorData().at(CharacterTile.y).at(CharacterTile.x) == 'F')
			{
				waves.push_back(new soundWave(80, 3.0f, 1.0f, m_vCharacters.at(i)->getPosition()));
			}
		}
	}

	//Perform checks between characters
	characterInteractions(m_vUnits, m_vEnemies);
	characterInteractions(m_vEnemies, m_vUnits);

	//Door checks
	for (int i = 0; i < m_vDoors.size(); i++)
	{
		//For every door check if a character is standing near it. If they are then open the door.
		bool bOpenDoor = false;
		for (int j = 0; j < m_vCharacters.size(); j++)
		{
			sf::Vector2u CharacterTile(((int)m_vCharacters.at(j)->getPosition().x - m_CurrentMap->getPosition().x) / (int)m_CurrentMap->getTileSize().x,
									   ((int)m_vCharacters.at(j)->getPosition().y - m_CurrentMap->getPosition().y) / (int)m_CurrentMap->getTileSize().y);

			if (CharacterTile == sf::Vector2u(m_vDoors.at(i)->getTile().x + 1, m_vDoors.at(i)->getTile().y) ||
				CharacterTile == sf::Vector2u(m_vDoors.at(i)->getTile().x - 1, m_vDoors.at(i)->getTile().y) ||
				CharacterTile == sf::Vector2u(m_vDoors.at(i)->getTile().x, m_vDoors.at(i)->getTile().y + 1) ||
				CharacterTile == sf::Vector2u(m_vDoors.at(i)->getTile().x, m_vDoors.at(i)->getTile().y - 1) ||
				CharacterTile == m_vDoors.at(i)->getTile())
			{
				bOpenDoor = true;
			}
		}
		m_vDoors.at(i)->setOpen(bOpenDoor);
	}

	//UI
	for (int i = 0; i < m_vUnits.size(); i++)
	{
		//Updates the UI bars
		//Health
		m_UnitUI.at(i)->setBarLevels(HealthBar, m_vUnits.at(i)->getHealthData());
		m_UnitUI.at(i)->setBarLevels(AmmoBar, m_vUnits.at(i)->getAmmoData());
		m_UnitUI.at(i)->setBarText(HealthBar, std::to_string((int)m_vUnits.at(i)->getHealthData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().upper));

		if (m_vUnits.at(i)->reloading())
		{
			m_UnitUI.at(i)->setBarText(AmmoBar, "Reloading");
		}
		else
		{
			m_UnitUI.at(i)->setBarText(AmmoBar, std::to_string((int)m_vUnits.at(i)->getAmmoData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().upper));
		}

		m_UnitUI.at(i)->getClassButton()->hovering(mousePos);
		for (int j = 0; j < m_vUnits.at(i)->getLoadoutSize(); j++)
		{
			m_UnitUI.at(i)->getLoadoutButton(j)->hovering(mousePos);
		}
	}

	exitButton->hovering(mousePos);
}

void Game::characterInteractions(std::vector<Character*> vCharSet1, std::vector<Character*>  vCharSet2)
{
	bool bSeenCharacter = false; //Holds whether a characters been seen by the current characters

	//For every character in the first container
	for (int i = 0; i < vCharSet1.size(); i++)
	{
		bSeenCharacter = false;
		//Check against every character in the second container
		for (int j = 0; j < vCharSet2.size(); j++)
		{
			//Dont allow checks on itself
			if (!(vCharSet1 == vCharSet2 && i == j))
			{
				//Check lazer collision
				if (vCharSet1.at(i)->lazerChecks({ vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(0),
												   vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(1)}))
				{
					//If a character is seem then set it to be a target
					bSeenCharacter = true;
					if (vCharSet2.at(j)->getHealthData().lower > 0)
					{
						vCharSet1.at(i)->setTarget(vCharSet2.at(j));
					}
				}
				
				if (vCharSet1.at(i)->isShooting())
				{
					//Take health off if a bullet has been shot
					vCharSet2.at(j)->setHealth(vCharSet2.at(j)->getHealthData().lower - vCharSet1.at(i)->bulletChecks({ vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(0),
						vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(1) }));

					waves.push_back(new soundWave(vCharSet1.at(i)->getWeapon()->getWeaponVolume(), 10.0f, 1.0f, vCharSet1.at(i)->getWeapon()->getWeaponEnd()));
					
				}
			}
		}

		//If no characters have been seen then set a Null target
		if (bSeenCharacter == false)
		{
			vCharSet1.at(i)->setTarget(NULL);
		}
	}
}

void Game::enableDebug()
{
	if (m_CurrentSettings->debugActive())
	{
		m_CurrentSettings->setDebug(false);
	}
	else
	{
		m_CurrentSettings->setDebug(true);
	}
}

int Game::clickLeft(sf::Vector2i mousePos)
{
	//Checks all of the UI Buttons
	for (int i = 0; i < m_UnitUI.size(); i++)
	{
		//If a Class button is pressed
		if (m_UnitUI.at(i)->getClassButton()->hovering(mousePos))
		{
			//Switches the class of the unit
			switch (m_vUnits.at(i)->getClass())
			{
				case Sniper:
					m_vUnits.at(i)->setClass(Support, m_Textures->getTexture(5));
					m_UnitUI.at(i)->setClassTexture(m_Textures->getTexture(8));
					break;
				case Support:
					m_vUnits.at(i)->setClass(Shotgunner, m_Textures->getTexture(6));
					m_UnitUI.at(i)->setClassTexture(m_Textures->getTexture(9));
					break;
				case Shotgunner:
					m_vUnits.at(i)->setClass(Assault, m_Textures->getTexture(7));
					m_UnitUI.at(i)->setClassTexture(m_Textures->getTexture(10));
					break;
				case Assault:
					m_vUnits.at(i)->setClass(Sniper, m_Textures->getTexture(4));
					m_UnitUI.at(i)->setClassTexture(m_Textures->getTexture(3));
					break;
			}
		}

		for (int j = 0; j < m_vUnits.at(i)->getLoadoutSize(); j++)
		{
			//If a loadout button
			if (m_UnitUI.at(i)->getLoadoutButton(j)->hovering(mousePos))
			{
				//Switches the class of the unit
				m_vUnits.at(i)->setLoadoutItem(j, m_vUnits.at(i)->getNextLoadoutItem(m_vUnits.at(i)->getLoadoutItem(j)));
					
				//Switches the class image once changed
				switch (m_vUnits.at(i)->getLoadoutItem(j))
				{
					case Lazer:
						m_UnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(16));
						break;

					case Silencer:
						m_UnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(17));
						break;

					case Scope:
						m_UnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(18));
						break;

					case None:
						m_UnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(15));
						break;
				}
				m_UnitUI.at(m_UnitUI.size() - 1)->scaleUI();
			}
		}
	}

	if (exitButton->hovering(mousePos))
	{
		return 1;
	}
	return 0;
}

void Game::clickRight(sf::Vector2i mousePos)
{
	m_Pathfinder.setupLists(); //Sets up the pathfinder for a new path
	if (m_vUnits.size() > 0)
	{
		m_vUnits.at(0)->setPath(m_Pathfinder.findPath(m_vUnits.at(0)->getPosition(), (sf::Vector2f)mousePos)); //Sets a path towards the clicked area
	}
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (int i = 0; i < m_Background.size(); i++)
	{
		if (m_Background.at(i) != NULL)
		{
			target.draw(*m_Background.at(i));
		}
	}

	if (m_CurrentSettings->debugActive())
	{
		target.draw(*m_CurrentMap); //Draws the Grid
	}

	for (int i = 0; i < waves.size(); i++)
	{
		if (waves.at(i) != NULL)
		{
			target.draw(*waves.at(i));
		}
	}

	//Draws the walls
	for (int i = 0; i < m_vWalls.size(); i++)
	{
		target.draw(*m_vWalls.at(i)); 
	}

	//Draws the doors
	for (int i = 0; i < m_vDoors.size(); i++)
	{
		target.draw(*m_vDoors.at(i));
	}

	//Draws the characters
	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		target.draw(*m_vCharacters.at(i));
	}

	if (m_CurrentSettings->debugActive())
	{
		//Draws the edges of the walls
		target.draw(m_EdgeLines);
	}

	for (int i = 0; i < m_UnitUI.size(); i++)
	{
		target.draw(*m_UnitUI.at(i));
	}

	//Draws the UI
	target.draw(m_Toolbar);
	target.draw(*exitButton);
}

std::vector<sf::Vector2f> Game::edgeReduction(std::vector<sf::Vector2f> vXEdges, std::vector<sf::Vector2f> vYEdges)
{
	//Reduces the amounts of edges for each wall by combining adjacent edges and removing unreachable edges.
	std::vector<float> vfXValues;
	std::vector<float> vfYValues;
	bool bFoundMatch = false;

	//For every edge
	for (int i = 0; i < vXEdges.size(); i++)
	{
		bFoundMatch = false;
		if (vfXValues.size() == 0) //If there are no current x values add one
		{
			vfXValues.push_back(vXEdges.at(i).x);
		}
		else
		{
			for (int j = 0; j < vfXValues.size(); j++) //For every current x value found
			{
				if (vXEdges.at(i).x == vfXValues.at(j)) //If the x matches to the current Xs
				{
					//A match has been found
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the xs match add the x to the list of xs
			{
				vfXValues.push_back(vXEdges.at(i).x);
			}
		}
	}

	for (int i = 0; i < vYEdges.size(); i++)
	{
		bFoundMatch = false;
		if (vfYValues.size() == 0) //If there are no current x values add one
		{
			vfYValues.push_back(vYEdges.at(i).y);
		}
		else
		{
			for (int j = 0; j < vfYValues.size(); j++) //For every current x value found
			{
				if (vYEdges.at(i).y == vfYValues.at(j)) //If the x matches to the current Xs
				{
					//A match has been found
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the xs match add the x to the list of xs
			{
				vfYValues.push_back(vYEdges.at(i).y);
			}
		}
	}

	std::vector<std::vector<sf::Vector2f>> vComparissonVectsX;
	std::vector<std::vector<sf::Vector2f>> vComparissonVectsY;

	vComparissonVectsX.resize(vfXValues.size());
	for (int i = 0; i < vfXValues.size(); i++)
	{
		for (int j = 0; j < vXEdges.size(); j++)
		{
			if (vXEdges.at(j).x == vfXValues.at(i)) //If the x matches to the current Xs
			{
				vComparissonVectsX.at(i).push_back(vXEdges.at(j));
			}
		}
	}

	vComparissonVectsY.resize(vfYValues.size());
	for (int i = 0; i < vfYValues.size(); i++)
	{
		for (int j = 0; j < vYEdges.size(); j++)
		{
			if (vYEdges.at(j).y == vfYValues.at(i)) //If the x matches to the current Xs
			{
				vComparissonVectsY.at(i).push_back(vYEdges.at(j));
			}
		}
	}

	bool currentlyEqual = true;
	int iBegin = 0;
	int iEnd = 0;
	std::vector<sf::Vector2f> vEdges;

	for (int i = 0; i < vComparissonVectsX.size(); i++)
	{
		iBegin = 0;
		iEnd = 0;
		for (int j = 0; j < vComparissonVectsX.at(i).size(); j++)
		{
			while (currentlyEqual)
			{
				if (j + 2 < vComparissonVectsX.at(i).size())
				{
					if (vComparissonVectsX.at(i).at(j + 1).y == vComparissonVectsX.at(i).at(j + 2).y)
					{
						j += 2;
					}
					else
					{
						j++;
						iEnd = j;
						currentlyEqual = false;
					}
				}
				else if (j + 2 == vComparissonVectsX.at(i).size())
				{
					iEnd = j + 1;
					currentlyEqual = false;
				}
				else
				{
					iEnd = j;
					currentlyEqual = false;
				}
			}
			if (j < vComparissonVectsX.at(i).size() - 1)
			{
				vEdges.push_back(vComparissonVectsX.at(i).at(iBegin));
				vEdges.push_back(vComparissonVectsX.at(i).at(iEnd));
				iBegin = j + 1;
			}
			currentlyEqual = true;
		}
	}


	for (int i = 0; i < vComparissonVectsY.size(); i++)
	{
		iBegin = 0;
		iEnd = 0;
		for (int j = 0; j < vComparissonVectsY.at(i).size(); j++)
		{
			while (currentlyEqual)
			{
				if (j + 2 < vComparissonVectsY.at(i).size())
				{
					if (vComparissonVectsY.at(i).at(j + 1).x == vComparissonVectsY.at(i).at(j + 2).x)
					{
						j += 2;
					}
					else
					{
						j++;
						iEnd = j;
						currentlyEqual = false;
					}
				}
				else if (j + 2 == vComparissonVectsY.at(i).size())
				{
					iEnd = j + 1;
					currentlyEqual = false;
				}
				else
				{
					iEnd = j;
					currentlyEqual = false;
				}
			}
			if (j < vComparissonVectsY.at(i).size() - 1)
			{
				vEdges.push_back(vComparissonVectsY.at(i).at(iBegin));
				vEdges.push_back(vComparissonVectsY.at(i).at(iEnd));
				iBegin = j + 1;
			}
			currentlyEqual = true;
		}
	}

	return vEdges;
}

Game::~Game()
{
	for (int i = 0; i < m_Background.size(); i++)
	{
		delete(m_Background.at(i));
		m_Background.at(i) = NULL;
	}

	//Cleans all pointers.
	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		delete(m_vCharacters.at(i));
		m_vCharacters.at(i) = NULL;
	}

	for (int i = 0; i < m_vWalls.size(); i++)
	{
		delete(m_vWalls.at(i));
		m_vWalls.at(i) = NULL;
	}

	for (int i = 0; i < m_vDoors.size(); i++)
	{
		delete(m_vDoors.at(i));
		m_vDoors.at(i) = NULL;
	}

	for (int i = 0; i < m_UnitUI.size(); i++)
	{
		delete(m_UnitUI.at(i));
		m_UnitUI.at(i) = NULL;
	}

	for (int i = 0; i < waves.size(); i++)
	{
		if (waves.at(i) != NULL)
		{
			delete waves.at(i);
			waves.at(i) = NULL;
		}
	}

	delete(exitButton);
	exitButton = NULL;
}