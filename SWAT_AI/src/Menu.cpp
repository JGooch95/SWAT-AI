#include "../include/Menu.h"

Menu::Menu(sf::Vector2u windowSize)
{
	m_Textures = TextureLoader::getInstance();

	for (int i = 0; i < 4; i++)
	{
		m_Buttons.push_back(new Button());
	}
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		m_Buttons.at(i)->setTexture(m_Textures->getTexture(15));
		m_Buttons.at(i)->setSize(sf::Vector2f(windowSize.x / 3.0f, ((windowSize.y / m_Buttons.size()) / 2)));
		m_Buttons.at(i)->setPosition(sf::Vector2f(windowSize.x / 3.0f, (windowSize.y / 3.0f) + (i*(windowSize.y / m_Buttons.size()) / 2) + (i * 1)));
	}

	m_Buttons.at(0)->setText("Start");
	m_Buttons.at(1)->setText("Options");
	m_Buttons.at(2)->setText("Editor");
	m_Buttons.at(3)->setText("Exit");

	m_Background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_Background.setFillColor(sf::Color(100,100,100,255));
	m_Background.setPosition(0,0);

	m_CurrentFont.loadFromFile("Assets/Fonts/arial.ttf");
	m_CurrentText.setString("SWAT AI");
	m_CurrentText.setFont(m_CurrentFont);
	m_CurrentText.setCharacterSize(200);
	m_CurrentText.setPosition(sf::Vector2f((windowSize.x /2.0f) - (m_CurrentText.getLocalBounds().width/2), windowSize.y*0.1));
}

void Menu::update(sf::Vector2i mousePos)
{
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		m_Buttons.at(i)->hovering(mousePos);
	}
}

int Menu::clickLeft(sf::Vector2i mousePos)
{
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		if (m_Buttons.at(i)->hovering(mousePos))
		{
			return i + 1;
		}
	}
	
	return 0;
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Background);
	target.draw(m_CurrentText);
	
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		target.draw(*m_Buttons.at(i));
	}
}

Menu::~Menu()
{
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		delete(m_Buttons.at(i));
		m_Buttons.at(i) = NULL;
	}
}