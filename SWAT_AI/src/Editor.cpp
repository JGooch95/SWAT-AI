#include "../include/Editor.h"

Editor::Editor(sf::Vector2u windowSize)
{
	//Sets up the default map
	m_CurrentMap = Map::getInstance();
	m_CurrentSettings = Settings::getInstance();
	m_Textures = TextureLoader::getInstance();
	m_Fonts = FontManager::getInstance();

	//Sets up the toolbar 
	m_Toolbar.setPosition(0, 0);
	m_Toolbar.setFillColor(sf::Color(70, 70, 70, 255));
	m_Toolbar.setSize(sf::Vector2f(windowSize.x, windowSize.y / 20));

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
	m_cCurrentTool = 'W';

	//Starts the editing on the main objects
	m_Editing = ObjectEdit;
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

void Editor::update(sf::Vector2i mousePos)
{
	//Performs hover checks for all of the buttons for highlighting
	m_ExitButton->hovering(mousePos);
	m_SaveButton->hovering(mousePos);
	for (int i = 0; i < m_vGridButtons.size(); i++)
	{
		m_vGridButtons.at(i)->hovering(mousePos);
	}
	for (int i = 0; i < m_vObjectButtons.size(); i++)
	{
		m_vObjectButtons.at(i)->hovering(mousePos);
	}
	for (int i = 0; i < m_vToolButtons.size(); i++)
	{
		m_vToolButtons.at(i)->hovering(mousePos);
	}
	for (int i = 0; i < m_vFloorButtons.size(); i++)
	{
		m_vFloorButtons.at(i)->hovering(mousePos);
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
}

int Editor::clickLeft(sf::Vector2i mousePos)
{
	//If the exit button is clicked return to the menu
	if (m_ExitButton->hovering(mousePos))
	{
		m_CurrentSettings->setDebug(false);
		return 1;
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
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x+1, m_CurrentMap->getGridDims().y));

		//Increment every row of the map by adding a new item
		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}
		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_vItems.at(i).resize(m_CurrentMap->getGridDims().x);
		}

		//Increment every row of the floor map by adding a new item
		for (int i = 0; i < m_vcFloorBits.size(); i++)
		{
			m_vcFloorBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}
		for (int i = 0; i <  m_vcFloorBits.size(); i++)
		{
			m_vFloorTiles.at(i).resize(m_CurrentMap->getGridDims().x);
		}
	}

	//If the - X Grid button is pressed
	if (m_vGridButtons.at(1)->hovering(mousePos))
	{
		//Decrement the dimensions
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x -1, m_CurrentMap->getGridDims().y));

		//Decrement the LevelBits vector
		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}

		//For the objects the pointers are cleared at the end of each row.
		for (int i = 0; i < m_vItems.size(); i++)
		{
			if (m_vItems.at(i).at(m_vItems.at(i).size() - 1) != NULL)
			{
				delete(m_vItems.at(i).at(m_vItems.at(i).size() - 1));
				m_vItems.at(i).at(m_vItems.at(i).size()-1) = NULL;
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
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y+1));

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
	}

	//If the - Y Grid button is pressed
	if (m_vGridButtons.at(3)->hovering(mousePos))
	{
		//Decrement the dimensions
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y - 1));

		//Decrement the levelBits vector
		m_vcLevelBits.resize(m_CurrentMap->getGridDims().y);
		m_vcLevelBits.at(m_vcLevelBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

		//Clear the pointers of all objects on the bottom row and decrement the row
		for (int i = 0; i < m_vItems.at(m_vItems.size() - 1).size(); i++)
		{
			if (m_vItems.at(m_vItems.size()-1).at(i) != NULL)
			{
				delete(m_vItems.at(m_vItems.size()-1).at(i));
				m_vItems.at(m_vItems.size()-1).at(i) = NULL;
			}
		}
		m_vItems.resize(m_CurrentMap->getGridDims().y);
		m_vItems.at(m_vItems.size() - 1).resize(m_CurrentMap->getGridDims().x);

		//Decrement the floorBits vector
		m_vcFloorBits.resize(m_CurrentMap->getGridDims().y);
		m_vcFloorBits.at(m_vcFloorBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

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
		m_vFloorTiles.at(m_vFloorTiles.size() - 1).resize(m_CurrentMap->getGridDims().x);
	}
	
	//If the mouse is within the bounds of the grid
	if (mousePos.x > m_CurrentMap->getPosition().x &&
		mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
		mousePos.y > m_CurrentMap->getPosition().y &&
		mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
	{
		//Calculate the mouses position on the grid
		sf::Vector2f gridPos(0,0);
		gridPos.x = (mousePos.x - m_CurrentMap->getPosition().x) / m_CurrentMap->getTileSize().x;
		gridPos.y = (mousePos.y - m_CurrentMap->getPosition().y) / m_CurrentMap->getTileSize().y;

		switch (m_Editing)
		{
			case ObjectEdit:
				//If the tool is valid
				if (m_cCurrentTool == 'W' || m_cCurrentTool == 'P' || m_cCurrentTool == 'E' ||
					m_cCurrentTool == 'D' || m_cCurrentTool == 'B' || m_cCurrentTool == ' ')
				{
					//Set the main map bit to the current tool letter
					m_vcLevelBits.at(gridPos.y).at(gridPos.x) = m_cCurrentTool;

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
							break;
						case 'D':
							m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(24));
							break;
						case 'B':
							m_vItems.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(30));
							break;
						}
					}
				}
			break;

			case FloorEdit:
				//If the tool is valid
				if (m_cCurrentTool == 'G' || m_cCurrentTool == 'F' || m_cCurrentTool == 'K' ||
					m_cCurrentTool == 'C' || m_cCurrentTool == 'B' || m_cCurrentTool == 'R' ||
					m_cCurrentTool == ' ')
				{
					//Set the floor map bit to the current tool letter
					m_vcFloorBits.at(gridPos.y).at(gridPos.x) = m_cCurrentTool;

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
				}
			break;

			//case CharacterEdit:
				/*
				switch (m_cCurrentTool)
				{
					int iPathPos = (gridPos.y * m_vcLevelBits.size()) + gridPos.x; //Position in proper grid
					case ' ':
						break;
					case 'S':
						int iCurrentPathPos = 0;
						int iEnemyNumber = 0;

						for (int i = 0; i < m_vcLevelBits.size(); i++)
						{
							for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
							{
								iCurrentPathPos++;
								if (m_vcLevelBits.at(i).at(j) == 'E')
								{
									iEnemyNumber++;
								}
							}
						}
						break;
					case 'P':
						//m_viPaths.at(0).push_back();
						break;
				}
			break;*/
		}
	}
	
	//If an object button is being hovered over set the tool to the value of the tool
	if (m_vObjectButtons.at(0)->hovering(mousePos)) //Wooden Wall
	{
		m_cCurrentTool = 'W';
		m_Editing = ObjectEdit;
	}
	if (m_vObjectButtons.at(1)->hovering(mousePos)) //Friendly unit
	{
		m_cCurrentTool = 'P';
		m_Editing = ObjectEdit;
	}
	if (m_vObjectButtons.at(2)->hovering(mousePos)) //Enemy unit
	{
		m_cCurrentTool = 'E';
		m_Editing = ObjectEdit;
	}
	if (m_vObjectButtons.at(3)->hovering(mousePos)) //Door
	{
		m_cCurrentTool = 'D';
		m_Editing = ObjectEdit;
	}
	if (m_vObjectButtons.at(4)->hovering(mousePos)) //Brick wall
	{
		m_cCurrentTool = 'B';
		m_Editing = ObjectEdit;
	}

	if (m_vToolButtons.at(0)->hovering(mousePos)) //Eraser
	{
		m_cCurrentTool = ' ';
	}
	if (m_vToolButtons.at(1)->hovering(mousePos)) //Eraser
	{
		m_cCurrentTool = 'S';
		m_Editing = CharacterEdit;
	}
	if (m_vToolButtons.at(2)->hovering(mousePos)) //Eraser
	{
		m_cCurrentTool = 'P';
		m_Editing = CharacterEdit;
	}

	if (m_vFloorButtons.at(0)->hovering(mousePos)) //Grass
	{
		m_cCurrentTool = 'G';
		m_Editing = FloorEdit;
	}
	if (m_vFloorButtons.at(1)->hovering(mousePos)) //Wooden floor
	{
		m_cCurrentTool = 'F';
		m_Editing = FloorEdit;
	}
	if (m_vFloorButtons.at(2)->hovering(mousePos)) //Concrete
	{
		m_cCurrentTool = 'C';
		m_Editing = FloorEdit;
	}
	if (m_vFloorButtons.at(3)->hovering(mousePos)) //Kitchen tile
	{
		m_cCurrentTool = 'K';
		m_Editing = FloorEdit;
	}
	if (m_vFloorButtons.at(4)->hovering(mousePos)) //Blue carpet
	{
		m_cCurrentTool = 'B';
		m_Editing = FloorEdit;
	}
	if (m_vFloorButtons.at(5)->hovering(mousePos)) //Red carpet
	{
		m_cCurrentTool = 'R';
		m_Editing = FloorEdit;
	}

	return 0;
}

void Editor::saveMap()
{
	//Saves the object map 
	std::ofstream File1;
	File1.open("Assets/Maps/CustomMap.txt", std::ios::out);
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
		{
			File1 << m_vcLevelBits.at(i).at(j);
		}
		//Creates a new row of bits
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
		for (int j = 0; j < m_vcFloorBits.at(i).size(); j++)
		{
			File1 << m_vcFloorBits.at(i).at(j);
		}
		//Creates a new row of bits
		if (i != m_vcFloorBits.size() - 1)
		{
			File1 << "\n";
		}
	}
	File1.close();
	
}

void Editor::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_CurrentSettings->debugActive())
	{
		target.draw(*m_CurrentMap);
	}
	target.draw(m_Sidebar);

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

	for (int i = 0; i < m_vUIText.size(); i++)
	{
		target.draw(m_vUIText.at(i));
	}

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
}