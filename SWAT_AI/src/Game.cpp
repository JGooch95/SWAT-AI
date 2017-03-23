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
	std::vector<sf::Vector2f> m_vEdgesX; 
	std::vector<sf::Vector2f> m_vEdgesY; 

	//Adds borders of the window to the edge checks
	m_vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y));
	m_vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y));

	m_vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));
	m_vEdgesY.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));

	m_vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y));
	m_vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));

	m_vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y));
	m_vEdgesX.push_back(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y));

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
				//m_vWalls.at(m_vWalls.size() - 1)->linkMap(&m_CurrentMap); //Sets up the grid values for units
				m_vWalls.at(m_vWalls.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f(j * m_CurrentMap->getTileSize().x, i * m_CurrentMap->getTileSize().y));
				m_vWalls.at(m_vWalls.size() - 1)->setSize(sf::Vector2f(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y));

				if (m_CurrentMap->getMapData().at(i - 1).at(j) != 'W' && i > 0)
				{
					//Top Edge
					m_vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					m_vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
				}
				if (m_CurrentMap->getMapData().at(i + 1).at(j) != 'W' && i < m_CurrentMap->getMapData().at(i).size())
				{
					//Bottom Edge
					m_vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
					m_vEdgesY.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
				}
				
				if (m_CurrentMap->getMapData().at(i).at(j-1) != 'W' && j > 0)
				{
					//Left Edge
					m_vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					m_vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
				}
				if (m_CurrentMap->getMapData().at(i).at(j + 1) != 'W' && j < m_CurrentMap->getMapData().at(i).size())
				{
					//Right Edge
					m_vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top));
					m_vEdgesX.push_back(sf::Vector2f(m_vWalls.at(m_vWalls.size() - 1)->getRect().left + m_vWalls.at(m_vWalls.size() - 1)->getRect().width, m_vWalls.at(m_vWalls.size() - 1)->getRect().top + m_vWalls.at(m_vWalls.size() - 1)->getRect().height));
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
					m_vUnits.at(m_vUnits.size() - 1)->setClass(Shotgunner, m_Textures.getTexture(6));
					m_vUnits.at(m_vUnits.size() - 1)->setVision(true);
					m_vCharacters.push_back(m_vUnits.at(m_vUnits.size() - 1));
				}

				//Sets generic character data
				m_vCharacters.at(m_vCharacters.size() - 1)->setPosition(m_CurrentMap->getPosition() + sf::Vector2f((j * m_CurrentMap->getTileSize().x) + (m_CurrentMap->getTileSize().x / 2), (i * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize().y / 2)));
				//m_vCharacters.at(m_vCharacters.size() - 1)->linkMap(&m_CurrentMap); //Sets up the grid values for units
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
				//m_vDoors.at(m_vDoors.size() - 1)->linkMap(&m_CurrentMap);
			}
		}
	}

	//Creating the UI for each unit
	for (int i = 0; i < m_vUnits.size(); i++)
	{
		unitScreen.push_back(new sf::RectangleShape);
		sf::Vector2f newSize(windowSize.x - m_CurrentMap->getWindowSize().x, ((windowSize.y - m_Toolbar.getSize().y - (5* (m_vUnits.size()-1))) / m_vUnits.size()));
		if (newSize.y > windowSize.y / 4)
		{
			newSize.y = windowSize.y / 4;
		}
		unitScreen.at(unitScreen.size() - 1)->setSize(newSize);
		unitScreen.at(unitScreen.size() - 1)->setPosition(sf::Vector2f(m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x, (m_Toolbar.getSize().y + (i * unitScreen.at(unitScreen.size() - 1)->getSize().y)) + (i*5)));
		unitScreen.at(unitScreen.size() - 1)->setFillColor(sf::Color(120,120, 120,255));
		
		UIBars.push_back(new Bar);
		UIBars.at(UIBars.size() - 1)->setSize(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getSize().x - (unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f),
														   unitScreen.at(unitScreen.size() - 1)->getSize().y/10));
		UIBars.at(UIBars.size() - 1)->setPosition(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getPosition().x + ((unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f) / 2.0f),
														       unitScreen.at(unitScreen.size() - 1)->getPosition().y + (2 * (unitScreen.at(unitScreen.size() - 1)->getSize().y / 5.0f))));
		UIBars.at(UIBars.size() - 1)->setLevelColor(sf::Color(0,255,0,255));
		UIBars.at(UIBars.size() - 1)->setBarColor(sf::Color(255, 0, 0, 255));

		Icons.push_back(Object());
		Icons.at(Icons.size() - 1).setTexture(m_Textures.getTexture(12));
		Icons.at(Icons.size() - 1).setSize(sf::Vector2f(UIBars.at(UIBars.size() - 1)->getSize().y, UIBars.at(UIBars.size() - 1)->getSize().y));
		Icons.at(Icons.size() - 1).setPosition(sf::Vector2f(UIBars.at(UIBars.size() - 1)->getPosition().x - (Icons.at(Icons.size() - 1).getSize().x* Icons.at(Icons.size() - 1).getScale().x), UIBars.at(UIBars.size() - 1)->getPosition().y));

		UIText.push_back(new sf::Text(std::to_string((int)m_vUnits.at(i)->getHealthData().x) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().y), m_CurrentFont, 20));
		while (UIText.at(UIText.size() - 1)->getGlobalBounds().height > UIBars.at(UIBars.size() - 1)->getSize().y *(2.0f / 3.0f))
		{
			UIText.at(UIText.size() - 1)->setCharacterSize(UIText.at(UIText.size() - 1)->getCharacterSize() - 1);
		}
		UIText.at(UIText.size() - 1)->setPosition(UIBars.at(UIBars.size() - 1)->getPosition() + sf::Vector2f((UIBars.at(UIBars.size() - 1)->getSize().x / 2) - (UIText.at(UIText.size() - 1)->getLocalBounds().width / 2), (UIBars.at(UIBars.size() - 1)->getSize().y / 3)));
		UIText.at(UIText.size() - 1)->setFillColor(sf::Color(0, 0, 0, 255));

		UIBars.push_back(new Bar);
		UIBars.at(UIBars.size() - 1)->setSize(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getSize().x - (unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f),
			unitScreen.at(unitScreen.size() - 1)->getSize().y / 10));
		UIBars.at(UIBars.size() - 1)->setPosition(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getPosition().x + ((unitScreen.at(unitScreen.size() - 1)->getSize().x / 5.0f) / 2.0f),
															   unitScreen.at(unitScreen.size() - 1)->getPosition().y + (3*(unitScreen.at(unitScreen.size() - 1)->getSize().y / 5.0f))));
		UIBars.at(UIBars.size() - 1)->setLevelColor(sf::Color(255, 255, 0, 255));
		UIBars.at(UIBars.size() - 1)->setBarColor(sf::Color(255, 0, 0, 255));
		UIBars.at(UIBars.size() - 1)->setLimit(m_vUnits.at(i)->getAmmoData().y);

		Icons.push_back(Object());
		Icons.at(Icons.size() - 1).setTexture(m_Textures.getTexture(13));
		Icons.at(Icons.size() - 1).setSize(sf::Vector2f(UIBars.at(UIBars.size() - 1)->getSize().y, UIBars.at(UIBars.size() - 1)->getSize().y));
		Icons.at(Icons.size() - 1).setPosition(sf::Vector2f(UIBars.at(UIBars.size() - 1)->getPosition().x - (Icons.at(Icons.size() - 1).getSize().x * Icons.at(Icons.size() - 1).getScale().x), UIBars.at(UIBars.size() - 1)->getPosition().y));

		UIText.push_back(new sf::Text(std::to_string((int)m_vUnits.at(i)->getAmmoData().x) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().y), m_CurrentFont, 20));
		while (UIText.at(UIText.size() - 1)->getGlobalBounds().height > UIBars.at(UIBars.size() - 1)->getSize().y *(2.0f/3.0f))
		{
			UIText.at(UIText.size() - 1)->setCharacterSize(UIText.at(UIText.size() - 1)->getCharacterSize()-1);
		}
		UIText.at(UIText.size() - 1)->setPosition(UIBars.at(UIBars.size() - 1)->getPosition() + sf::Vector2f((UIBars.at(UIBars.size() - 1)->getSize().x / 2) - (UIText.at(UIText.size() - 1)->getLocalBounds().width/2), (UIBars.at(UIBars.size() - 1)->getSize().y / 3)));
		UIText.at(UIText.size() - 1)->setFillColor(sf::Color(0,0,0,255));
		
		Button1.push_back(Button());
		Button1.at(Button1.size()-1).setPosition(unitScreen.at(unitScreen.size() - 1)->getPosition() + sf::Vector2f((unitScreen.at(unitScreen.size() - 1)->getSize().x / 20), unitScreen.at(unitScreen.size() - 1)->getSize().x / 20));
		Button1.at(Button1.size() - 1).setTexture(m_Textures.getTexture(9));
		Button1.at(Button1.size() - 1).setSize(sf::Vector2f(unitScreen.at(unitScreen.size() - 1)->getSize().y/4, unitScreen.at(unitScreen.size() - 1)->getSize().y / 4));
	}

	//Reduces the amounts of edges for each wall by combining adjacent edges and removing unreachable edges.
	std::vector<float> XValues;
	std::vector<float> YValues;
	std::vector<std::vector<sf::Vector2f>> ComparissonVectsX;
	std::vector<std::vector<sf::Vector2f>> ComparissonVectsY;
	bool bFoundMatch = false;

	//For every edge
	for (int i = 0; i < m_vEdgesX.size(); i++)
	{
		bFoundMatch = false;
		if (XValues.size() == 0) //If there are no current x values add one
		{
			XValues.push_back(m_vEdgesX.at(i).x);
		}
		else
		{
			for (int j = 0; j < XValues.size(); j++) //For every current x value found
			{
				if (m_vEdgesX.at(i).x == XValues.at(j)) //If the x matches to the current Xs
				{
					//A match has been found
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the xs match add the x to the list of xs
			{
				XValues.push_back(m_vEdgesX.at(i).x);
			}
		}
	}

	for (int i = 0; i < m_vEdgesY.size(); i++)
	{
		bFoundMatch = false;
		if (YValues.size() == 0) //If there are no current x values add one
		{
			YValues.push_back(m_vEdgesY.at(i).y);
		}
		else
		{
			for (int j = 0; j < YValues.size(); j++) //For every current x value found
			{
				if (m_vEdgesY.at(i).y == YValues.at(j)) //If the x matches to the current Xs
				{
					//A match has been found
					bFoundMatch = true;
				}
			}
			if (!bFoundMatch) //If none of the xs match add the x to the list of xs
			{
				YValues.push_back(m_vEdgesY.at(i).y);
			}
		}
	}
	ComparissonVectsX.resize(XValues.size());
	for (int i = 0; i < XValues.size(); i++)
	{
		for (int j = 0; j < m_vEdgesX.size(); j++)
		{
			if (m_vEdgesX.at(j).x == XValues.at(i)) //If the x matches to the current Xs
			{
				ComparissonVectsX.at(i).push_back(m_vEdgesX.at(j));
			}
		}
	}

	ComparissonVectsY.resize(YValues.size());
	for (int i = 0; i < YValues.size(); i++)
	{
		for (int j = 0; j < m_vEdgesY.size(); j++)
		{
			if (m_vEdgesY.at(j).y == YValues.at(i)) //If the x matches to the current Xs
			{
				ComparissonVectsY.at(i).push_back(m_vEdgesY.at(j));
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

	m_vEdges = Edges;
	m_EdgeLines.resize(m_vEdges.size());
	m_EdgeLines.setPrimitiveType(sf::Lines);

	for (int i = 0; i < m_vEdges.size(); i++)
	{
		m_EdgeLines[i] = sf::Vertex(m_vEdges.at(i), sf::Color(255,0,0,255));
	}

	//Sets up the pathfinder
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
		UIBars.at(i * 2)->setLevel(m_vUnits.at(i)->getHealthData().x);
		UIBars.at(i * 2)->setLimit(m_vUnits.at(i)->getHealthData().y);

		//Ammo
		UIBars.at((i * 2) + 1)->setLevel(m_vUnits.at(i)->getAmmoData().x);
		UIBars.at((i * 2) + 1)->setLimit(m_vUnits.at(i)->getAmmoData().y);

		//Updates the UI Text
		//Health
		UIText.at(i * 2)->setString(std::to_string((int)m_vUnits.at(i)->getHealthData().x) + "/" + std::to_string((int)m_vUnits.at(i)->getHealthData().y));

		//Ammo
		if (m_vUnits.at(i)->reloading())
		{
			UIText.at((i * 2) + 1)->setString("Reloading");
		}
		else
		{
			UIText.at((i * 2) + 1)->setString(std::to_string((int)m_vUnits.at(i)->getAmmoData().x) + "/" + std::to_string((int)m_vUnits.at(i)->getAmmoData().y));
		}

		//Centers the text onto the bar its displaying data for
		UIText.at(i * 2)->setPosition(UIBars.at(i * 2)->getPosition() + sf::Vector2f((UIBars.at(i * 2)->getSize().x / 2) - (UIText.at(i * 2)->getLocalBounds().width / 2), 0));
		UIText.at((i * 2) + 1)->setPosition(UIBars.at((i * 2) + 1)->getPosition() + sf::Vector2f((UIBars.at((i * 2) + 1)->getSize().x / 2) - (UIText.at((i * 2) + 1)->getLocalBounds().width / 2), 0));
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
					if (vCharSet2.at(j)->getHealthData().x > 0)
					{
						vCharSet1.at(i)->setTarget(vCharSet2.at(j));
					}
				}

				//Take health off if a bullet has been shot
				vCharSet2.at(j)->setHealth(vCharSet2.at(j)->getHealthData().x - vCharSet1.at(i)->bulletChecks({ vCharSet2.at(j)->getCollisionLine(vCharSet1.at(i)->getRotation()).at(0),
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
	for (int i = 0; i < Button1.size(); i++)
	{
		//If a Class button is pressed
		if (Button1.at(i).clicked(mousePos))
		{
			//Switches the class of the unit
			switch (m_vUnits.at(i)->getClass())
			{
			case Sniper:
				m_vUnits.at(i)->setClass(Support, m_Textures.getTexture(5));
				Button1.at(i).setTexture(m_Textures.getTexture(8));
				break;
			case Support:
				m_vUnits.at(i)->setClass(Shotgunner, m_Textures.getTexture(6));
				Button1.at(i).setTexture(m_Textures.getTexture(9));
				break;
			case Shotgunner:
				m_vUnits.at(i)->setClass(Assault, m_Textures.getTexture(7));
				Button1.at(i).setTexture(m_Textures.getTexture(10));
				break;
			case Assault:
				m_vUnits.at(i)->setClass(Sniper, m_Textures.getTexture(4));
				Button1.at(i).setTexture(m_Textures.getTexture(3));
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

	//Draws the Unit UI
	for (int i = 0; i < unitScreen.size(); i++)
	{
		target.draw(*unitScreen.at(i));
	}
	for (int i = 0; i < UIBars.size(); i++)
	{
		target.draw(*UIBars.at(i));
	}
	for (int i = 0; i < Icons.size(); i++)
	{
		target.draw(Icons.at(i));
	}
	for (int i = 0; i < UIText.size(); i++)
	{
		target.draw(*UIText.at(i));
	}
	for (int i = 0; i < Button1.size(); i++)
	{
		target.draw(Button1.at(i));
	}

	//Draws the UI
	target.draw(m_Toolbar);
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
}