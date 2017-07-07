#include "../include/HUDWindow.h"

HUDWindow::HUDWindow()
{
	//Links the resource files
	m_CurrentMap = Map::getInstance();
	m_Textures = TextureLoader::getInstance();
	m_Fonts = FontManager::getInstance();

	//Resizes the backdrop
	m_BackDrop.setSize(sf::Vector2f(400,100));
	m_BackDrop.setPosition(sf::Vector2f(0,0));
	m_BackDrop.setFillColor(sf::Color(120, 120, 120, 255));

	//Sets up the bars
	m_UIBars.resize(2);
	m_UIText.resize(m_UIBars.size());
	for (int i = 0; i < m_UIBars.size(); i++)
	{
		m_UIBars.at(i) = new Bar();
		m_UIBars.at(i)->setBarColor(sf::Color(255, 0, 0, 255));
		m_UIBars.at(i)->setLimit(100);

		m_Icons.push_back(Object());

		m_UIText.at(i) = new sf::Text();
		m_UIText.at(i)->setCharacterSize(20);
		m_UIText.at(i)->setFillColor(sf::Color(0, 0, 0, 255));
		m_UIText.at(i)->setFont(*m_Fonts->getFont(0));
	}
	m_UIBars.at(0)->setLevelColor(sf::Color(0, 255, 0, 255));
	m_UIBars.at(1)->setLevelColor(sf::Color(255, 255, 0, 255));

	//Sets up the loadout buttons
	m_LoadoutButton.resize(2);
	for (int i = 0; i < m_LoadoutButton.size(); i++)
	{
		m_LoadoutButton.at(i) = new Button();
		m_LoadoutButton.at(i)->setBackgroundColor(sf::Color(70, 70, 70, 255));
	}
	m_LoadoutButton.at(0)->setTexture(m_Textures->getTexture(12));
	m_LoadoutButton.at(1)->setTexture(m_Textures->getTexture(13));

	//Sets up the class selection button
	m_ClassButton.setBackgroundColor(sf::Color(70, 70, 70, 255));
	scaleUI();
}

void HUDWindow::setSize(sf::Vector2f size)
{
	m_BackDrop.setSize(size);
	scaleUI();
}

void HUDWindow::setPosition(sf::Vector2f pos)
{
	m_BackDrop.setPosition(pos);
	scaleUI();
}

void HUDWindow::setClassTexture(sf::Texture* newTex)
{
	m_ClassButton.setTexture(newTex);
}

void HUDWindow::setBarLevels(BarType currentBarType, Util::Limits barSettings)
{
	switch (currentBarType)
	{
		case HealthBar:
			m_UIBars.at(0)->setLevel(barSettings.lower);
			m_UIBars.at(0)->setLimit(barSettings.upper);
			break;

		case AmmoBar:
			m_UIBars.at(1)->setLevel(barSettings.lower);
			m_UIBars.at(1)->setLimit(barSettings.upper);
			break;
	}
}

void HUDWindow::setBarIcon(BarType currentBarType, sf::Texture* newTex)
{
	switch (currentBarType)
	{
		case HealthBar:
			m_Icons.at(0).setTexture(newTex);
			break;

		case AmmoBar:
			m_Icons.at(1).setTexture(newTex);
			break;
	}
}

void HUDWindow::setBarText(BarType currentBarType, std::string barText)
{
	switch (currentBarType)
	{
		case HealthBar:
			m_UIText.at(0)->setString(barText);
			m_UIText.at(0)->setPosition(m_UIBars.at(0)->getPosition() + sf::Vector2f((m_UIBars.at(0)->getSize().x / 2.0f) - (m_UIText.at(0)->getLocalBounds().width / 2.0f), 0.f));
			break;

		case AmmoBar:
			m_UIText.at(1)->setString(barText);
			m_UIText.at(1)->setPosition(m_UIBars.at(1)->getPosition() + sf::Vector2f((m_UIBars.at(1)->getSize().x / 2.0f) - (m_UIText.at(1)->getLocalBounds().width / 2.0f), 0.0f));
			break;
	}
}

void HUDWindow::scaleUI()
{
	//Class changing button
	m_ClassButton.setPosition(m_BackDrop.getPosition() + sf::Vector2f((m_BackDrop.getSize().y / 20.0f), m_BackDrop.getSize().y / 20.0f));
	m_ClassButton.setSize(sf::Vector2f(m_BackDrop.getSize().y / 4.0f, m_BackDrop.getSize().y / 4.0f));

	//Health and ammo bars
	for (int i = 0; i < m_UIBars.size(); i++)
	{
		//Bars
		m_UIBars.at(i)->setSize(sf::Vector2f(m_BackDrop.getSize().x - (m_BackDrop.getSize().x / 5.0f), m_BackDrop.getSize().y / 10.0f));
		m_UIBars.at(i)->setPosition(sf::Vector2f(m_BackDrop.getPosition().x + ((m_BackDrop.getSize().x / 5.0f) / 2.0f), m_BackDrop.getPosition().y + ((2 + i) * (m_BackDrop.getSize().y / 5.0f))));

		//Icons
		m_Icons.at(i).setSize(sf::Vector2f(m_UIBars.at(i)->getSize().y, m_UIBars.at(i)->getSize().y));
		m_Icons.at(i).setPosition(sf::Vector2f(m_UIBars.at(i)->getPosition().x - (m_Icons.at(i).getSize().x* m_Icons.at(i).getScale().x), m_UIBars.at(i)->getPosition().y));

		//Text
		while (m_UIText.at(i)->getGlobalBounds().height > m_UIBars.at(i)->getSize().y *(2.0f / 3.0f))
		{
			m_UIText.at(i)->setCharacterSize(m_UIText.at(i)->getCharacterSize() - 1);
		}
		m_UIText.at(i)->setPosition(m_UIBars.at(i)->getPosition() + sf::Vector2f((m_UIBars.at(i)->getSize().x / 2.0f) - (m_UIText.at(i)->getLocalBounds().width / 2.0f), (m_UIBars.at(i)->getSize().y / 3.0f)));
		m_UIText.at(i)->setFillColor(sf::Color(0, 0, 0, 255));
	}

	//Loadout buttons
	for (int i = 0; i < m_LoadoutButton.size(); i++)
	{
		m_LoadoutButton.at(i)->setPosition(sf::Vector2f(m_ClassButton.getPosition().x + ((i+1) * (m_ClassButton.getSize().x*m_ClassButton.getScale().x)) + ((m_ClassButton.getSize().x*m_ClassButton.getScale().x) / 4.0f), m_ClassButton.getPosition().y + ((m_ClassButton.getSize().y * m_ClassButton.getScale().y) / 4.0f)));
		m_LoadoutButton.at(i)->setSize(sf::Vector2f((m_ClassButton.getSize().y * m_ClassButton.getScale().y) / 2.0f, (m_ClassButton.getSize().y * m_ClassButton.getScale().y) / 2.0f));
	}
}

void HUDWindow::setLoadoutTexture(int iIndex, sf::Texture* newTex)
{
	m_LoadoutButton.at(iIndex)->setTexture(newTex);
}

sf::Vector2f HUDWindow::getSize()
{
	return m_BackDrop.getSize();
}

Button* HUDWindow::getClassButton()
{
	return &m_ClassButton;
}

Button* HUDWindow::getLoadoutButton(int iIndex)
{
	return m_LoadoutButton.at(iIndex);
}

void HUDWindow::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_BackDrop);
	
	for (int i = 0; i < m_UIBars.size(); i++)
	{
		target.draw(*m_UIBars.at(i));
	}

	for (int i = 0; i < m_Icons.size(); i++)
	{
		target.draw(m_Icons.at(i));
	}
	target.draw(m_ClassButton);

	for (int i = 0; i < m_LoadoutButton.size(); i++)
	{
		target.draw(*m_LoadoutButton.at(i));
	}

	for (int i = 0; i < m_UIText.size(); i++)
	{
		target.draw(*m_UIText.at(i));
	}
	
}

HUDWindow::~HUDWindow()
{
	for (int i = 0; i < m_LoadoutButton.size(); i++)
	{
		delete m_LoadoutButton.at(i);
		m_LoadoutButton.at(i) = NULL;
	}
}