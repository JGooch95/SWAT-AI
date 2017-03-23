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

	m_CurrentMap->setup(
		sf::FloatRect(sf::Vector2f(0, m_Toolbar.getSize().y), sf::Vector2f(windowSize) - sf::Vector2f(windowSize.x/3, m_Toolbar.getSize().y)),
		sf::Vector2f(20, 20)
	);
	m_CurrentMap->load("./Assets/Maps/1.txt"); //Loads a map from a file

	//Loads the main textures into the software
	m_Textures.loadTextures(std::vector<std::string>{
		"Assets/Sprites/Wood.jpg",						//0
		"Assets/Sprites/Unit.png",						//1
		"Assets/Sprites/Enemy.png",						//2
		"Assets/Sprites/SniperIcon.png",				//3
		"Assets/Sprites/Sniper.png",					//4
		"Assets/Sprites/LMG.png",						//5
		"Assets/Sprites/Shotgun.png",					//6
		"Assets/Sprites/Assault.png",					//7
		"Assets/Sprites/SupportIcon.png",				//8
		"Assets/Sprites/ShotgunnerIcon.png",			//9
		"Assets/Sprites/AssaultIcon.png",				//10
		"Assets/Sprites/Muzzle.png",					//11
		"Assets/Sprites/Health.png",					//12
		"Assets/Sprites/Ammo.png",						//13
		"Assets/Sprites/Grass.jpg"						//14
	});

	m_CurrentFont.loadFromFile("Assets/Fonts/arial.ttf"); //Loads the main font

	m_Background.setTexture(m_Textures.getTexture(14));
	m_Background.setArea(sf::Vector2f(m_CurrentMap->getWindowSize()));
	m_Background.setPosition(sf::Vector2f(m_CurrentMap->getPosition()));

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
			if (m_CurrentMap->getMapData().at(i).at(j) == 'W')
			{
				m_vWalls.push_back(new Object);
				m_vWalls.at(m_vWalls.size() - 1)->setTexture(m_Textures.getTexture(0)); //Sets the unit texture
				m_vWalls.at(m_vWalls.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y));
				m_vWalls.at(m_vWalls.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));

				if (m_CurrentMap->getMapData().at(i - 1).at(j) != 'W' && i > 0)
				{
					//Top Edge
					vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
				}
				if (m_CurrentMap->getMapData().at(i + 1).at(j) != 'W' && i < m_CurrentMap->getMapData().at(i).size())
				{
					//Bottom Edge
					vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
					vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
				}
				
				if (m_CurrentMap->getMapData().at(i).at(j-1) != 'W' && j > 0)
				{
					//Left Edge
					vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
				}
				if (m_CurrentMap->getMapData().at(i).at(j + 1) != 'W' && j < m_CurrentMap->getMapData().at(i).size())
				{
					//Right Edge
					vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
				}
			}

			//Characters
			if (m_CurrentMap->getMapData().at(i).at(j) == 'E' || m_CurrentMap->getMapData().at(i).at(j) == 'P')
			{
				//Enemies specifics
				if (m_CurrentMap->getMapData().at(i).at(j) == 'E')
				{
					m_vEnemies.push_back(new Character);
					m_vEnemies.at(m_vEnemies.size() - 1)->setTexture(m_Textures.getTexture(2)); //Sets the unit texture
					m_vEnemies.at(m_vEnemies.size() - 1)->setClass(Assault, m_Textures.getTexture(7));
					m_vCharacters.push_back(m_vEnemies.at(m_vEnemies.size() - 1));
				}

				//Friendly units specifics
				else if (m_CurrentMap->getMapData().at(i).at(j) == 'P')
				{
					m_vUnits.push_back(new Character);
					m_vUnits.at(m_vUnits.size() - 1)->setTexture(m_Textures.getTexture(1)); //Sets the unit texture
					m_vUnits.at(m_vUnits.size() - 1)->setClass(Sniper, m_Textures.getTexture(4));
					m_vUnits.at(m_vUnits.size() - 1)->setVision(true);
					m_vCharacters.push_back(m_vUnits.at(m_vUnits.size() - 1));
				}

				//Sets generic character data
				m_vCharacters.at(m_vCharacters.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setSize(sf::Vector2f(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y), std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y)));
				m_vCharacters.at(m_vCharacters.size() - 1)->setMuzzle(m_Textures.getTexture(11));
			}
			
			//Doors
			if (m_CurrentMap->getMapData().at(i).at(j) == 'D')
			{
				m_vDoors.push_back(new Entrance); 
				m_vDoors.at(m_vDoors.size() - 1)->setTexture(m_Textures.getTexture(0));

				if ((m_CurrentMap->getMapData().at(i).at(j - 1) == 'W') && j > 0)
				{
					m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y / 4));
					m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(0, ((m_vDoors.at(m_vDoors.size() - 1)->getSize().y * m_vDoors.at(m_vDoors.size() - 1)->getScale().y) / 2.0f)));
					m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
				}
				else if ((m_CurrentMap->getMapData().at(i).at(j + 1) == 'W') && j < m_CurrentMap->getMapData().at(i).size())
				{
					m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y / 4));
					m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f(0, ((m_vDoors.at(m_vDoors.size() - 1)->getSize().y * m_vDoors.at(m_vDoors.size() - 1)->getScale().y) / 2.0f)));
					m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f(((j+1) * m_CurrentMap->getTileSize().x), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
					m_vDoors.at(m_vDoors.size() - 1)->setOrientation(180);
					m_vDoors.at(m_vDoors.size() - 1)->setDirection(-1);
				}
				else if ((m_CurrentMap->getMapData().at(i - 1).at(j) == 'W') && i > 0)
				{
					m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x / 4, m_CurrentMap->getTileSize().y));
					m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f((m_vDoors.at(m_vDoors.size() - 1)->getSize().x * m_vDoors.at(m_vDoors.size() - 1)->getScale().x) / 2.0f, 0));
					m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), (i * m_CurrentMap->getTileSize().y)));
				}
				else if ((m_CurrentMap->getMapData().at(i + 1).at(j) == 'W') && i < m_CurrentMap->getMapData().size())
				{
					m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x / 4, m_CurrentMap->getTileSize().y));
					m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f((m_vDoors.at(m_vDoors.size() - 1)->getSize().x * m_vDoors.at(m_vDoors.size() - 1)->getScale().x) / 2.0f, 0));
					m_vDoors.at(m_vDoors.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), ((i + 1) * m_CurrentMap->getTileSize().y)));
					m_vDoors.at(m_vDoors.size() - 1)->setOrientation(180);
					m_vDoors.at(m_vDoors.size() - 1)->setDirection(-1);
				}
				else
				{
					m_vDoors.at(m_vDoors.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x / 4, m_CurrentMap->getTileSize().y));
					m_vDoors.at(m_vDoors.size() - 1)->setOrigin(sf::Vector2f((m_vDoors.at(m_vDoors.size() - 1)->getSize().x * m_vDoors.at(m_vDoors.size() - 1)->getScale().x) / 2.0f, 0));
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
		m_UnitUI.at(m_UnitUI.size() - 1)->setClassTexture(m_Textures.getTexture(3));
					
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarLevels(HealthBar, m_vUnits.at(i)->getHealthData());
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarIcon(HealthBar, m_Textures.getTexture(12));		
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarText(HealthBar, std::to_string((int)m_vUnits.at(i)->getHealthData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().upper));
					
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarLevels(AmmoBar, m_vUnits.at(i)->getAmmoData());
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarIcon(AmmoBar, m_Textures.getTexture(13));
		m_UnitUI.at(m_UnitUI.size() - 1)->setBarText(AmmoBar, std::to_string((int)m_vUnits.at(i)->getAmmoData().lower) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().upper));
					
		m_UnitUI.at(m_UnitUI.size() - 1)->setFont(&m_CurrentFont);
		m_UnitUI.at(m_UnitUI.size() - 1)->scaleUI();
	}

	m_vEdges = edgeReduction(vEdgesX, vEdgesY);

	m_EdgeLines.resize(m_vEdges.size());
	m_EdgeLines.setPrimitiveType(sf::Lines);
	for (int i = 0; i < m_vEdges.size(); i++)
	{
		m_EdgeLines[i] = sf::Vertex(m_vEdges.at(i), sf::Color(255,0,0,255));
	}

	m_Pathfinder.setup();
}

void Game::update(sf::Vector2i mousePos)
{
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
	}
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

				//Take health off if a bullet has been shot
				vCharSet2.at(j)->setHealth(vCharSet2.at(j)->getHealthData().lower - vCharSet1.at(i)->bulletChecks({ vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(0),
																												vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(1) }));
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

void Game::clickLeft(sf::Vector2i mousePos)
{
	//Checks all of the UI Buttons
	for (int i = 0; i < m_UnitUI.size(); i++)
	{
		//If a Class button is pressed
		if (m_UnitUI.at(i)->getClassButton()->clicked(mousePos))
		{
			//Switches the class of the unit
			switch (m_vUnits.at(i)->getClass())
			{
			case Sniper:
				m_vUnits.at(i)->setClass(Support, m_Textures.getTexture(5));
				m_UnitUI.at(i)->setClassTexture(m_Textures.getTexture(8));
				break;
			case Support:
				m_vUnits.at(i)->setClass(Shotgunner, m_Textures.getTexture(6));
				m_UnitUI.at(i)->setClassTexture(m_Textures.getTexture(9));
				break;
			case Shotgunner:
				m_vUnits.at(i)->setClass(Assault, m_Textures.getTexture(7));
				m_UnitUI.at(i)->setClassTexture(m_Textures.getTexture(10));
				break;
			case Assault:
				m_vUnits.at(i)->setClass(Sniper, m_Textures.getTexture(4));
				m_UnitUI.at(i)->setClassTexture(m_Textures.getTexture(3));
				break;
			}
		}
	}
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
	target.draw(m_Background);
	if (m_CurrentSettings->debugActive())
	{
		target.draw(*m_CurrentMap); //Draws the Grid
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

	delete(m_CurrentMap);
	m_CurrentMap = NULL;

	delete(m_CurrentSettings);
	m_CurrentSettings = NULL;
}