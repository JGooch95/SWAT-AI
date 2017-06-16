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
	m_vEdges = edgeReduction(vEdgesX, vEdgesY);

	//Sets up the debug lines for the edges of the walls
	m_EdgeLines.resize(m_vEdges.size()*2);
	m_EdgeLines.setPrimitiveType(sf::Lines);
	for (int i = 0; i < m_vEdges.size(); i++)
	{
		m_EdgeLines[2 * i] = sf::Vertex(m_vEdges.at(i).first, sf::Color(255, 0, 0, 255));
		m_EdgeLines[(2 * i) + 1] = sf::Vertex(m_vEdges.at(i).second, sf::Color(255, 0, 0, 255));
	}

	//Sets up the exit button
	exitButton = new Button();
	exitButton->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
	exitButton->setPosition(sf::Vector2f(m_Toolbar.getPosition().x + m_Toolbar.getSize().x - exitButton->getSize().x, m_Toolbar.getPosition().y));
	exitButton->setTexture(m_Textures->getTexture(20));
	exitButton->setBackgroundColor(sf::Color(70, 70, 70, 255));

	loadPatrolPaths("Assets/Maps/CustomMapPaths.txt");
}

void Game::update(sf::Vector2i mousePos)
{
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> vEdgesToCheck; //Holds all of the edges to be checked this frame

	//Adds all of the door edges to the checked edges.
	for (int j = 0; j < m_vDoors.size(); j++)
	{
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> tempEdges = m_vDoors.at(j)->getEdges();
		vEdgesToCheck.insert(std::end(vEdgesToCheck), std::begin(tempEdges), std::end(tempEdges));
	}
	vEdgesToCheck.insert(std::end(vEdgesToCheck), std::begin(m_vEdges), std::end(m_vEdges)); //Adds all of the wall edges to the end of the checked edges

	//Perform character checks
	for (int i = 0; i < m_vCharacters.size(); i++)
	{
		m_vCharacters.at(i)->update(); //Update data and states
		if (!m_vCharacters.at(i)->isDead())
		{
			m_vCharacters.at(i)->lazerChecks(vEdgesToCheck); //Perform checks for the aiming
			m_vCharacters.at(i)->visionCalculation(vEdgesToCheck); //Perform checks for the vision cone
			m_vCharacters.at(i)->bulletChecks(vEdgesToCheck); //Perform checks for any of the shot bullets
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
		m_vUnitUI.at(i)->getClassButton()->hovering(mousePos);
		for (int j = 0; j < m_vUnits.at(i)->getLoadoutSize(); j++)
		{
			m_vUnitUI.at(i)->getLoadoutButton(j)->hovering(mousePos);
		}
	}

	//Check if the mouse is above the exit button for highlighting
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
			if (!vCharSet1.at(i)->isDead() && !vCharSet2.at(j)->isDead())
			{
				//Dont allow checks on itself
				if (!(vCharSet1 == vCharSet2 && i == j))
				{
					//Check lazer collision
					if (vCharSet1.at(i)->lazerChecks({ vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation())}))
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
						vCharSet2.at(j)->setHealth(vCharSet2.at(j)->getHealthData().lower - vCharSet1.at(i)->bulletChecks({ vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation())}));
						if (vCharSet2.at(j)->getAimingState() != AIM)
						{
							vCharSet2.at(j)->setAimingState(SEARCH_SPIN);
						}
					}

					for (int k = 0; k < vCharSet2.at(j)->getSoundWaves()->size(); k++)
					{
						if (vCharSet1.at(i)->hearsSound(vCharSet2.at(j)->getSoundWaves()->at(k)))
						{
							vCharSet1.at(i)->setPath(vCharSet1.at(i)->getPosition(), vCharSet2.at(j)->getSoundWaves()->at(k)->getPosition());
						}
					}
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

int Game::clickLeft(sf::Vector2i mousePos)
{
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
		return 1;
	}
	return 0;
}

void Game::clickRight(sf::Vector2i mousePos)
{
	if (m_vUnits.size() > 0)
	{
		m_vUnits.at(0)->setMovementState(MOVE_TO_SPOT);
		m_vUnits.at(0)->setPath(m_vUnits.at(0)->getPosition(), (sf::Vector2f)mousePos); //Sets a path towards the clicked area
	}
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
					if (vComparissonVectsX.at(i).at(j + 1).y == vComparissonVectsX.at(i).at(j + 2).y)
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
				newValue.first = vComparissonVectsX.at(i).at(iBegin);
				newValue.second = vComparissonVectsX.at(i).at(iEnd);
				vEdges.push_back(newValue);
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
					if (vComparissonVectsY.at(i).at(j + 1).x == vComparissonVectsY.at(i).at(j + 2).x)
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
				newValue.first = vComparissonVectsY.at(i).at(iBegin);
				newValue.second = vComparissonVectsY.at(i).at(iEnd);
				vEdges.push_back(newValue);
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
		std::string currentNum;
		std::vector<int> viPathNodes;

		float index = 0;
		while (!pathFile.eof()) //while the end of file hasnt been reached
		{
			viPathNodes.clear();
			currentNum = "";
			getline(pathFile, sLine); //Get the next line

			if (sLine != "")
			{
				for (int i = 3; i < sLine.length(); i++)
				{
					if (sLine.at(i) != ' ')
					{
						currentNum = currentNum + sLine.at(i);
					}
					else if (currentNum != "" || i == sLine.length() - 1)
					{
						viPathNodes.push_back(stoi(currentNum));
						currentNum = "";
					}
				}
				if (currentNum != "")
				{
					viPathNodes.push_back(stoi(currentNum));
				}
				m_vEnemies.at(index)->setPatrolPath(viPathNodes);
				index++;
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

	if (m_CurrentSettings->debugActive())
	{
		//Draws the edges of the walls
		target.draw(m_EdgeLines);
	}

	for (int i = 0; i < m_vUnitUI.size(); i++)
	{
		target.draw(*m_vUnitUI.at(i));
	}

	//Draws the UI
	target.draw(m_Toolbar);
	target.draw(*exitButton);
}

/*void Game::addEdge(sf::Vector2f points[2])
{
	int iPointIndices[2] { 0, 0 };
	
	bool bFound = false;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < m_vCorners.size(); j++)
		{
			if (points[i] == m_vCorners.at(j))
			{
				iPointIndices[i] = j;
				bFound = true;
			}
		}
		if (!bFound)
		{
			m_vCorners.push_back(points[i]);
		}
	}

	m_vEdges.push_back(std::pair <int, int>( iPointIndices[0], iPointIndices[1]));
}*/

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

	delete(exitButton);
	exitButton = NULL;
}