#include "../include/HUDWindow.h"

HUDWindow::HUDWindow()
{
	m_CurrentMap->getInstance();
	m_BackDrop.setSize(sf::Vector2f(400,100));
	m_BackDrop.setPosition(sf::Vector2f(0,0));
	m_BackDrop.setFillColor(sf::Color(120, 120, 120, 255));

	m_Icons.push_back(Object());
	m_Icons.push_back(Object());

	m_UIBars.push_back(new Bar);
	m_UIBars.at(m_UIBars.size() - 1)->setLevelColor(sf::Color(0, 255, 0, 255));
	m_UIBars.at(m_UIBars.size() - 1)->setBarColor(sf::Color(255, 0, 0, 255));
	m_UIBars.at(m_UIBars.size() - 1)->setLimit(100);
	m_UIBars.push_back(new Bar);
	m_UIBars.at(m_UIBars.size() - 1)->setLevelColor(sf::Color(255, 255, 0, 255));
	m_UIBars.at(m_UIBars.size() - 1)->setBarColor(sf::Color(255, 0, 0, 255));
	m_UIBars.at(m_UIBars.size() - 1)->setLimit(100);

	m_UIText.push_back(new sf::Text());
	m_UIText.at(m_UIText.size() - 1)->setCharacterSize(20);
	m_UIText.at(m_UIText.size() - 1)->setFillColor(sf::Color(0, 0, 0, 255));

	m_UIText.push_back(new sf::Text());
	m_UIText.at(m_UIText.size() - 1)->setCharacterSize(20);
	m_UIText.at(m_UIText.size() - 1)->setFillColor(sf::Color(0, 0, 0, 255));

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
		m_UIText.at(0)->setPosition(m_UIBars.at(0)->getPosition() + sf::Vector2f((m_UIBars.at(0)->getSize().x / 2) - (m_UIText.at(0)->getLocalBounds().width / 2), 0));
		break;

	case AmmoBar:
		m_UIText.at(1)->setString(barText);
		m_UIText.at(1)->setPosition(m_UIBars.at(1)->getPosition() + sf::Vector2f((m_UIBars.at(1)->getSize().x / 2) - (m_UIText.at(1)->getLocalBounds().width / 2), 0));
		break;
	}
}

void HUDWindow::setFont(sf::Font* newFont)
{
	for (int i = 0; i < m_UIText.size(); i++)
	{
		m_UIText.at(i)->setFont(*newFont);
	}
}

void HUDWindow::scaleUI()
{
	//Health Bar
	m_UIBars.at(0)->setSize(sf::Vector2f(m_BackDrop.getSize().x - (m_BackDrop.getSize().x / 5.0f),
		m_BackDrop.getSize().y / 10));
	m_UIBars.at(0)->setPosition(sf::Vector2f(m_BackDrop.getPosition().x + ((m_BackDrop.getSize().x / 5.0f) / 2.0f),
		m_BackDrop.getPosition().y + (2 * (m_BackDrop.getSize().y / 5.0f))));

	m_Icons.at(0).setSize(sf::Vector2f(m_UIBars.at(0)->getSize().y, m_UIBars.at(0)->getSize().y));
	m_Icons.at(0).setPosition(sf::Vector2f(m_UIBars.at(0)->getPosition().x - (m_Icons.at(0).getSize().x* m_Icons.at(0).getScale().x), m_UIBars.at(0)->getPosition().y));

	while (m_UIText.at(0)->getGlobalBounds().height > m_UIBars.at(0)->getSize().y *(2.0f / 3.0f))
	{
		m_UIText.at(0)->setCharacterSize(m_UIText.at(0)->getCharacterSize() - 1);
	}
	m_UIText.at(0)->setPosition(m_UIBars.at(0)->getPosition() + sf::Vector2f((m_UIBars.at(0)->getSize().x / 2) - (m_UIText.at(0)->getLocalBounds().width / 2), (m_UIBars.at(0)->getSize().y / 3)));

	//Ammo Bar
	m_UIBars.at(1)->setSize(sf::Vector2f(m_BackDrop.getSize().x - (m_BackDrop.getSize().x / 5.0f), m_BackDrop.getSize().y / 10));
	m_UIBars.at(1)->setPosition(sf::Vector2f(m_BackDrop.getPosition().x + ((m_BackDrop.getSize().x / 5.0f) / 2.0f),
		m_BackDrop.getPosition().y + (3 * (m_BackDrop.getSize().y / 5.0f))));

	m_Icons.at(1).setSize(sf::Vector2f(m_UIBars.at(1)->getSize().y, m_UIBars.at(1)->getSize().y));
	m_Icons.at(1).setPosition(sf::Vector2f(m_UIBars.at(1)->getPosition().x - (m_Icons.at(1).getSize().x * m_Icons.at(1).getScale().x), m_UIBars.at(1)->getPosition().y));

	while (m_UIText.at(1)->getGlobalBounds().height > m_UIBars.at(1)->getSize().y *(2.0f / 3.0f))
	{
		m_UIText.at(1)->setCharacterSize(m_UIText.at(1)->getCharacterSize() - 1);
	}
	m_UIText.at(1)->setPosition(m_UIBars.at(1)->getPosition() + sf::Vector2f((m_UIBars.at(1)->getSize().x / 2) - (m_UIText.at(1)->getLocalBounds().width / 2), (m_UIBars.at(1)->getSize().y / 3)));
	m_UIText.at(1)->setFillColor(sf::Color(0, 0, 0, 255));

	m_ClassButton.setPosition(m_BackDrop.getPosition() + sf::Vector2f((m_BackDrop.getSize().x / 20), m_BackDrop.getSize().x / 20));
	m_ClassButton.setSize(sf::Vector2f(m_BackDrop.getSize().y / 4, m_BackDrop.getSize().y / 4));

	for (int i = 0; i < m_UIText.size(); i++)
	{
		while (m_UIText.at(i)->getGlobalBounds().height > m_UIBars.at(i)->getSize().y *(2.0f / 3.0f))
		{
			m_UIText.at(i)->setCharacterSize(m_UIText.at(i)->getCharacterSize() - 1);
		}
	}
}

sf::Vector2f HUDWindow::getSize()
{
	return m_BackDrop.getSize();
}

Button* HUDWindow::getClassButton()
{
	return &m_ClassButton;
}

void HUDWindow::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Draws the Unit UI
	
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

	for (int i = 0; i < m_UIText.size(); i++)
	{
		target.draw(*m_UIText.at(i));
	}
	
}

HUDWindow::~HUDWindow()
{

}