#include "../include/Game.h"

Game::Game(sf::Vector2u windowSize)
{
	//Sets up the toolbar 
	m_Toolbar.setPosition(0, 0);
	m_Toolbar.setFillColor(sf::Color(70,70, 70,255));
	m_Toolbar.setSize(sf::Vector2f(windowSize.x, windowSize.y / 20));

	//Sets up the default map
	m_CurrentMap->setup(
		sf::FloatRect(sf::Vector2f(0, m_Toolbar.getSize().y), sf::Vector2f(windowSize) - sf::Vector2f(windowSize.x/3, m_Toolbar.getSize().y)),
		sf::Vector2f(20, 20)
	);

	//Loads the map files
	m_CurrentMap->load(ObjectMap, "./Assets/Maps/CustomMap.txt"); 
	m_CurrentMap->load(FloorMap, "./Assets/Maps/CustomMapFloor.txt");

	//Setting up the floor tiles
	for (int i = 0; i < m_CurrentMap->getFloorData().size(); i++)
	{
		for (int j = 0; j < m_CurrentMap->getFloorData().at(i).size(); j++)
		{
			if (m_CurrentMap->getFloorData().at(i).at(j) == 'G' || m_CurrentMap->getFloorData().at(i).at(j) == 'F' || m_CurrentMap->getFloorData().at(i).at(j) == 'C' ||
				m_CurrentMap->getFloorData().at(i).at(j) == 'K' || m_CurrentMap->getFloorData().at(i).at(j) == 'B' || m_CurrentMap->getFloorData().at(i).at(j) == 'R')
			{
				//Sets up the tile
				m_Background.push_back(new Object);
				m_Background.at(m_Background.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));
				m_Background.at(m_Background.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y)));

				//Sets the floor texture
				switch (m_CurrentMap->getFloorData().at(i).at(j))
				{
					case 'G':
						m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(14));
						break;
					case 'F':
						m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(25));
						break;
					case 'C':
						m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(26));
						break;
					case 'K':
						m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(27));
						break;
					case 'B':
						m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(28));
						break;
					case 'R':
						m_Background.at(m_Background.size() - 1)->setTexture(m_Textures->getTexture(29));
						break;
				}
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
					m_vEnemies.at(m_vEnemies.size() - 1)->setClass(Assault);
					m_vEnemies.at(m_vEnemies.size() - 1)->useAI(new EnemyAI());
					m_vCharacters.push_back(m_vEnemies.at(m_vEnemies.size() - 1));
				}

				//Friendly units specifics
				else if (m_CurrentMap->getMapData().at(i).at(j) == 'P')
				{
					m_vUnits.push_back(new Character);
					m_vUnits.at(m_vUnits.size() - 1)->setTexture(m_Textures->getTexture(1)); //Sets the unit texture
					m_vUnits.at(m_vUnits.size() - 1)->setClass(Sniper);
					m_vUnits.at(m_vUnits.size() - 1)->setVision(true);
					m_vCharacters.push_back(m_vUnits.at(m_vUnits.size() - 1));
				}

				//Sets generic character data
				m_vCharacters.at(m_vCharacters.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setSize(sf::Vector2f(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y), std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setOrigin(sf::Vector2f(m_vCharacters.at(m_vCharacters.size() - 1)->getRect().width/2.0f, (m_vCharacters.at(m_vCharacters.size() - 1)->getRect().height / 2)));
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

	//Calculates the size of the unit UI so it scales relative to the screen size
	sf::Vector2f newSize(windowSize.x - m_CurrentMap->getWindowSize().x, ((windowSize.y - m_Toolbar.getSize().y - (5 * (m_vUnits.size() - 1))) / m_vUnits.size()));
	if (newSize.y > windowSize.y / 4)
	{
		newSize.y = windowSize.y / 4;
	}

	//Set up every UI window to fit on the right side of the screen
	for (int i = 0; i < m_vUnits.size(); i++)
	{
		m_vUnitUI.push_back(new HUDWindow);
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setSize(newSize);
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, (m_Toolbar.getSize().y + (i * m_vUnitUI.at(m_vUnitUI.size() - 1)->getSize().y)) + (i * 5)));
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setClassTexture(m_Textures->getTexture(3));
					   
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setBarLevels(HealthBar, m_vUnits.at(i)->getHealthData());
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setBarIcon(HealthBar, m_Textures->getTexture(12));		
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setBarText(HealthBar, std::to_string((int)m_vUnits.at(i)->getHealthData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().upper));
					   
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setBarLevels(AmmoBar, m_vUnits.at(i)->getAmmoData());
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setBarIcon(AmmoBar, m_Textures->getTexture(13));
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setBarText(AmmoBar, std::to_string((int)m_vUnits.at(i)->getAmmoData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().upper));
		  			   
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setLoadoutTexture(0, m_Textures->getTexture(15));
		m_vUnitUI.at(m_vUnitUI.size() - 1)->setLoadoutTexture(1, m_Textures->getTexture(15));
		  			   
		m_vUnitUI.at(m_vUnitUI.size() - 1)->scaleUI();
	}

	//Removes unnecessary wall edges
	edgeReduction(vEdgesX, vEdgesY);

	//Sets up the debug lines for the edges of the walls
	m_EdgeLines.resize(m_CurrentMap->m_vWallEdges.size()*2);
	m_EdgeLines.setPrimitiveType(sf::Lines);
	for (int i = 0; i < m_CurrentMap->m_vWallEdges.size(); i++)
	{
		
		m_EdgeLines[2 * i] = sf::Vertex(*m_CurrentMap->m_vWallEdges.at(i).first, sf::Color(255, 0, 0, 255));
		m_EdgeLines[(2 * i) + 1] = sf::Vertex(*m_CurrentMap->m_vWallEdges.at(i).second, sf::Color(255, 0, 0, 255));
		
	}

	//Sets up the exit button
	exitButton = new Button();
	exitButton->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
	exitButton->setPosition(sf::Vector2f(m_Toolbar.getPosition().x + m_Toolbar.getSize().x - exitButton->getSize().x, m_Toolbar.getPosition().y));
	exitButton->setTexture(m_Textures->getTexture(20));
	exitButton->setBackgroundColor(sf::Color(70, 70, 70, 255));

	loadPatrolPaths("Assets/Maps/CustomMapPaths.txt");

	Crosshair.setTexture(m_Textures->getTexture(38));
	Crosshair.setSize(sf::Vector2f(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y), std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y)));
	Crosshair.setOrigin(Crosshair.getSize().x / 2, Crosshair.getSize().y / 2);
	Crosshair.setFillColor(sf::Color(255, 255, 255, 155));
}

void Game::update(sf::Vector2i mousePos)
{	
	for (int i = 0; i < m_vWaves.size(); i++)
	{
		m_vWaves.at(i)->update();
		if (m_vWaves.at(i)->isDone())
		{
			delete  m_vWaves.at(i);
			 m_vWaves.at(i) = NULL;
			 m_vWaves.erase( m_vWaves.begin() + i);
		}
		else
		{
			for (int j = 0; j < m_vCharacters.size(); j++)
			{
				if ((Util::magnitude( m_vWaves.at(i)->getPosition() - m_vCharacters.at(j)->getPosition()) <  m_vWaves.at(i)->getRadius() + m_vCharacters.at(j)->getSize().y))
				{
					switch ( m_vWaves.at(i)->getType())
					{
					case Explosion:
						m_vCharacters.at(j)->setHealth(m_vCharacters.at(j)->getHealthData().lower - 1);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < m_vThrowables.size(); i++)
	{
		m_vThrowables.at(i)->update(mousePos);
		if (m_vThrowables.at(i)->isDone())
		{
			switch (m_vThrowables.at(i)->getType())
			{
				case Grenade:
					m_vWaves.push_back(new WaveEffect(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) * 2, 10, 0.5, sf::Vector2f(m_vThrowables.at(i)->getPosition()), Explosion));
					m_vWaves.push_back(new WaveEffect(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) * 10, 10, 0.5, sf::Vector2f(m_vThrowables.at(i)->getPosition()), Sound));
					break;

				case Flashbang:
					m_vWaves.push_back(new WaveEffect(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) * 3, 10, 0.5, sf::Vector2f(m_vThrowables.at(i)->getPosition()), Flash));
					m_vWaves.push_back(new WaveEffect(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) * 5, 10, 0.5, sf::Vector2f(m_vThrowables.at(i)->getPosition()), Sound));
					break;

				case Rock:
					m_vWaves.push_back(new WaveEffect(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) * 3, 10, 0.5, sf::Vector2f(m_vThrowables.at(i)->getPosition()), Sound));
					break;
			}

			delete  m_vThrowables.at(i);
			m_vThrowables.at(i) = NULL;
			m_vThrowables.erase(m_vThrowables.begin() + i);
		}
	}

	//Clears the temp edge data
	m_CurrentMap->clearEdges(&m_CurrentMap->m_vTempEdges);
	m_CurrentMap->clearCorners(&m_CurrentMap->m_vTempCorners);

	//Adds all of the door edges to the checked edges.
	for (int j = 0; j < m_vDoors.size(); j++)
	{
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> tempEdges = m_vDoors.at(j)->getEdges();
		for (int i = 0; i < 4; i++)
		{
			sf::Vector2f point[2] = { tempEdges.at(i).first, tempEdges.at(i).second };
			addEdge(point, &m_CurrentMap->m_vTempCorners, &m_CurrentMap->m_vTempEdges);
		}
	}
	
	//Clears all of the corners and adds the new corners
	m_CurrentMap->m_vCorners.clear();
	m_CurrentMap->m_vCorners.insert(std::end(m_CurrentMap->m_vCorners), std::begin(m_CurrentMap->m_vWallCorners), std::end(m_CurrentMap->m_vWallCorners));
	m_CurrentMap->m_vCorners.insert(std::end(m_CurrentMap->m_vCorners), std::begin(m_CurrentMap->m_vTempCorners), std::end(m_CurrentMap->m_vTempCorners));

	//Clears all of the edges and adds the new edges
	m_CurrentMap->clearEdges(&m_CurrentMap->m_vEdges);
	m_CurrentMap->m_vEdges.insert(std::end(m_CurrentMap->m_vEdges), std::begin(m_CurrentMap->m_vWallEdges), std::end(m_CurrentMap->m_vWallEdges));
	m_CurrentMap->m_vEdges.insert(std::end(m_CurrentMap->m_vEdges), std::begin(m_CurrentMap->m_vTempEdges), std::end(m_CurrentMap->m_vTempEdges)); //Adds all of the wall edges to the end of the checked edges

	if (m_CurrentSettings->debugActive())
	{
		//Sets up the debug lines for the edges of the walls
		m_EdgeLines.clear();
		if (m_CurrentSettings->debugActive())
		{
			m_EdgeLines.resize(m_CurrentMap->m_vEdges.size() * 2);
			m_EdgeLines.setPrimitiveType(sf::Lines);
			for (int i = 0; i < m_CurrentMap->m_vEdges.size(); i++)
			{
				m_EdgeLines[2 * i] = sf::Vertex(*m_CurrentMap->m_vEdges.at(i).first, sf::Color(255, 0, 0, 255));
				m_EdgeLines[(2 * i) + 1] = sf::Vertex(*m_CurrentMap->m_vEdges.at(i).second, sf::Color(255, 0, 0, 255));
			}
		}
	}

	//Perform character checks
	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		m_vCharacters.at(i)->update(); //Update data and states
	}
	if (m_vUnits.size() > 0)
	{
		if (m_vUnits.at(0)->bManualControls)
		{
			m_vUnits.at(0)->lookAt(sf::Vector2f(mousePos));

			if (mousePos.x > m_CurrentMap->getPosition().x &&
				mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
				mousePos.y > m_CurrentMap->getPosition().y &&
				mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
			{
				bDrawCrosshair = true;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					m_vUnits.at(0)->shoot();
				}
			}
			else
			{
				bDrawCrosshair = false;
			}

			//Window Collision
			if (m_vUnits.at(0)->getRect().left + m_vUnits.at(0)->getRect().width > m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x)
			{
				m_vUnits.at(0)->setPosition(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x - (m_vUnits.at(0)->getRect().width/2), m_vUnits.at(0)->getPosition().y));
			}
			if (m_vUnits.at(0)->getRect().left < m_CurrentMap->getPosition().x)
			{
				m_vUnits.at(0)->setPosition(sf::Vector2f(m_CurrentMap->getPosition().x + (m_vUnits.at(0)->getRect().width / 2), m_vUnits.at(0)->getPosition().y));
			}
			if (m_vUnits.at(0)->getRect().top + m_vUnits.at(0)->getRect().height > m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
			{
				m_vUnits.at(0)->setPosition(sf::Vector2f(m_vUnits.at(0)->getPosition().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y - (m_vUnits.at(0)->getRect().height / 2)));
			}
			if (m_vUnits.at(0)->getRect().top < m_CurrentMap->getPosition().y)
			{
				m_vUnits.at(0)->setPosition(sf::Vector2f(m_vUnits.at(0)->getPosition().x, m_CurrentMap->getPosition().y + (m_vUnits.at(0)->getRect().height / 2)));
			}
			
			//Wall Collision
			for (int j = 0; j < m_vWalls.size(); j++)
			{
				CollideTool.AABBBoxCollision(m_vUnits.at(0), m_vWalls.at(j));
			}

			if (bDrawCrosshair)
			{
				Crosshair.setPosition(sf::Vector2f(mousePos));
			}
		}
	}

	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		if (!m_vCharacters.at(i)->isDead())
		{
			m_vCharacters.at(i)->lazerEdgeChecks(); //Perform checks for the aiming
			m_vCharacters.at(i)->bulletEdgeChecks(); //Perform checks for any of the shot bullets
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
			if (!m_vCharacters.at(j)->isDead())
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
		}
		m_vDoors.at(i)->setOpen(bOpenDoor);
	}

	//UI
	for (int i = 0; i < m_vUnits.size(); i++)
	{
		//Updates the UI bars
		m_vUnitUI.at(i)->setBarLevels(HealthBar, m_vUnits.at(i)->getHealthData());
		m_vUnitUI.at(i)->setBarLevels(AmmoBar, m_vUnits.at(i)->getAmmoData());
		m_vUnitUI.at(i)->setBarText(HealthBar, std::to_string((int)m_vUnits.at(i)->getHealthData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().upper));

		//If the character is reloading replace the values with reloading text
		if (m_vUnits.at(i)->reloading())
		{
			m_vUnitUI.at(i)->setBarText(AmmoBar, "Reloading");
		}
		else
		{
			m_vUnitUI.at(i)->setBarText(AmmoBar, std::to_string((int)m_vUnits.at(i)->getAmmoData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().upper));
		}

		//Check if the mouse is above the buttons for highlighting
		m_vUnitUI.at(i)->getClassButton()->update(mousePos);
		for (int j = 0; j < m_vUnits.at(i)->getLoadoutSize(); j++)
		{
			m_vUnitUI.at(i)->getLoadoutButton(j)->update(mousePos);
		}
	}
	exitButton->update(mousePos);
}

void Game::characterInteractions(std::vector<Character*> vCharSet1, std::vector<Character*>  vCharSet2)
{
	//For every character in the first container
	for (int i = 0; i < vCharSet1.size(); i++)
	{
		//Check lazer collision
		int iIntersector = vCharSet1.at(i)->rayChecks(vCharSet2, 1);

		//If the lazer collided with a character
		if (iIntersector != -1)
		{
			//If a character is alive then set it to be a target
			if (vCharSet2.at(iIntersector)->getHealthData().lower > 0)
			{
				vCharSet1.at(i)->setTarget(vCharSet2.at(iIntersector));
			}
		}
		else //If no characters have been seen then set a Null target
		{
			vCharSet1.at(i)->setTarget(NULL);
		}

		//If the character is shooting check if the bullet hit opposing characters
		if (vCharSet1.at(i)->isShooting())
		{
			int iShot = vCharSet1.at(i)->rayChecks(vCharSet2, 0);

			if (iShot != -1)
			{
				vCharSet2.at(iShot)->setHealth(vCharSet2.at(iShot)->getHealthData().lower - vCharSet1.at(i)->getWeapon()->getDamage());
			}
		}

		
		//Check against every character in the second container
		for (int j = 0; j < vCharSet2.size(); j++)
		{
			if (!vCharSet1.at(i)->isDead() && !vCharSet2.at(j)->isDead())
			{
				//Dont allow checks on itself
				if (!(vCharSet1 == vCharSet2 && i == j))
				{
					for (int k = 0; k < vCharSet2.at(j)->getSoundWaves()->size(); k++)
					{
						vCharSet1.at(i)->hearsSound(vCharSet2.at(j)->getSoundWaves()->at(k));
					}
					
				}
			}
		}
	}
}

int Game::processInput(sf::Event keyCode, sf::Vector2i mousePos)
{
	if (keyCode.type == sf::Event::KeyPressed)
	{
		if (mousePos.x > m_CurrentMap->getPosition().x &&
			mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
			mousePos.y > m_CurrentMap->getPosition().y &&
			mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
		{
			switch (keyCode.key.code)
			{
				case sf::Keyboard::M:
					if (m_vUnits.size() > 0)
					{
						m_vUnits.at(0)->toggleManualControls();
					}
					break;

				case sf::Keyboard::R:
					if (m_vUnits.size() > 0)
					{
						if (m_vUnits.at(0)->bManualControls)
						{
							m_vUnits.at(0)->getWeapon()->reload();
						}
					}
					if (m_CurrentSettings->debugActive())
					{
						m_vThrowables.push_back(new Throwable(Rock, sf::Vector2i(m_vUnits.at(0)->getPosition())));
					}
					break;

				case sf::Keyboard::G:
					if (m_CurrentSettings->debugActive())
					{
						m_vThrowables.push_back(new Throwable(Grenade, sf::Vector2i(m_vUnits.at(0)->getPosition())));
					}
					break;

				case sf::Keyboard::F:
					if (m_CurrentSettings->debugActive())
					{
						m_vThrowables.push_back(new Throwable(Flashbang, sf::Vector2i(m_vUnits.at(0)->getPosition())));
					}
					break;
			}
		}
	}
	else if (keyCode.type == sf::Event::MouseButtonPressed)
	{
		switch (keyCode.key.code)
		{
			case sf::Mouse::Left:
				//Checks all of the UI Buttons
				for (int i = 0; i < m_vUnitUI.size(); i++)
				{
					//If a Class button is pressed
					if (m_vUnitUI.at(i)->getClassButton()->hovering(mousePos))
					{
						//Switches the class of the unit
						switch (m_vUnits.at(i)->getClass())
						{
						case Sniper:
							m_vUnits.at(i)->setClass(Support);
							m_vUnitUI.at(i)->setClassTexture(m_Textures->getTexture(8));
							break;
						case Support:
							m_vUnits.at(i)->setClass(Shotgunner);
							m_vUnitUI.at(i)->setClassTexture(m_Textures->getTexture(9));
							break;
						case Shotgunner:
							m_vUnits.at(i)->setClass(Assault);
							m_vUnitUI.at(i)->setClassTexture(m_Textures->getTexture(10));
							break;
						case Assault:
							m_vUnits.at(i)->setClass(Sniper);
							m_vUnitUI.at(i)->setClassTexture(m_Textures->getTexture(3));
							break;
						}
					}

					for (int j = 0; j < m_vUnits.at(i)->getLoadoutSize(); j++)
					{
						//If a loadout button is pressd
						if (m_vUnitUI.at(i)->getLoadoutButton(j)->hovering(mousePos))
						{
							//Switches the class of the unit
							m_vUnits.at(i)->setLoadoutItem(j, m_vUnits.at(i)->getNextLoadoutItem(m_vUnits.at(i)->getLoadoutItem(j)));

							//Switches the class image once changed
							switch (m_vUnits.at(i)->getLoadoutItem(j))
							{
							case Lazer:
								m_vUnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(16));
								break;

							case Silencer:
								m_vUnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(17));
								break;

							case Scope:
								m_vUnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(18));
								break;

							case None:
								m_vUnitUI.at(i)->setLoadoutTexture(j, m_Textures->getTexture(15));
								break;
							}
							m_vUnitUI.at(m_vUnitUI.size() - 1)->scaleUI();
						}
					}
				}

				//If the exit button is pressed return to the menu
				if (exitButton->hovering(mousePos))
				{
					return S_Menu;
				}
				break;

			case sf::Mouse::Right:
				if (mousePos.x > m_CurrentMap->getPosition().x &&
					mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
					mousePos.y > m_CurrentMap->getPosition().y &&
					mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
				{
					if (m_vUnits.size() > 0)
					{
						m_vUnits.at(0)->setPath(m_vUnits.at(0)->getPosition(), (sf::Vector2f)mousePos); //Sets a path towards the clicked area
					}
				}
				break;
		}
	}
			
	return S_None;
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Game::edgeReduction(std::vector<sf::Vector2f> vXEdges, std::vector<sf::Vector2f> vYEdges)
{
	//Reduces the amounts of edges for each wall by combining adjacent edges and removing unreachable edges.
	std::vector<float> vfXValues; //Holds all X values used for wall edges
	std::vector<float> vfYValues; //Holds all Y values used for wall edges
	bool bFoundMatch = false; //Used to check if that X or Y value has already been entered
	std::pair<sf::Vector2f, sf::Vector2f> newValue;

	//Edge sorting
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
					//A match has been found so dont add the value
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
		if (vfYValues.size() == 0) //If there are no current y values add one
		{
			vfYValues.push_back(vYEdges.at(i).y);
		}
		else
		{
			for (int j = 0; j < vfYValues.size(); j++) //For every current y value found
			{
				if (vYEdges.at(i).y == vfYValues.at(j)) //If the y matches to the current Ys
				{
					//A match has been found so dont add the value
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the ys match add the y to the list of xs
			{
				vfYValues.push_back(vYEdges.at(i).y);
			}
		}
	}

	std::vector<std::vector<sf::Vector2f>> vComparissonVectsX; //Holds the X edge vectors to compare
	std::vector<std::vector<sf::Vector2f>> vComparissonVectsY;  //Holds the Y edge vectors to compare

	//Check the vectors against the X values collected and pushes the vector back if it matches for sorting
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

	//Check the vectors against the Y values collected and pushes the vector back if it matches for sorting
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

	bool bCurrentlyEqual = true;
	int iBegin = 0;
	int iEnd = 0;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> vEdges;

	//For every x value
	for (int i = 0; i < vComparissonVectsX.size(); i++)
	{
		iBegin = 0;
		iEnd = 0;
		//For every vector that has this x value
		for (int j = 0; j < vComparissonVectsX.at(i).size(); j++)
		{
			//While the x values of adjacent edges top and bottom sides  are the same
			while (bCurrentlyEqual) 
			{
				//If moving up the vector doesnt exceed the extents
				if (j + 2 < vComparissonVectsX.at(i).size())
				{
					//If the ys match move to the next set of edges
					if (abs(vComparissonVectsX.at(i).at(j + 1).y- vComparissonVectsX.at(i).at(j + 2).y) < 0.1f)
					{
						j += 2;
					}
					else //Else set the current point as the end of the edge
					{
						j++;
						iEnd = j;
						bCurrentlyEqual = false;
					}
				}
				else if (j + 2 == vComparissonVectsX.at(i).size())
				{
					iEnd = j + 1;
					bCurrentlyEqual = false;
				}
				else
				{
					iEnd = j;
					bCurrentlyEqual = false;
				}
			}
			//If the end of the vector has been reached store the current edge
			if (j < vComparissonVectsX.at(i).size() - 1)
			{
				sf::Vector2f newEdge[] { vComparissonVectsX.at(i).at(iBegin), vComparissonVectsX.at(i).at(iEnd) };
				addEdge(newEdge, &m_CurrentMap->m_vWallCorners, &m_CurrentMap->m_vWallEdges);
				iBegin = j + 1;
			}
			bCurrentlyEqual = true;
		}
	}

	//For every y value
	for (int i = 0; i < vComparissonVectsY.size(); i++)
	{
		iBegin = 0;
		iEnd = 0;
		for (int j = 0; j < vComparissonVectsY.at(i).size(); j++)
		{
			//While the y values of adjacent edges left and right sides  are the same
			while (bCurrentlyEqual)
			{
				//If moving up the vector doesnt exceed the extents
				if (j + 2 < vComparissonVectsY.at(i).size())
				{
					//If the Xs match move to the next set of edges
					if (abs(vComparissonVectsY.at(i).at(j + 1).x - vComparissonVectsY.at(i).at(j + 2).x) < 0.1f)
					{
						j += 2;
					}
					else //Else set the current point as the end of the edge
					{
						j++;
						iEnd = j;
						bCurrentlyEqual = false;
					}
				}
				else if (j + 2 == vComparissonVectsY.at(i).size())
				{
					iEnd = j + 1;
					bCurrentlyEqual = false;
				}
				else
				{
					iEnd = j;
					bCurrentlyEqual = false;
				}
			}
			//If the end of the vector has been reached store the current edge
			if (j < vComparissonVectsY.at(i).size() - 1)
			{
				sf::Vector2f newEdge[] { vComparissonVectsY.at(i).at(iBegin), vComparissonVectsY.at(i).at(iEnd) };
				addEdge(newEdge, &m_CurrentMap->m_vWallCorners, &m_CurrentMap->m_vWallEdges);
				iBegin = j + 1;
			}
			bCurrentlyEqual = true;
		}
	}

	return vEdges; //Return the reduced edges
}

void Game::loadPatrolPaths(std::string sDir)
{
	std::ifstream pathFile;
	pathFile.open(sDir); //Open the map file

	if (pathFile.is_open()) //If the file opened correctly
	{
		//Initialise reading variables
		std::string sLine;
		std::string sCurrentNum;
		std::vector<int> viPathNodes;

		int iIndex = 0;
		while (!pathFile.eof()) //while the end of file hasnt been reached
		{
			viPathNodes.clear();
			sCurrentNum = "";
			getline(pathFile, sLine); //Get the next line

			if (sLine != "")
			{
				std::istringstream sNum(sLine);

				sNum >> sCurrentNum; //Read the index
				while (!sNum.eof())
				{
					sNum >> sCurrentNum; //Read the number
					viPathNodes.push_back(stoi(sCurrentNum)); //Add it to the vector
				}
				m_vEnemies.at(iIndex)->setPatrolPath(viPathNodes);
				iIndex++;
			}
		}
	}
	else
	{
		//Ouptut an error
		std::cout << "Path File: " << sDir << " could not be opened." << "\n";
	}

	pathFile.close();
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Draws the floor
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

	//Draws the sound waves
	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		for (int j = 0; j < m_vCharacters.at(i)->getSoundWaves()->size(); j++)
		{
			if (m_vCharacters.at(i)->getSoundWaves()->at(j) != NULL)
			{
				target.draw(*m_vCharacters.at(i)->getSoundWaves()->at(j));
			}
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

	for (int i = 0; i <  m_vWaves.size(); i++)
	{
		target.draw(* m_vWaves.at(i));
	}

	for (int i = 0; i < m_vThrowables.size(); i++)
	{
		target.draw(*m_vThrowables.at(i));
	}

	if (m_CurrentSettings->debugActive())
	{
		//Draws the edges of the walls
		target.draw(m_EdgeLines);
	}

	for (int i = 0; i < m_vUnitUI.size(); i++)
	{
		target.draw(*m_vUnitUI.at(i));
	}

	if (m_vUnits.size() > 0)
	{
		if (m_vUnits.at(0)->bManualControls && bDrawCrosshair)
		{
			target.draw(Crosshair);
		}
	}

	//Draws the UI
	target.draw(m_Toolbar);
	target.draw(*exitButton);
}

void Game::addEdge(sf::Vector2f points[2], std::vector<sf::Vector2f*>* vCornerList, std::vector<std::pair<sf::Vector2f*, sf::Vector2f*>>* vEdgeList)
{
	sf::Vector2f* aiPointIndices[2] {NULL, NULL};
	
	bool bFound = false;

	//For both points
	for (int i = 0; i < 2; i++)
	{
		bFound = false;
		for (int j = 0; j < vCornerList->size(); j++) //Check the corner list
		{
			//If the point exists on the corner list
			if (abs(points[i].x - (*vCornerList->at(j)).x) <  0.01f &&
				abs(points[i].y - (*vCornerList->at(j)).y) <  0.01f ) 
			{
				aiPointIndices[i] = vCornerList->at(j); //Assign the corner to that address
				bFound = true;
			}
		}
		if (!bFound || m_CurrentMap->m_vCorners.size() == 0) //If the corner isnt in the list
		{
			//Create a new corner
			aiPointIndices[i] = new sf::Vector2f(points[i]); 
			vCornerList->push_back(aiPointIndices[i]);
		}
	}

	//Add the edge to the list
	vEdgeList->push_back(std::pair <sf::Vector2f*, sf::Vector2f*>( aiPointIndices[0], aiPointIndices[1]));
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
		m_vCharacters.at(i)->~Character();
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

	for (int i = 0; i < m_vUnitUI.size(); i++)
	{
		delete(m_vUnitUI.at(i));
		m_vUnitUI.at(i) = NULL;
	}

	if (m_CurrentMap != NULL)
	{
		m_CurrentMap->clearAllEdgeData();
	}

	delete(exitButton);
	exitButton = NULL;

	for (int i = 0; i < m_vThrowables.size(); i++)
	{
		if (m_vThrowables.at(i) != NULL)
		{
			delete m_vThrowables.at(i);
			m_vThrowables.at(i) = NULL;
		}
	}

	for (int i = 0; i < m_vWaves.size(); i++)
	{
		if (m_vWaves.at(i) != NULL)
		{
			delete m_vWaves.at(i);
			m_vWaves.at(i) = NULL;
		}
	}
}