#include "../include/Editor.h"

Editor::Editor(sf::Vector2u windowSize)
{
	//Sets up the default map
	m_CurrentMap = Map::getInstance();
	m_CurrentSettings = Settings::getInstance();
	m_Textures = TextureLoader::getInstance();

	//Sets up the toolbar 
	m_Toolbar.setPosition(0, 0);
	m_Toolbar.setFillColor(sf::Color(70, 70, 70, 255));
	m_Toolbar.setSize(sf::Vector2f(windowSize.x, windowSize.y / 20));

	m_CurrentMap->setup(
		sf::FloatRect(sf::Vector2f(0, m_Toolbar.getSize().y), sf::Vector2f(windowSize) - sf::Vector2f(windowSize.x / 3, m_Toolbar.getSize().y)),
		sf::Vector2f(20, 20)
	);

	m_CurrentFont.loadFromFile("Assets/Fonts/arial.ttf"); //Loads the main font

	m_CurrentSettings->setDebug(true);

	exitButton = new Button();
	exitButton->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
	exitButton->setPosition(sf::Vector2f(m_Toolbar.getPosition().x + m_Toolbar.getSize().x - exitButton->getSize().x, m_Toolbar.getPosition().y));
	exitButton->setTexture(m_Textures->getTexture(20));

	m_Sidebar.setPosition(m_CurrentMap->getWindowSize().x, m_Toolbar.getSize().y);
	m_Sidebar.setFillColor(sf::Color(120, 120, 120, 255));
	m_Sidebar.setSize(sf::Vector2f(windowSize.x - m_CurrentMap->getWindowSize().x, windowSize.y - m_Toolbar.getSize().y));

	for (int i = 0; i < 4; i++)
	{
		gridButtons.push_back(new Button);
	}

	float fBorderSize = m_Sidebar.getSize().x / 20.0f;
	float fContentGap = m_Sidebar.getSize().y / 20.0f;
	UIText.resize(2);
	UIText.at(0).setString("Grid X: ");
	UIText.at(1).setString("Grid Y: ");

	for (int i = 0; i < UIText.size(); i++)
	{
		UIText.at(i).setFont(m_CurrentFont);
		UIText.at(i).setCharacterSize(100);
		while (UIText.at(i).getLocalBounds().width > m_Sidebar.getSize().x / 4)
		{
			UIText.at(i).setCharacterSize(UIText.at(i).getCharacterSize()-1);
		}
		float fYOffset = 0.0f;
		if (i > 0)
		{
			fYOffset = UIText.at(i - 1).getLocalBounds().height;
		}

		UIText.at(i).setPosition(m_Sidebar.getPosition().x + fBorderSize, m_Sidebar.getPosition().y + ((i + 1) * fYOffset) + fBorderSize);
	}

	UIText.at(0).setFillColor(sf::Color::White);
	UIText.at(1).setFillColor(sf::Color::White);

	for (int i = 0; i < gridButtons.size(); i++)
	{
		gridButtons.at(i)->setSize(sf::Vector2f(UIText.at(i/2).getLocalBounds().height, UIText.at(i / 2).getLocalBounds().height));
		gridButtons.at(i)->setPosition( sf::Vector2f(UIText.at(i / 2).getPosition().x + UIText.at(i / 2).getLocalBounds().width, UIText.at(i / 2).getPosition().y + (UIText.at(i / 2).getLocalBounds().height / 2)));

		if (i % 2 == 0)
		{
			gridButtons.at(i)->setTexture(m_Textures->getTexture(21));
			gridButtons.at(i)->setPosition(sf::Vector2f(gridButtons.at(i)->getPosition().x + gridButtons.at(i)->getSize().x + (gridButtons.at(i)->getSize().x / 2), gridButtons.at(i)->getPosition().y ));
		}
		else
		{
			gridButtons.at(i)->setTexture(m_Textures->getTexture(22));
		}
	}

	m_vcLevelBits.clear();
	m_vcLevelBits.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().y);
	}

	m_Items.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_Items.size(); i++)
	{
		m_Items.at(i).resize(m_CurrentMap->getGridDims().y);
	}

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

	m_FloorTiles.resize(m_CurrentMap->getGridDims().x);
	for (int i = 0; i < m_FloorTiles.size(); i++)
	{
		m_FloorTiles.at(i).resize(m_CurrentMap->getGridDims().y);
	}

	for (int i = 0; i < m_FloorTiles.size(); i++)
	{
		for (int j = 0; j < m_FloorTiles.at(i).size(); j++)
		{
			m_FloorTiles.at(i).at(j) = new Object();
			m_FloorTiles.at(i).at(j)->setTexture(m_Textures->getTexture(14));
		}
	}

	objectButtons.resize(4);
	for (int i = 0; i < objectButtons.size(); i++)
	{
		objectButtons.at(i) = new Button();
		objectButtons.at(i)->setSize(gridButtons.at(0)->getSize() * 2.0f);

		float fXOffset = 0.0f;
		float fYOffset = 0.0f;
		if (i > 0)
		{
			fXOffset = objectButtons.at(i - 1)->getSize().x;

			if (objectButtons.at(i - 1)->getPosition().x + objectButtons.at(i - 1)->getSize().x + objectButtons.at(i)->getSize().x > m_Sidebar.getPosition().x + m_Sidebar.getSize().x - fBorderSize)
			{
				fXOffset = 0.0f;
				fYOffset += objectButtons.at(i - 1)->getSize().y;
			}

		}
		objectButtons.at(i)->setPosition(sf::Vector2f(m_Sidebar.getPosition().x + fBorderSize + ( i* fXOffset), gridButtons.at(gridButtons.size() - 1)->getPosition().y + gridButtons.at(gridButtons.size() - 1)->getSize().y + fContentGap +fYOffset));
	}
	objectButtons.at(0)->setTexture(m_Textures->getTexture(0));
	objectButtons.at(1)->setTexture(m_Textures->getTexture(1));
	objectButtons.at(2)->setTexture(m_Textures->getTexture(2));
	objectButtons.at(3)->setTexture(m_Textures->getTexture(24));

	floorButtons.resize(4);
	for (int i = 0; i < floorButtons.size(); i++)
	{
		floorButtons.at(i) = new Button();
		floorButtons.at(i)->setSize(gridButtons.at(0)->getSize() * 2.0f);

		float fXOffset = 0.0f;
		float fYOffset = 0.0f;
		if (i > 0)
		{
			fXOffset = floorButtons.at(i - 1)->getSize().x;

			if (floorButtons.at(i - 1)->getPosition().x + floorButtons.at(i - 1)->getSize().x + floorButtons.at(i)->getSize().x > m_Sidebar.getPosition().x + m_Sidebar.getSize().x - fBorderSize)
			{
				fXOffset = 0.0f;
				fYOffset += floorButtons.at(i - 1)->getSize().y;
			}

		}
		floorButtons.at(i)->setPosition(sf::Vector2f(m_Sidebar.getPosition().x + fBorderSize + (i* fXOffset), objectButtons.at(objectButtons.size() - 1)->getPosition().y + objectButtons.at(gridButtons.size() - 1)->getSize().y + fContentGap + fYOffset));
	}
	floorButtons.at(0)->setTexture(m_Textures->getTexture(14));
	floorButtons.at(1)->setTexture(m_Textures->getTexture(25));
	floorButtons.at(2)->setTexture(m_Textures->getTexture(26));
	floorButtons.at(3)->setTexture(m_Textures->getTexture(27));

	toolButtons.resize(1);
	for (int i = 0; i < toolButtons.size(); i++)
	{
		toolButtons.at(i) = new Button();
		toolButtons.at(i)->setSize(gridButtons.at(0)->getSize() * 2.0f);

		float fXOffset = 0.0f;
		float fYOffset = 0.0f;
		if (i > 0)
		{
			fXOffset = toolButtons.at(i - 1)->getSize().x;

			if (toolButtons.at(i - 1)->getPosition().x + toolButtons.at(i - 1)->getSize().x + toolButtons.at(i)->getSize().x > m_Sidebar.getPosition().x + m_Sidebar.getSize().x - fBorderSize)
			{
				fXOffset = 0.0f;
				fYOffset += toolButtons.at(i - 1)->getSize().y;
			}

		}
		toolButtons.at(i)->setPosition(sf::Vector2f(m_Sidebar.getPosition().x + fBorderSize + (i* fXOffset), floorButtons.at(floorButtons.size() - 1)->getPosition().y + floorButtons.at(floorButtons.size() - 1)->getSize().y + fContentGap + fYOffset));
	}
	toolButtons.at(0)->setTexture(m_Textures->getTexture(23));

	cCurrentTool = 'W';
	bEditingFloor = false;
}

void Editor::update(sf::Vector2i mousePos)
{
	exitButton->hovering(mousePos);
	for (int i = 0; i < gridButtons.size(); i++)
	{
		gridButtons.at(i)->hovering(mousePos);
	}

	for (int i = 0; i < m_Items.size(); i++)
	{
		for (int j = 0; j < m_Items.at(i).size(); j++)
		{
			if (m_Items.at(i).at(j) != NULL)
			{
				m_Items.at(i).at(j)->setSize(m_CurrentMap->getTileSize());
				m_Items.at(i).at(j)->setPosition(sf::Vector2f((j * m_CurrentMap->getTileSize().x) + m_CurrentMap->getPosition().x, (i * m_CurrentMap->getTileSize().y) + m_CurrentMap->getPosition().y));
			}
		}
	}

	for (int i = 0; i < m_FloorTiles.size(); i++)
	{
		for (int j = 0; j < m_FloorTiles.at(i).size(); j++)
		{
			if (m_FloorTiles.at(i).at(j) != NULL)
			{
				m_FloorTiles.at(i).at(j)->setSize(m_CurrentMap->getTileSize());
				m_FloorTiles.at(i).at(j)->setPosition(sf::Vector2f((j * m_CurrentMap->getTileSize().x) + m_CurrentMap->getPosition().x, (i * m_CurrentMap->getTileSize().y) + m_CurrentMap->getPosition().y));
			}
		}
	}

	for (int i = 0; i < objectButtons.size(); i++)
	{
		objectButtons.at(i)->hovering(mousePos);
	}
	for (int i = 0; i < toolButtons.size(); i++)
	{
		toolButtons.at(i)->hovering(mousePos);
	}
	for (int i = 0; i < floorButtons.size(); i++)
	{
		floorButtons.at(i)->hovering(mousePos);
	}
}

int Editor::clickLeft(sf::Vector2i mousePos)
{
	if (exitButton->hovering(mousePos))
	{
		saveMap();
		m_CurrentSettings->setDebug(false);
		return 1;
	}
	
	if (gridButtons.at(0)->hovering(mousePos))
	{
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x+1, m_CurrentMap->getGridDims().y));
		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}
		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_Items.at(i).resize(m_CurrentMap->getGridDims().x);
		}

		for (int i = 0; i < m_vcFloorBits.size(); i++)
		{
			m_vcFloorBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}
		for (int i = 0; i <  m_vcFloorBits.size(); i++)
		{
			m_FloorTiles.at(i).resize(m_CurrentMap->getGridDims().x);
		}
	}
	if (gridButtons.at(1)->hovering(mousePos))
	{
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x -1, m_CurrentMap->getGridDims().y));
		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_vcLevelBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}

		for (int i = 0; i < m_Items.size(); i++)
		{
			if (m_Items.at(i).at(m_Items.at(i).size() - 1) != NULL)
			{
				delete(m_Items.at(i).at(m_Items.at(i).size() - 1));
				m_Items.at(i).at(m_Items.at(i).size()-1) = NULL;
			}
			m_Items.at(i).resize(m_CurrentMap->getGridDims().x);
		}

		for (int i = 0; i < m_vcFloorBits.size(); i++)
		{
			m_vcFloorBits.at(i).resize(m_CurrentMap->getGridDims().x);
		}

		for (int i = 0; i < m_FloorTiles.size(); i++)
		{
			if (m_FloorTiles.at(i).at(m_FloorTiles.at(i).size() - 1) != NULL)
			{
				delete(m_FloorTiles.at(i).at(m_FloorTiles.at(i).size() - 1));
				m_FloorTiles.at(i).at(m_FloorTiles.at(i).size() - 1) = NULL;
			}
			m_FloorTiles.at(i).resize(m_CurrentMap->getGridDims().x);
		}
	}

	if (gridButtons.at(2)->hovering(mousePos))
	{
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y+1));
		m_vcLevelBits.resize(m_CurrentMap->getGridDims().y);
		m_vcLevelBits.at(m_vcLevelBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

		m_Items.resize(m_CurrentMap->getGridDims().y);
		m_Items.at(m_Items.size() - 1).resize(m_CurrentMap->getGridDims().x);


		m_vcFloorBits.resize(m_CurrentMap->getGridDims().y);
		m_vcFloorBits.at(m_vcFloorBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

		m_FloorTiles.resize(m_CurrentMap->getGridDims().y);
		m_FloorTiles.at(m_FloorTiles.size() - 1).resize(m_CurrentMap->getGridDims().x);
	}
	if (gridButtons.at(3)->hovering(mousePos))
	{
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y - 1));
		m_vcLevelBits.resize(m_CurrentMap->getGridDims().y);
		m_vcLevelBits.at(m_vcLevelBits.size() - 1).resize(m_CurrentMap->getGridDims().x);

		for (int i = 0; i < m_Items.at(m_Items.size() - 1).size(); i++)
		{
			if (m_Items.at(m_Items.size()-1).at(i) != NULL)
			{
				delete(m_Items.at(m_Items.size()-1).at(i));
				m_Items.at(m_Items.size()-1).at(i) = NULL;
			}
		}
		m_Items.resize(m_CurrentMap->getGridDims().y);
		m_Items.at(m_Items.size() - 1).resize(m_CurrentMap->getGridDims().x);

		m_vcFloorBits.resize(m_CurrentMap->getGridDims().y);
		m_vcFloorBits.at(m_vcFloorBits.size() - 1).resize(m_CurrentMap->getGridDims().x);
		for (int i = 0; i < m_FloorTiles.at(m_Items.size() - 1).size(); i++)
		{
			if (m_FloorTiles.at(m_FloorTiles.size() - 1).at(i) != NULL)
			{
				delete(m_FloorTiles.at(m_FloorTiles.size() - 1).at(i));
				m_FloorTiles.at(m_FloorTiles.size() - 1).at(i) = NULL;
			}
		}
		m_FloorTiles.resize(m_CurrentMap->getGridDims().y);
		m_FloorTiles.at(m_FloorTiles.size() - 1).resize(m_CurrentMap->getGridDims().x);
	}
	
	if (mousePos.x > m_CurrentMap->getPosition().x &&
		mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
		mousePos.y > m_CurrentMap->getPosition().y &&
		mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
	{
		sf::Vector2f gridPos(0,0);
		gridPos.x = (mousePos.x - m_CurrentMap->getPosition().x) / m_CurrentMap->getTileSize().x;
		gridPos.y = (mousePos.y - m_CurrentMap->getPosition().y) / m_CurrentMap->getTileSize().y;

		if (!bEditingFloor)
		{
			m_vcLevelBits.at(gridPos.y).at(gridPos.x) = cCurrentTool;

			if (cCurrentTool == 'W')
			{
				if (m_Items.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_Items.at(gridPos.y).at(gridPos.x));
					m_Items.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_Items.at(gridPos.y).at(gridPos.x) = new Object();
				m_Items.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(0));
			}
			if (cCurrentTool == 'P')
			{
				if (m_Items.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_Items.at(gridPos.y).at(gridPos.x));
					m_Items.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_Items.at(gridPos.y).at(gridPos.x) = new Object();
				m_Items.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(1));

			}
			if (cCurrentTool == 'E')
			{
				if (m_Items.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_Items.at(gridPos.y).at(gridPos.x));
					m_Items.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_Items.at(gridPos.y).at(gridPos.x) = new Object();
				m_Items.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(2));

			}
			if (cCurrentTool == 'D')
			{
				if (m_Items.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_Items.at(gridPos.y).at(gridPos.x));
					m_Items.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_Items.at(gridPos.y).at(gridPos.x) = new Object();
				m_Items.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(24));

			}
			if (cCurrentTool == ' ')
			{
				if (m_Items.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_Items.at(gridPos.y).at(gridPos.x));
					m_Items.at(gridPos.y).at(gridPos.x) = NULL;
				}
			}
		}
		else
		{
			m_vcFloorBits.at(gridPos.y).at(gridPos.x) = cCurrentTool;
			if (cCurrentTool == 'G')
			{
				if (m_FloorTiles.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_FloorTiles.at(gridPos.y).at(gridPos.x));
					m_FloorTiles.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_FloorTiles.at(gridPos.y).at(gridPos.x) = new Object();
				m_FloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(14));
			}
			if (cCurrentTool == 'F')
			{
				if (m_FloorTiles.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_FloorTiles.at(gridPos.y).at(gridPos.x));
					m_FloorTiles.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_FloorTiles.at(gridPos.y).at(gridPos.x) = new Object();
				m_FloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(25));
			}
			if (cCurrentTool == 'K')
			{
				if (m_FloorTiles.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_FloorTiles.at(gridPos.y).at(gridPos.x));
					m_FloorTiles.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_FloorTiles.at(gridPos.y).at(gridPos.x) = new Object();
				m_FloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(27));
			}
			if (cCurrentTool == 'C')
			{
				if (m_FloorTiles.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_FloorTiles.at(gridPos.y).at(gridPos.x));
					m_FloorTiles.at(gridPos.y).at(gridPos.x) = NULL;
				}
				m_FloorTiles.at(gridPos.y).at(gridPos.x) = new Object();
				m_FloorTiles.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(26));
			}
			if (cCurrentTool == ' ')
			{
				if (m_FloorTiles.at(gridPos.y).at(gridPos.x) != NULL)
				{
					delete(m_FloorTiles.at(gridPos.y).at(gridPos.x));
					m_FloorTiles.at(gridPos.y).at(gridPos.x) = NULL;
				}
			}
		}
	}
	if (objectButtons.at(0)->hovering(mousePos))
	{
		cCurrentTool = 'W';
		bEditingFloor = false;
	}
	if (objectButtons.at(1)->hovering(mousePos))
	{
		cCurrentTool = 'P';
		bEditingFloor = false;
	}
	if (objectButtons.at(2)->hovering(mousePos))
	{
		cCurrentTool = 'E';
		bEditingFloor = false;
	}
	if (objectButtons.at(3)->hovering(mousePos))
	{
		cCurrentTool = 'D';
		bEditingFloor = false;
	}

	if (toolButtons.at(0)->hovering(mousePos))
	{
		cCurrentTool = ' ';
	}

	if (floorButtons.at(0)->hovering(mousePos))
	{
		cCurrentTool = 'G';
		bEditingFloor = true;
	}
	if (floorButtons.at(1)->hovering(mousePos))
	{
		cCurrentTool = 'F';
		bEditingFloor = true;
	}
	if (floorButtons.at(2)->hovering(mousePos))
	{
		cCurrentTool = 'C';
		bEditingFloor = true;
	}
	if (floorButtons.at(3)->hovering(mousePos))
	{
		cCurrentTool = 'K';
		bEditingFloor = true;
	}

	return 0;
}

void Editor::enableDebug()
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

void Editor::saveMap()
{
	//Saves a score value to a text file
	std::ofstream File1;
	File1.open("Assets/Maps/CustomMap.txt", std::ios::out);
	for (int i = 0; i < m_vcLevelBits.size(); i++)
	{
		for (int j = 0; j < m_vcLevelBits.at(i).size(); j++)
		{
			File1 << m_vcLevelBits.at(i).at(j);
		}
		if (i != m_vcLevelBits.size() - 1)
		{
			File1 << "\n";
		}
	}
	File1.close();

	File1.open("Assets/Maps/CustomMapFloor.txt", std::ios::out);
	for (int i = 0; i < m_vcFloorBits.size(); i++)
	{
		for (int j = 0; j < m_vcFloorBits.at(i).size(); j++)
		{
			File1 << m_vcFloorBits.at(i).at(j);
		}
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
	target.draw(m_Toolbar);
	target.draw(*exitButton);

	for (int i = 0; i < m_FloorTiles.size(); i++)
	{
		for (int j = 0; j < m_FloorTiles.at(i).size(); j++)
		{
			if (m_FloorTiles.at(i).at(j) != NULL)
			{
				target.draw(*m_FloorTiles.at(i).at(j));
			}
		}
	}

	for (int i = 0; i < m_Items.size(); i++)
	{
		for (int j = 0; j < m_Items.at(i).size(); j++)
		{
			if (m_Items.at(i).at(j) != NULL)
			{
				target.draw(*m_Items.at(i).at(j));
			}
		}
	}

	for (int i = 0; i < gridButtons.size(); i++)
	{
		target.draw(*gridButtons.at(i));
	}
	for (int i = 0; i < objectButtons.size(); i++)
	{
		target.draw(*objectButtons.at(i));
	}
	for (int i = 0; i < toolButtons.size(); i++)
	{
		target.draw(*toolButtons.at(i));
	}
	for (int i = 0; i < floorButtons.size(); i++)
	{
		target.draw(*floorButtons.at(i));
	}

	for (int i = 0; i < UIText.size(); i++)
	{
		target.draw(UIText.at(i));
	}
}

Editor::~Editor()
{
	for (int i = 0; i < gridButtons.size(); i++)
	{
		delete(gridButtons.at(i));
		gridButtons.at(i) = NULL;
	}

	for (int i = 0; i < objectButtons.size(); i++)
	{
		delete(objectButtons.at(i));
		objectButtons.at(i) = NULL;
	}
	for (int i = 0; i < floorButtons.size(); i++)
	{
		delete(floorButtons.at(i));
		floorButtons.at(i) = NULL;
	}
	for (int i = 0; i < toolButtons.size(); i++)
	{
		delete(toolButtons.at(i));
		toolButtons.at(i) = NULL;
	}
	for (int i = 0; i < m_FloorTiles.size(); i++)
	{
		for (int j = 0; j < m_FloorTiles.at(i).size(); j++)
		{
			if (m_FloorTiles.at(i).at(j) != NULL)
			{
				delete(m_FloorTiles.at(i).at(j));
				m_FloorTiles.at(i).at(j) = NULL;
			}
		}
	}

	for (int i = 0; i < m_Items.size(); i++)
	{
		for (int j = 0; j < m_Items.at(i).size(); j++)
		{
			if (m_Items.at(i).at(j) != NULL)
			{
				delete(m_Items.at(i).at(j));
				m_Items.at(i).at(j) = NULL;
			}
		}
	}
	delete(exitButton);
	exitButton = NULL;
}