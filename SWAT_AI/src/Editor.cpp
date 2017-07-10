#include "../include/Editor.h"

Editor::Editor(sf::Vector2u windowSize)
{
	//Links the Asset managers
	m_CurrentMap = Map::getInstance();
	m_CurrentSettings = Settings::getInstance();
	m_Textures = TextureLoader::getInstance();
	m_Fonts = FontManager::getInstance();

	//Sets up the toolbar 
	m_Toolbar.setPosition(0, 0);
	m_Toolbar.setFillColor(sf::Color(70, 70, 70, 255));
	m_Toolbar.setSize(sf::Vector2f(windowSize.x, windowSize.y / 20));

	//Sets up the map window
	m_CurrentMap->setup(
		sf::FloatRect(sf::Vector2f(0, m_Toolbar.getSize().y), sf::Vector2f(windowSize) - sf::Vector2f(windowSize.x / 3, m_Toolbar.getSize().y)),
		sf::Vector2f(20, 20)
	);

	//Enables debug mode for the grid
	m_CurrentSettings->setDebug(true);

	//Sets up the exit button
	m_ExitButton = new Button();
	m_ExitButton->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
	m_ExitButton->setPosition(sf::Vector2f(m_Toolbar.getPosition().x + m_Toolbar.getSize().x - m_ExitButton->getSize().x, m_Toolbar.getPosition().y));
	m_ExitButton->setTexture(m_Textures->getTexture(20));
	m_ExitButton->setBackgroundColor(sf::Color(70, 70, 70, 255));

	//Sets up the save button
	m_SaveButton = new Button();
	m_SaveButton->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
	m_SaveButton->setPosition(sf::Vector2f(m_Toolbar.getPosition().x + (m_Toolbar.getSize().x/2) - (m_SaveButton->getSize().x/2), m_Toolbar.getPosition().y));
	m_SaveButton->setTexture(m_Textures->getTexture(31));
	m_SaveButton->setBackgroundColor(sf::Color(70, 70, 70, 255));

	//Sets up the sidebar containing all of the buttons
	m_Sidebar.setPosition(m_CurrentMap->getWindowSize().x, m_Toolbar.getSize().y);
	m_Sidebar.setFillColor(sf::Color(120, 120, 120, 255));
	m_Sidebar.setSize(sf::Vector2f(windowSize.x - m_CurrentMap->getWindowSize().x, windowSize.y - m_Toolbar.getSize().y));

	// Creates 4 new buttons for grid resizing
	for (int i = 0; i < 4; i++)
	{
		m_vGridButtons.push_back(new Button);
	}
	 
	float fBorderSize = m_Sidebar.getSize().x / 20.0f; //Gap between the edge of the sidebar and the content inside
	float fContentGap = m_Sidebar.getSize().y / 20.0f; //The gap between sets of content in the y axis

	//Labels for the tools
	m_vUIText.resize(5);
	m_vUIText.at(0).setString("Grid X: ");
	m_vUIText.at(1).setString("Grid Y: ");
	m_vUIText.at(2).setString("Objects: ");
	m_vUIText.at(3).setString("Floor Tiles: ");
	m_vUIText.at(4).setString("Tools: ");

	//For every label
	for (int i = 0; i < m_vUIText.size(); i++)
	{
		//Setup the text sizing
		m_vUIText.at(i).setFont(*m_Fonts->getFont(0));
		m_vUIText.at(i).setCharacterSize(100);
		while (m_vUIText.at(i).getLocalBounds().width > m_Sidebar.getSize().x / 4)
		{
			m_vUIText.at(i).setCharacterSize(m_vUIText.at(i).getCharacterSize()-1);
		}

		//Sets the position of the text to be incrementally further down the sidebar
		float fYOffset = 0.0f;
		if (i > 0)
		{
			fYOffset = m_vUIText.at(i - 1).getLocalBounds().height;
		}
		m_vUIText.at(i).setPosition(m_Sidebar.getPosition().x + fBorderSize, m_Sidebar.getPosition().y + ((i + 1) * fYOffset) + fBorderSize);
		m_vUIText.at(i).setFillColor(sf::Color::White);
	}

	//For the grid buttons
	for (int i = 0; i < m_vGridButtons.size(); i++)
	{
		//Set up their main data
		m_vGridButtons.at(i)->setSize(sf::Vector2f(m_vUIText.at(i/2).getLocalBounds().height, m_vUIText.at(i / 2).getLocalBounds().height));
		m_vGridButtons.at(i)->setPosition( sf::Vector2f(m_vUIText.at(i / 2).getPosition().x + m_vUIText.at(i / 2).getLocalBounds().width, m_vUIText.at(i / 2).getPosition().y + (m_vUIText.at(i / 2).getLocalBounds().height / 2)));
		m_vGridButtons.at(i)->setBackgroundColor(sf::Color(70, 70, 70, 255));

		//If theyre a second button then shift them to the right and change the texture
		if (i % 2 == 0)
		{
			m_vGridButtons.at(i)->setTexture(m_Textures->getTexture(21));
			m_vGridButtons.at(i)->setPosition(sf::Vector2f(m_vGridButtons.at(i)->getPosition().x + m_vGridButtons.at(i)->getSize().x + (m_vGridButtons.at(i)->getSize().x / 2), m_vGridButtons.at(i)->getPosition().y ));
		}
		else
		{
			m_vGridButtons.at(i)->setTexture(m_Textures->getTexture(22));
		}
	}

	//Sets up the levelbits
	m_vcLevelBits.clear();
	m_vcLevelBits.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().y);
	}

	//Sets up the map objects array
	m_vItems.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_vItems.size(); i++)
	{
		m_vItems.at(i).resize(m_CurrentMap->getGridDims().y);
	}

	//Sets up the floor bits and fills them with grass tiles
	m_vcFloorBits.clear();
	m_vcFloorBits.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_vcFloorBits.size(); i++)
	{
		m_vcFloorBits.at(i).resize(m_CurrentMap->getGridDims().y);
	}
	for (int i = 0; i < m_vcFloorBits.size(); i++)
	{
		for (int j = 0; j < m_vcFloorBits.at(i).size(); j++)
		{
			m_vcFloorBits.at(i).at(j) = 'G';
		}
	}

	//Sets up the floor tiles and fills them with grass tiles
	m_vFloorTiles.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_vFloorTiles.size(); i++)
	{
		m_vFloorTiles.at(i).resize(m_CurrentMap->getGridDims().y);
	}
	for (int i = 0; i < m_vFloorTiles.size(); i++)
	{
		for (int j = 0; j < m_vFloorTiles.at(i).size(); j++)
		{
			m_vFloorTiles.at(i).at(j) = new Object();
			m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(14));
		}
	}

	//Sets the position of the label to be below the previous content and creates buttons underneath
	//Objects
	m_vUIText.at(2).setPosition(m_vUIText.at(2).getPosition().x, m_vGridButtons.at(m_vGridButtons.size() - 1)->getPosition().y + m_vGridButtons.at(m_vGridButtons.size() - 1)->getSize().y + fContentGap);
	createButtons(&m_vObjectButtons, 2, std::vector<int>{0,1,2,24,30});

	//Floor objects
	m_vUIText.at(3).setPosition(m_vUIText.at(2).getPosition().x, m_vObjectButtons.at(m_vObjectButtons.size() - 1)->getPosition().y + m_vObjectButtons.at(m_vObjectButtons.size() - 1)->getSize().y + fContentGap);
	createButtons(&m_vFloorButtons, 3, std::vector<int>{14, 25, 26, 27, 28, 29});

	//Tools
	m_vUIText.at(4).setPosition(m_vUIText.at(3).getPosition().x, m_vFloorButtons.at(m_vFloorButtons.size() - 1)->getPosition().y + m_vFloorButtons.at(m_vFloorButtons.size() - 1)->getSize().y + fContentGap);
	createButtons(&m_vToolButtons, 4, std::vector<int>{23, 33, 34});

	//Sets the starting tool to the wooden wall tool
	m_cCurrentTool = ' ';

	//Starts the editing on the main objects
	m_Editing = ObjectEdit;

	//Sets up the path editor tools
	Selector.setOutlineThickness(3);
	Selector.setOutlineColor(sf::Color(0,255,0,255));
	Selector.setFillColor(sf::Color(0, 255, 0, 0));
	Selector.setSize(m_CurrentMap->getTileSize());
	PathLine.setPrimitiveType(sf::LinesStrip);

	loadMap(" ");
}

void Editor::createButtons(std::vector<Button*>* vButtonSet, int iUIindex,  std::vector<int> iTexIndex)
{
	float fBorderSize = m_Sidebar.getSize().x / 20.0f; //Gap between the edge of the sidebar and the content inside
	float fContentGap = m_Sidebar.getSize().y / 20.0f; //The gap between sets of content in the y axis

	if (vButtonSet != NULL) //If there is a set of buttons
	{
		vButtonSet->resize(iTexIndex.size()); //Resize the button count

		//The buttons under the label are then created 
		for (int i = 0; i < vButtonSet->size(); i++)
		{
			vButtonSet->at(i) = new Button();
			vButtonSet->at(i)->setSize(m_vGridButtons.at(0)->getSize() * 2.0f);
			vButtonSet->at(i)->setBackgroundColor(sf::Color(70, 70, 70, 255));

			//They are then positioned to be beneath the label to act in a tight grid fashion
			float fXOffset = 0.0f;
			float fYOffset = 0.0f;
			if (i > 0)
			{
				//Once the buttons start to leave the screen a new row is created
				fXOffset = vButtonSet->at(i - 1)->getSize().x;
				if (vButtonSet->at(i - 1)->getPosition().x + vButtonSet->at(i - 1)->getSize().x + vButtonSet->at(i)->getSize().x > m_Sidebar.getPosition().x + m_Sidebar.getSize().x - fBorderSize)
				{
					fXOffset = 0.0f;
					fYOffset += vButtonSet->at(i - 1)->getSize().y;
				}

			}
			vButtonSet->at(i)->setPosition(sf::Vector2f(m_Sidebar.getPosition().x + fBorderSize + (i* fXOffset), m_vUIText.at(iUIindex).getPosition().y + m_vUIText.at(iUIindex).getLocalBounds().height + fContentGap + fYOffset));
		}

		//Set the button textures
		for (int i = 0; i < iTexIndex.size(); i++)
		{
			vButtonSet->at(i)->setTexture(m_Textures->getTexture(iTexIndex.at(i)));
		}
	}
}

void Editor::updatePathUI()
{
	//Clear the path UI
	m_vEnemyCircles.clear();
	PathLine.clear();

	//IF an enemy has been selected
	if (iSelectedEnemy != NULL)
	{
		//Create Path UI
		PathLine.resize(iSelectedEnemy->path.size());
		for (int i = 0; i < iSelectedEnemy->path.size(); i++)
		{
			sf::CircleShape tempCircle;
			tempCircle.setFillColor(sf::Color::Red);
			tempCircle.setRadius(std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) / 5.0f);
			tempCircle.setOrigin(tempCircle.getRadius(), tempCircle.getRadius());
			tempCircle.setPosition(m_CurrentMap->getPosition() + sf::Vector2f(iSelectedEnemy->path.at(i).x * m_CurrentMap->getTileSize().x, iSelectedEnemy->path.at(i).y * m_CurrentMap->getTileSize().y) + (m_CurrentMap->getTileSize() / 2.0f));
			m_vEnemyCircles.push_back(tempCircle);

			PathLine[i].position = m_vEnemyCircles.at(i).getPosition();
			PathLine[i].color = sf::Color::Green;
		}

		//Move the selector to the enemy position
		Selector.setSize(m_CurrentMap->getTileSize());
		Selector.setPosition(m_CurrentMap->getPosition() + sf::Vector2f(iSelectedEnemy->position.x * m_CurrentMap->getTileSize().x, iSelectedEnemy->position.y * m_CurrentMap->getTileSize().y));
	}
	else
	{
		//Move the selector to the default location
		Selector.setSize(m_CurrentMap->getTileSize());
		Selector.setPosition(sf::Vector2f(0,0));
	}
}


void Editor::update(sf::Vector2i mousePos)
{
	//Performs hover checks for all of the buttons for highlighting
	m_ExitButton->update(mousePos);
	m_SaveButton->update(mousePos);
	for (int i = 0; i < m_vGridButtons.size(); i++)
	{
		m_vGridButtons.at(i)->update(mousePos);
	}
	for (int i = 0; i < m_vObjectButtons.size(); i++)
	{
		m_vObjectButtons.at(i)->update(mousePos);
	}
	for (int i = 0; i < m_vToolButtons.size(); i++)
	{
		m_vToolButtons.at(i)->update(mousePos);
	}
	for (int i = 0; i < m_vFloorButtons.size(); i++)
	{
		m_vFloorButtons.at(i)->update(mousePos);
	}

	//Positions all items that exist within the world objects at their respective grid space
	for (int i = 0; i < m_vItems.size(); i++)
	{
		for (int j = 0; j < m_vItems.at(i).size(); j++)
		{
			if (m_vItems.at(i).at(j) != NULL)
			{
				m_vItems.at(i).at(j)->setSize(m_CurrentMap->getTileSize());
				m_vItems.at(i).at(j)->setPosition(sf::Vector2f((j * m_CurrentMap->getTileSize().x) + m_CurrentMap->getPosition().x, (i * m_CurrentMap->getTileSize().y) + m_CurrentMap->getPosition().y));
			}
		}
	}

	//Positions all floor items that exist within the world at their respective grid space
	for (int i = 0; i < m_vFloorTiles.size(); i++)
	{
		for (int j = 0; j < m_vFloorTiles.at(i).size(); j++)
		{
			if (m_vFloorTiles.at(i).at(j) != NULL)
			{
				m_vFloorTiles.at(i).at(j)->setSize(m_CurrentMap->getTileSize());
				m_vFloorTiles.at(i).at(j)->setPosition(sf::Vector2f((j * m_CurrentMap->getTileSize().x) + m_CurrentMap->getPosition().x, (i * m_CurrentMap->getTileSize().y) + m_CurrentMap->getPosition().y));
			}
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//If the mouse is within the bounds of the grid
		if (mousePos.x > m_CurrentMap->getPosition().x &&
			mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
			mousePos.y > m_CurrentMap->getPosition().y &&
			mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
		{
			//Calculate the mouses position on the grid
			sf::Vector2i gridPos(0, 0);
			gridPos.x = (mousePos.x - m_CurrentMap->getPosition().x) / m_CurrentMap->getTileSize().x;
			gridPos.y = (mousePos.y - m_CurrentMap->getPosition().y) / m_CurrentMap->getTileSize().y;
			EnemyData newEnemy;

			switch (m_Editing)
			{
				case ObjectEdit:
					if (m_vcLevelBits.at(gridPos.y).at(gridPos.x) != m_cCurrentTool)
					{
					//If the tool is valid
						if (m_cCurrentTool == 'W' || m_cCurrentTool == 'P' || m_cCurrentTool == 'E' ||
							m_cCurrentTool == 'D' || m_cCurrentTool == 'B' || m_cCurrentTool == ' ')
						{
							//If there is an item currently in that location clear the pointer
							if (m_vItems.at(gridPos.y).at(gridPos.x) != NULL)
							{
								delete(m_vItems.at(gridPos.y).at(gridPos.x));
								m_vItems.at(gridPos.y).at(gridPos.x) = NULL;
							}
							//If the tool isnt an eraser create a new object
							if (m_cCurrentTool != ' ')
							{
								m_vItems.at(gridPos.y).at(gridPos.x) = new Object();

								//Give the object the following texture
								switch (m_cCurrentTool)
								{
								case 'W':
									m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(0));
									break;
								case 'P':
									m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(1));
									break;
								case 'E':
									m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(2));
									newEnemy.position = gridPos;
									m_vEnemyPaths.push_back(newEnemy);
									break;
								case 'D':
									m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(24));
									break;
								case 'B':
									m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(30));
									break;
								}
							}
							else
							{
								if (m_vcLevelBits.at(gridPos.y).at(gridPos.x) == 'E')
								{
									for (int i = 0; i < m_vEnemyPaths.size(); i++)
									{
										if (gridPos == m_vEnemyPaths.at(i).position)
										{
											m_vEnemyPaths.erase(m_vEnemyPaths.begin() + i);
											iSelectedEnemy = NULL;
											updatePathUI();
										}
									}
								}
							}

							//Set the main map bit to the current tool letter
							m_vcLevelBits.at(gridPos.y).at(gridPos.x) = m_cCurrentTool;
						}
					}
					break;

				case FloorEdit:
					if (m_vcFloorBits.at(gridPos.y).at(gridPos.x) != m_cCurrentTool)
					{
						//If the tool is valid
						if (m_cCurrentTool == 'G' || m_cCurrentTool == 'F' || m_cCurrentTool == 'K' ||
							m_cCurrentTool == 'C' || m_cCurrentTool == 'B' || m_cCurrentTool == 'R' ||
							m_cCurrentTool == ' ')
						{
							//If there is an item currently in that location clear the pointer
							if (m_vFloorTiles.at(gridPos.y).at(gridPos.x) != NULL)
							{
								delete(m_vFloorTiles.at(gridPos.y).at(gridPos.x));
								m_vFloorTiles.at(gridPos.y).at(gridPos.x) = NULL;
							}

							//If the tool isnt an eraser create a new object
							if (m_cCurrentTool != ' ')
							{
								m_vFloorTiles.at(gridPos.y).at(gridPos.x) = new Object();

								//Give the object the following texture
								switch (m_cCurrentTool)
								{
								case 'G':
									m_vFloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(14));
									break;
								case 'F':
									m_vFloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(25));
									break;
								case 'K':
									m_vFloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(27));
									break;
								case 'C':
									m_vFloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(26));
									break;
								case 'B':
									m_vFloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(28));
									break;
								case 'R':
									m_vFloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(29));
									break;
								}
							}

							//Set the floor map bit to the current tool letter
							m_vcFloorBits.at(gridPos.y).at(gridPos.x) = m_cCurrentTool;
						}
					}
					break;
			}
		}
	}
}

int Editor::processInput(sf::Event keyCode, sf::Vector2i mousePos)
{
	if (keyCode.type == sf::Event::MouseButtonPressed)
	{
		switch (keyCode.key.code)
		{
		case sf::Mouse::Left:
			//If the exit button is clicked return to the menu
			if (m_ExitButton->hovering(mousePos))
			{
				m_CurrentSettings->setDebug(false);
				return S_Menu;
			}

			//If the save button is clicked save the game
			if (m_SaveButton->hovering(mousePos))
			{
				saveMap();
			}

			//If the + X Grid button is pressed
			if (m_vGridButtons.at(0)->hovering(mousePos))
			{
				//Increment the dimensions
				m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x + 1, m_CurrentMap->getGridDims().y));

				//Increment every row of the map by adding a new item
				for (int i = 0; i < m_vcLevelBits.size(); i++)
				{
					m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().x);
					m_vItems.at(i).resize(m_CurrentMap->getGridDims().x);
				}

				//Increment every row of the floor map by adding a new item
				for (int i = 0; i < m_vcFloorBits.size(); i++)
				{
					m_vcFloorBits.at(i).resize(m_CurrentMap->getGridDims().x);
					m_vFloorTiles.at(i).resize(m_CurrentMap->getGridDims().x);
				}
				updatePathUI();
			}

			//If the - X Grid button is pressed
			if (m_vGridButtons.at(1)->hovering(mousePos))
			{
				//Decrement the dimensions
				m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x - 1, m_CurrentMap->getGridDims().y));

				//Check if any path nodes are off the screen
				for (int i = 0; i < m_vEnemyPaths.size(); i++)
				{
					for (int j = m_vEnemyPaths.at(i).path.size() - 1; j >= 0; j--)
					{
						if (m_vEnemyPaths.at(i).path.at(j).x >= m_CurrentMap->getGridDims().x)  //If they are
						{
							m_vEnemyPaths.at(i).path.erase(m_vEnemyPaths.at(i).path.begin() + j); //Erase them
						}
					}
				}

				//Decrement the LevelBits vector
				for (int i = 0; i < m_vcLevelBits.size(); i++)
				{
					if (m_vcLevelBits.at(i).at(m_vcLevelBits.at(i).size() - 1) == 'E')
					{
						for (int j = 0; j < m_vEnemyPaths.size(); j++)
						{
							if (sf::Vector2i(m_vcLevelBits.at(i).size() - 1, i) == m_vEnemyPaths.at(j).position)
							{
								//If the enemy is removed delete the associated enemy path
								m_vEnemyPaths.erase(m_vEnemyPaths.begin() + j);
								iSelectedEnemy = NULL;
							}
						}
					}
					m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().x);
				}
				updatePathUI();

				//For the objects the pointers are cleared at the end of each row.
				for (int i = 0; i < m_vItems.size(); i++)
				{
					if (m_vItems.at(i).at(m_vItems.at(i).size() - 1) != NULL)
					{
						delete(m_vItems.at(i).at(m_vItems.at(i).size() - 1));
						m_vItems.at(i).at(m_vItems.at(i).size() - 1) = NULL;
					}
					m_vItems.at(i).resize(m_CurrentMap->getGridDims().x);
				}

				//Decrement the FloorBits vector
				for (int i = 0; i < m_vcFloorBits.size(); i++)
				{
					m_vcFloorBits.at(i).resize(m_CurrentMap->getGridDims().x);
				}

				//For the objects the pointers are cleared at the end of each row.
				for (int i = 0; i < m_vFloorTiles.size(); i++)
				{
					if (m_vFloorTiles.at(i).at(m_vFloorTiles.at(i).size() - 1) != NULL)
					{
						delete(m_vFloorTiles.at(i).at(m_vFloorTiles.at(i).size() - 1));
						m_vFloorTiles.at(i).at(m_vFloorTiles.at(i).size() - 1) = NULL;
					}
					m_vFloorTiles.at(i).resize(m_CurrentMap->getGridDims().x);
				}
			}

			//If the + Y Grid button is pressed
			if (m_vGridButtons.at(2)->hovering(mousePos))
			{
				//Increment the dimensions
				m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y + 1));

				//A new row is added to each vector
				//LevelBits
				m_vcLevelBits.resize(m_CurrentMap->getGridDims().y);
				m_vcLevelBits.at(m_vcLevelBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

				//Map objects
				m_vItems.resize(m_CurrentMap->getGridDims().y);
				m_vItems.at(m_vItems.size() - 1).resize(m_CurrentMap->getGridDims().x);

				//FloorBits
				m_vcFloorBits.resize(m_CurrentMap->getGridDims().y);
				m_vcFloorBits.at(m_vcFloorBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

				//Floor objects
				m_vFloorTiles.resize(m_CurrentMap->getGridDims().y);
				m_vFloorTiles.at(m_vFloorTiles.size() - 1).resize(m_CurrentMap->getGridDims().x);
				updatePathUI();
			}

			//If the - Y Grid button is pressed
			if (m_vGridButtons.at(3)->hovering(mousePos))
			{
				//Decrement the dimensions
				m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y - 1));

				//Check if any path nodes are off the screen
				for (int i = 0; i < m_vEnemyPaths.size(); i++)
				{
					for (int j = m_vEnemyPaths.at(i).path.size() - 1; j >= 0; j--)
					{
						if (m_vEnemyPaths.at(i).path.at(j).y >= m_CurrentMap->getGridDims().y) //If they are
						{
							m_vEnemyPaths.at(i).path.erase(m_vEnemyPaths.at(i).path.begin() + j); //Erase them
						}
					}
				}

				//Decrement the LevelBits vector
				for (int i = 0; i < m_vcLevelBits.at(m_vcLevelBits.size() - 1).size(); i++)
				{
					if (m_vcLevelBits.at(m_vcLevelBits.size() - 1).at(i) == 'E')
					{
						for (int j = 0; j < m_vEnemyPaths.size(); j++)
						{
							if (sf::Vector2i(i, m_vcLevelBits.size() - 1) == m_vEnemyPaths.at(j).position)
							{
								//If the enemy is removed delete the associated enemy path
								m_vEnemyPaths.erase(m_vEnemyPaths.begin() + j);
								iSelectedEnemy = NULL;
							}
						}
					}
				}
				updatePathUI();

				//Decrement the levelBits vector
				m_vcLevelBits.resize(m_CurrentMap->getGridDims().y);

				//Clear the pointers of all objects on the bottom row and decrement the row
				for (int i = 0; i < m_vItems.at(m_vItems.size() - 1).size(); i++)
				{
					if (m_vItems.at(m_vItems.size() - 1).at(i) != NULL)
					{
						delete(m_vItems.at(m_vItems.size() - 1).at(i));
						m_vItems.at(m_vItems.size() - 1).at(i) = NULL;
					}
				}
				m_vItems.resize(m_CurrentMap->getGridDims().y);

				//Decrement the floorBits vector
				m_vcFloorBits.resize(m_CurrentMap->getGridDims().y);

				//Clear the pointers of all objects on the bottom row and decrement the row
				for (int i = 0; i < m_vFloorTiles.at(m_vItems.size() - 1).size(); i++)
				{
					if (m_vFloorTiles.at(m_vFloorTiles.size() - 1).at(i) != NULL)
					{
						delete(m_vFloorTiles.at(m_vFloorTiles.size() - 1).at(i));
						m_vFloorTiles.at(m_vFloorTiles.size() - 1).at(i) = NULL;
					}
				}
				m_vFloorTiles.resize(m_CurrentMap->getGridDims().y);
			}

			//If the mouse is within the bounds of the grid
			if (mousePos.x > m_CurrentMap->getPosition().x &&
				mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
				mousePos.y > m_CurrentMap->getPosition().y &&
				mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
			{
				//Calculate the mouses position on the grid
				sf::Vector2i gridPos((mousePos.x - m_CurrentMap->getPosition().x) / m_CurrentMap->getTileSize().x,
					(mousePos.y - m_CurrentMap->getPosition().y) / m_CurrentMap->getTileSize().y);

				if (m_Editing == CharacterEdit)
				{
					switch (m_cCurrentTool)
					{
						//Eraser
					case ' ':
						//Check each path node of the selected character
						for (int j = 0; j < iSelectedEnemy->path.size(); j++)
						{
							if (iSelectedEnemy->path.at(j) == gridPos) //If clicked
							{
								//Remove the path node
								iSelectedEnemy->path.erase(iSelectedEnemy->path.begin() + j);
								updatePathUI();

								//Stop searching
								j = iSelectedEnemy->path.size();
							}
						}
						break;

						//Selector
					case 'S':
						//If the selected area has an enemy in it
						if (m_vcLevelBits.at(gridPos.y).at(gridPos.x) == 'E')
						{
							//Find the enemy path vector
							for (int i = 0; i < m_vEnemyPaths.size(); i++)
							{
								if (gridPos == m_vEnemyPaths.at(i).position)
								{
									//Set it to be the selected enemy
									iSelectedEnemy = &m_vEnemyPaths.at(i);
									updatePathUI();
								}
							}
						}
						break;

						//Path node placement
					case 'P':
						//If there is a selected enemy
						if (iSelectedEnemy != NULL)
						{
							//Add a path node
							iSelectedEnemy->path.push_back(gridPos);
							updatePathUI();
						}
						break;
					}
				}
			}

			//If an object button is being hovered over set the tool to the value of the tool
			if (m_vObjectButtons.at(0)->hovering(mousePos)) //Wooden Wall
			{
				m_cCurrentTool = 'W';
				m_Editing = ObjectEdit;
			}
			else if (m_vObjectButtons.at(1)->hovering(mousePos)) //Friendly unit
			{
				m_cCurrentTool = 'P';
				m_Editing = ObjectEdit;
			}
			else if (m_vObjectButtons.at(2)->hovering(mousePos)) //Enemy unit
			{
				m_cCurrentTool = 'E';
				m_Editing = ObjectEdit;
			}
			else if (m_vObjectButtons.at(3)->hovering(mousePos)) //Door
			{
				m_cCurrentTool = 'D';
				m_Editing = ObjectEdit;
			}
			else if (m_vObjectButtons.at(4)->hovering(mousePos)) //Brick wall
			{
				m_cCurrentTool = 'B';
				m_Editing = ObjectEdit;
			}

			if (m_vToolButtons.at(0)->hovering(mousePos)) //Eraser
			{
				m_cCurrentTool = ' ';
			}
			else if (m_vToolButtons.at(1)->hovering(mousePos)) //Eraser
			{
				m_cCurrentTool = 'S';
				m_Editing = CharacterEdit;
			}
			else if (m_vToolButtons.at(2)->hovering(mousePos)) //Eraser
			{
				m_cCurrentTool = 'P';
				m_Editing = CharacterEdit;
			}

			if (m_vFloorButtons.at(0)->hovering(mousePos)) //Grass
			{
				m_cCurrentTool = 'G';
				m_Editing = FloorEdit;
			}
			else if (m_vFloorButtons.at(1)->hovering(mousePos)) //Wooden floor
			{
				m_cCurrentTool = 'F';
				m_Editing = FloorEdit;
			}
			else if (m_vFloorButtons.at(2)->hovering(mousePos)) //Concrete
			{
				m_cCurrentTool = 'C';
				m_Editing = FloorEdit;
			}
			else if (m_vFloorButtons.at(3)->hovering(mousePos)) //Kitchen tile
			{
				m_cCurrentTool = 'K';
				m_Editing = FloorEdit;
			}
			else if (m_vFloorButtons.at(4)->hovering(mousePos)) //Blue carpet
			{
				m_cCurrentTool = 'B';
				m_Editing = FloorEdit;
			}
			else if (m_vFloorButtons.at(5)->hovering(mousePos)) //Red carpet
			{
				m_cCurrentTool = 'R';
				m_Editing = FloorEdit;
			}
			break;
		}
	}

	return S_None;
}

void Editor::saveMap()
{
	//Saves the object map 
	std::ofstream File1;
	File1.open("Assets/Maps/CustomMap.txt", std::ios::out);
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		//For bits in row write bits
		for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
		{
			File1 << m_vcLevelBits.at(i).at(j);
		}
		//If not the last row add a new row
		if (i != m_vcLevelBits.size() - 1)
		{
			File1 << "\n";
		}
	}
	File1.close();

	//Saves the floor map 
	File1.open("Assets/Maps/CustomMapFloor.txt", std::ios::out);
	for (int i = 0; i < m_vcFloorBits.size(); i++)
	{
		//For bits in row write bits
		for (int j = 0; j < m_vcFloorBits.at(i).size(); j++)
		{
			File1 << m_vcFloorBits.at(i).at(j);
		}
		//If not the last row add a new row
		if (i != m_vcFloorBits.size() - 1)
		{
			File1 << "\n";
		}
	}
	File1.close();

	//Saves the paths
	File1.open("Assets/Maps/CustomMapPaths.txt", std::ios::out);
	int iEnemiesSaved = 0;

	//Look for enemies within the map
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
		{
			if (m_vcLevelBits.at(i).at(j) == 'E') //If an enemy is found
			{
				//Find its path
				for (int k = 0; k < m_vEnemyPaths.size(); k++)
				{
					if (sf::Vector2i(j, i) == m_vEnemyPaths.at(k).position)
					{
						//Add its path to the file preceding its index
						File1 << std::to_string(k) + ":";
						for (int l = 0; l < m_vEnemyPaths.at(k).path.size(); l++)
						{
							int iPathPos = (m_vEnemyPaths.at(k).path.at(l).y * m_CurrentMap->getGridDims().x) + m_vEnemyPaths.at(k).path.at(l).x;
							File1 << " " + std::to_string(iPathPos);
						}

						iEnemiesSaved++; //Increment enemies saved
						
						//If more enemies exist add a new row
						if (iEnemiesSaved != m_vEnemyPaths.size()) 
						{
							File1 << "\n";
						}
					}
				}
			}
		}
	}
	File1.close();
}

void Editor::loadMap(std::string sDir)
{
	EnemyData newEnemy;

	//Clear the floor data
	for (int i = 0; i < m_vFloorTiles.size(); i++)
	{
		for (int j = 0; j < m_vFloorTiles.at(i).size(); j++)
		{
			if (m_vFloorTiles.at(i).at(j) != NULL)
			{
				delete(m_vFloorTiles.at(i).at(j));
				m_vFloorTiles.at(i).at(j) = NULL;
			}
		}
	}

	//Clear the item data
	for (int i = 0; i < m_vItems.size(); i++)
	{
		for (int j = 0; j < m_vItems.at(i).size(); j++)
		{
			if (m_vItems.at(i).at(j) != NULL)
			{
				delete(m_vItems.at(i).at(j));
				m_vItems.at(i).at(j) = NULL;
			}
		}
	}

	//Load the object map
	m_CurrentMap->load(ObjectMap, "./Assets/Maps/CustomMap.txt");
	m_vcLevelBits = m_CurrentMap->getMapData();

	//Check each level bit and fill in the item vector with the correct objecta
	m_vItems.resize(m_vcLevelBits.size());
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		m_vItems.at(i).resize(m_vcLevelBits.at(i).size());
		for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
		{
			if (m_vcLevelBits.at(i).at(j) == 'W' || m_vcLevelBits.at(i).at(j) == 'P' || m_vcLevelBits.at(i).at(j) == 'E' ||
				m_vcLevelBits.at(i).at(j) == 'D' || m_vcLevelBits.at(i).at(j) == 'B')
			{

				m_vItems.at(i).at(j) = new Object();
				switch (m_vcLevelBits.at(i).at(j))
				{
				case 'W':
					m_vItems.at(i).at(j)->setTexture(m_Textures->getTexture(0));
					break;
				case 'P':
					m_vItems.at(i).at(j)->setTexture(m_Textures->getTexture(1));
					break;
				case 'E':
					m_vItems.at(i).at(j)->setTexture(m_Textures->getTexture(2));
					//newEnemy.position = sf::Vector2i(j, i);
					//m_vEnemyPaths.push_back(newEnemy);
					break;
				case 'D':
					m_vItems.at(i).at(j)->setTexture(m_Textures->getTexture(24));
					break;
				case 'B':
					m_vItems.at(i).at(j)->setTexture(m_Textures->getTexture(30));
					break;
				}
			}
		}
	}

	//Load the floor map
	m_CurrentMap->load(FloorMap, "./Assets/Maps/CustomMapFloor.txt");
	m_vcFloorBits = m_CurrentMap->getFloorData();

	//Check each floor bit and fill in the floor vector with the correct objecta
	m_vFloorTiles.resize(m_vcFloorBits.size());
	for (int i = 0; i < m_vcFloorBits.size(); i++)
	{
		m_vFloorTiles.at(i).resize(m_vcFloorBits.at(i).size());
		for (int j = 0; j < m_vcFloorBits.at(i).size(); j++)
		{
			if (m_vcFloorBits.at(i).at(j) == 'G' || m_vcFloorBits.at(i).at(j) == 'F' || m_vcFloorBits.at(i).at(j) == 'K' ||
				m_vcFloorBits.at(i).at(j) == 'C' || m_vcFloorBits.at(i).at(j) == 'B' || m_vcFloorBits.at(i).at(j) == 'R' )
			{
				m_vFloorTiles.at(i).at(j) = new Object();
				switch (m_vcFloorBits.at(i).at(j))
				{
				case 'G':
					m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(14));
					break;			 	   
				case 'F':			 	   
					m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(25));
					break;			 	   
				case 'K':			 	   
					m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(27));
					break;			 	   
				case 'C':			 	  
					m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(26));
					break;			 	   
				case 'B':			 	   
					m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(28));
					break;			 	   
				case 'R':			 	   
					m_vFloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(29));
					break;
				}
			}
		}
	}
	
	//Loads paths for enemies
	std::ifstream pathFile;
	pathFile.open("Assets/Maps/CustomMapPaths.txt"); //Open the map file

	if (pathFile.is_open()) //If the file opened correctly
	{
		//Initialise reading variables
		std::string sLine;
		std::string sCurrentNum;

		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
			{
				if (m_vcLevelBits.at(i).at(j) == 'E')
				{
					if (!pathFile.eof())
					{
						getline(pathFile, sLine); //Get the next line

						if (sLine != "")
						{
							m_vEnemyPaths.push_back(EnemyData()); //Add a new enemy
							m_vEnemyPaths.at(m_vEnemyPaths.size() - 1).position = sf::Vector2i(j, i); //Set the grid position

							std::istringstream sNum(sLine);

							sNum >> sCurrentNum; //Read the index
							while (!sNum.eof())
							{
								sNum >> sCurrentNum; //Read the number

								//Translate it to x y grid cooridinates
								sf::Vector2i gridPos((((stoi(sCurrentNum) % (int)m_CurrentMap->getGridDims().x))),
									(((stoi(sCurrentNum) / (int)m_CurrentMap->getGridDims().x))));
								
								//Push it to the path vector
								m_vEnemyPaths.at(m_vEnemyPaths.size() - 1).path.push_back(gridPos);
							}
						}
					}
				}
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

void Editor::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Draw the grid
	target.draw(*m_CurrentMap);
	
	target.draw(m_Sidebar);

	//Floor Items
	for (int i = 0; i < m_vFloorTiles.size(); i++)
	{
		for (int j = 0; j < m_vFloorTiles.at(i).size(); j++)
		{
			if (m_vFloorTiles.at(i).at(j) != NULL)
			{
				target.draw(*m_vFloorTiles.at(i).at(j));
			}
		}
	}

	//Draw main items
	for (int i = 0; i < m_vItems.size(); i++)
	{
		for (int j = 0; j < m_vItems.at(i).size(); j++)
		{
			if (m_vItems.at(i).at(j) != NULL)
			{
				target.draw(*m_vItems.at(i).at(j));
			}
		}
	}

	//Draw path editing tools
	if (m_Editing == CharacterEdit)
	{
		target.draw(Selector);
		target.draw(PathLine);
		for (int i = 0; i < m_vEnemyCircles.size(); i++)
		{
			target.draw(m_vEnemyCircles.at(i));
		}
	}

	//Buttons
	for (int i = 0; i < m_vGridButtons.size(); i++)
	{
		target.draw(*m_vGridButtons.at(i));
	}
	for (int i = 0; i < m_vObjectButtons.size(); i++)
	{
		target.draw(*m_vObjectButtons.at(i));
	}
	for (int i = 0; i < m_vToolButtons.size(); i++)
	{
		target.draw(*m_vToolButtons.at(i));
	}
	for (int i = 0; i < m_vFloorButtons.size(); i++)
	{
		target.draw(*m_vFloorButtons.at(i));
	}

	//Button text
	for (int i = 0; i < m_vUIText.size(); i++)
	{
		target.draw(m_vUIText.at(i));
	}

	//Toolbar + Toolbar buttons
	target.draw(m_Toolbar);
	target.draw(*m_ExitButton);
	target.draw(*m_SaveButton);
}

Editor::~Editor()
{
	for (int i = 0; i < m_vGridButtons.size(); i++)
	{
		delete(m_vGridButtons.at(i));
		m_vGridButtons.at(i) = NULL;
	}

	for (int i = 0; i < m_vObjectButtons.size(); i++)
	{
		delete(m_vObjectButtons.at(i));
		m_vObjectButtons.at(i) = NULL;
	}
	for (int i = 0; i < m_vFloorButtons.size(); i++)
	{
		delete(m_vFloorButtons.at(i));
		m_vFloorButtons.at(i) = NULL;
	}
	for (int i = 0; i < m_vToolButtons.size(); i++)
	{
		delete(m_vToolButtons.at(i));
		m_vToolButtons.at(i) = NULL;
	}
	for (int i = 0; i < m_vFloorTiles.size(); i++)
	{
		for (int j = 0; j < m_vFloorTiles.at(i).size(); j++)
		{
			if (m_vFloorTiles.at(i).at(j) != NULL)
			{
				delete(m_vFloorTiles.at(i).at(j));
				m_vFloorTiles.at(i).at(j) = NULL;
			}
		}
	}

	for (int i = 0; i < m_vItems.size(); i++)
	{
		for (int j = 0; j < m_vItems.at(i).size(); j++)
		{
			if (m_vItems.at(i).at(j) != NULL)
			{
				delete(m_vItems.at(i).at(j));
				m_vItems.at(i).at(j) = NULL;
			}
		}
	}
	delete(m_ExitButton);
	m_ExitButton = NULL;

	delete(m_SaveButton);
	m_SaveButton = NULL;

	iSelectedEnemy = NULL;
}