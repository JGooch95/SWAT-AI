#include "../include/Menu.h"

Menu::Menu(sf::Vector2u windowSize)
{
	m_Textures = TextureLoader::getInstance();

	m_Background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_Background.setFillColor(sf::Color(77,77,78,255));
	m_Background.setPosition(0,0);

	m_ContentBox.setPosition(windowSize.x/ 3.0f, windowSize.y* (1.0f/6.0f));
	m_ContentBox.setSize(sf::Vector2f(windowSize.x/3.0f, windowSize.y * (2.0f/3.0f)));
	m_ContentBox.setFillColor(sf::Color(42,42,42, 255));

	float BorderSize = 0;
	for (int i = 0; i < 4; i++)
	{
		m_Buttons.push_back(new Button());
	}

	float fButtonGap = m_ContentBox.getSize().x / 30.0f;
	float fButtonArea = (m_ContentBox.getSize().y * (2.0f / 3.0f)) - (fButtonGap);
	float fButtonStart = m_ContentBox.getPosition().y + (m_ContentBox.getSize().y/ 3.0f) + fButtonGap;

	for (int i = 0; i < m_Buttons.size(); i++)
	{
		m_Buttons.at(i)->setColor(sf::Color(0, 0,0,0));
		m_Buttons.at(i)->setBackgroundColor(sf::Color(0, 186, 255, 255));
		m_Buttons.at(i)->setSize(sf::Vector2f(m_ContentBox.getSize().x - (fButtonGap *2), 
											 ((fButtonArea - (m_Buttons.size() * fButtonGap))/ m_Buttons.size())));
		m_Buttons.at(i)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap,
												  fButtonStart + (i* (m_Buttons.at(i)->getSize().y + fButtonGap))));
	}

	m_Buttons.at(0)->setText("Start");
	m_Buttons.at(1)->setText("Editor");
	m_Buttons.at(2)->setText("Options");
	m_Buttons.at(3)->setText("Exit");

	m_CurrentFont.loadFromFile("Assets/Fonts/arial.ttf");
	m_CurrentText.setString("SWAT AI");
	m_CurrentText.setFont(m_CurrentFont);
	m_CurrentText.setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap, m_ContentBox.getPosition().y + fButtonGap));

	m_CurrentText.setCharacterSize(200);
	while (m_CurrentText.getLocalBounds().width > m_ContentBox.getSize().x - (2 * fButtonGap) || 
		   m_CurrentText.getLocalBounds().height > (m_ContentBox.getSize().y / 3) - (2 * fButtonGap))
	{
		m_CurrentText.setCharacterSize(m_CurrentText.getCharacterSize() - 1);
	}
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
	target.draw(m_ContentBox);
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