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

	UIText.resize(2);
	UIText.at(0).setFont(m_CurrentFont);
	UIText.at(1).setFont(m_CurrentFont);
	UIText.at(0).setString("Grid X: ");
	UIText.at(1).setString("Grid Y: ");

	UIText.at(0).setCharacterSize(30);
	UIText.at(1).setCharacterSize(30);

	UIText.at(0).setFillColor(sf::Color::White);
	UIText.at(1).setFillColor(sf::Color::White);

	for (int i = 0; i < gridButtons.size(); i++)
	{
		gridButtons.at(i)->setSize(sf::Vector2f(m_Toolbar.getSize().y, m_Toolbar.getSize().y));
		gridButtons.at(i)->setPosition(m_Sidebar.getPosition() + sf::Vector2f(m_Sidebar.getSize().x * 0.75, m_Sidebar.getSize().y * (1/10)));

		if (i % 2 == 0)
		{
			gridButtons.at(i)->setTexture(m_Textures->getTexture(21));
			gridButtons.at(i)->setPosition(sf::Vector2f(gridButtons.at(i)->getPosition().x + gridButtons.at(i)->getSize().x + (gridButtons.at(i)->getSize().x / 2), gridButtons.at(i)->getPosition().y));
		}
		else
		{
			gridButtons.at(i)->setTexture(m_Textures->getTexture(22));
		}
	}

	for (int i = 0; i < gridButtons.size(); i += 2)
	{
		gridButtons.at(i)->setPosition(sf::Vector2f(gridButtons.at(i)->getPosition().x, gridButtons.at(i)->getPosition().y + (((i+2) / 2) * (gridButtons.at(i)->getSize().y + (gridButtons.at(i)->getSize().y / 2)))));
		gridButtons.at(i+1)->setPosition(sf::Vector2f(gridButtons.at(i+1)->getPosition().x, gridButtons.at(i+1)->getPosition().y + (((i+2) / 2) * (gridButtons.at(i+1)->getSize().y + (gridButtons.at(i+1)->getSize().y / 2)))));
		UIText.at(i / 2).setPosition(gridButtons.at(i+1)->getPosition() - sf::Vector2f(UIText.at(i / 2).getLocalBounds().width, 0.0f));
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

	editorButtons.resize(4);
	for (int i = 0; i < editorButtons.size(); i++)
	{
		editorButtons.at(i) = new Button();
		editorButtons.at(i)->setSize(gridButtons.at(0)->getSize());
		editorButtons.at(i)->setPosition(gridButtons.at(2)->getPosition() + sf::Vector2f(0, gridButtons.at(2)->getSize().y * (i+1)));
	}
	editorButtons.at(0)->setTexture(m_Textures->getTexture(0));
	editorButtons.at(1)->setTexture(m_Textures->getTexture(1));
	editorButtons.at(2)->setTexture(m_Textures->getTexture(2));
	editorButtons.at(3)->setTexture(m_Textures->getTexture(22));

	cCurrentTool = 'W';
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

	for (int i = 0; i < editorButtons.size(); i++)
	{
		editorButtons.at(i)->hovering(mousePos);
	}
}

int Editor::clickLeft(sf::Vector2i mousePos)
{
	if (exitButton->hovering(mousePos))
	{
		saveMap();
		//m_CurrentMap->setLevelBits(m_vcLevelBits);
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
		}

		for (int i = 0; i < m_vcLevelBits.size(); i++)
		{
			m_Items.at(i).resize(m_CurrentMap->getGridDims().x);
		}
	}

	if (gridButtons.at(2)->hovering(mousePos))
	{
		m_CurrentMap->setDimensions(sf::Vector2f(m_CurrentMap->getGridDims().x, m_CurrentMap->getGridDims().y+1));
		m_vcLevelBits.resize(m_CurrentMap->getGridDims().y);
		m_vcLevelBits.at(m_vcLevelBits.size() - 1).resize(m_CurrentMap->getGridDims().x);
		m_Items.resize(m_CurrentMap->getGridDims().y);
		m_Items.at(m_Items.size() - 1).resize(m_CurrentMap->getGridDims().x);
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
	}
	
	if (mousePos.x > m_CurrentMap->getPosition().x &&
		mousePos.x < m_CurrentMap->getPosition().x + m_CurrentMap->getWindowSize().x &&
		mousePos.y > m_CurrentMap->getPosition().y &&
		mousePos.y < m_CurrentMap->getPosition().y + m_CurrentMap->getWindowSize().y)
	{
		sf::Vector2f gridPos(0,0);
		gridPos.x = ((int)mousePos.x - (int)m_CurrentMap->getPosition().x) /(int) m_CurrentMap->getTileSize().x;
		gridPos.y = ((int)mousePos.y - (int)m_CurrentMap->getPosition().y) / (int)m_CurrentMap->getTileSize().y;

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
				m_Items.at(gridPos.y).at(gridPos.x)->setTexture(m_Textures->getTexture(22));

			}
	}
	if (editorButtons.at(0)->hovering(mousePos))
	{
		cCurrentTool = 'W';
	}
	if (editorButtons.at(1)->hovering(mousePos))
	{
		cCurrentTool = 'P';
	}
	if (editorButtons.at(2)->hovering(mousePos))
	{
		cCurrentTool = 'E';
	}
	if (editorButtons.at(3)->hovering(mousePos))
	{
		cCurrentTool = 'D';
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
		File1 << "\n";
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
	for (int i = 0; i < editorButtons.size(); i++)
	{
		target.draw(*editorButtons.at(i));
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

	for (int i = 0; i < editorButtons.size(); i++)
	{
		delete(editorButtons.at(i));
		editorButtons.at(i) = NULL;
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