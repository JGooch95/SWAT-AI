#include "../include/Menu.h"

Menu::Menu(sf::Vector2u windowSize)
{
	//Sets up the background
	m_Background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_Background.setFillColor(sf::Color(77,77,78,255));
	m_Background.setPosition(0,0);

	//Sets up the box containing the content
	m_ContentBox.setPosition(windowSize.x/ 3.0f, windowSize.y* (1.0f/6.0f));
	m_ContentBox.setSize(sf::Vector2f(windowSize.x/3.0f, windowSize.y * (2.0f/3.0f)));
	m_ContentBox.setFillColor(sf::Color(42,42,42, 255));

	//Create the buttons
	for (int i = 0; i < 4; ++i)
	{
		m_Buttons.push_back(new Button());
	}

	float fButtonGap = m_ContentBox.getSize().x / 30.0f; //The gap between each button
	float fButtonArea = (m_ContentBox.getSize().y * (2.0f / 3.0f)) - (fButtonGap); //The size in the y axis of where the buttons are kept
	float fButtonStart = m_ContentBox.getPosition().y + (m_ContentBox.getSize().y/ 3.0f) + fButtonGap; //The y position of where the buttons are kept

	//Setup every button
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons.at(i)->setColor(sf::Color(0, 0,0,0)); 
		m_Buttons.at(i)->setBackgroundColor(sf::Color(0, 186, 255, 255));
		m_Buttons.at(i)->setSize(sf::Vector2f(m_ContentBox.getSize().x - (fButtonGap *2), 
											 ((fButtonArea - (m_Buttons.size() * fButtonGap))/ m_Buttons.size())));
		m_Buttons.at(i)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap,
												  fButtonStart + (i* (m_Buttons.at(i)->getSize().y + fButtonGap))));
	}

	//Sets the button text
	m_Buttons.at(0)->setText("Start");
	m_Buttons.at(1)->setText("Editor");
	m_Buttons.at(2)->setText("Options");
	m_Buttons.at(3)->setText("Exit");

	//Sets up the title
	m_TitleText.setString("SWAT AI");
	m_TitleText.setFont(*m_Fonts->getFont(0));
	m_TitleText.setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap, m_ContentBox.getPosition().y + fButtonGap));

	//Fits the title to be within the bounds of the content box
	m_TitleText.setCharacterSize(200);
	while (m_TitleText.getLocalBounds().width > m_ContentBox.getSize().x - (2 * fButtonGap) || 
		   m_TitleText.getLocalBounds().height > (m_ContentBox.getSize().y / 3) - (2 * fButtonGap))
	{
		m_TitleText.setCharacterSize(m_TitleText.getCharacterSize() - 1);
	}
}

void Menu::update(sf::Vector2i mousePos)
{
	//Checks buttons for a hovering mouse
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons.at(i)->update(mousePos);
	}
}

int Menu::processInput(sf::Event keyCode, sf::Vector2i mousePos)
{
	if (keyCode.type == sf::Event::MouseButtonPressed)
	{
		switch (keyCode.key.code)
		{
		case sf::Mouse::Left:
			//Checks if the buttons have been clicked
			for (int i = 0; i < m_Buttons.size(); ++i)
			{
				if (m_Buttons.at(i)->hovering(mousePos))
				{
					switch (i + 1)
					{
						case 1:
							return S_Game;
						case 2:
							return S_Editor;
						case 3:
							return S_Options;
						case 4:
							return Exit;
					}
				}
			}
			break;
		}
	}
	return S_None;
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Background);
	target.draw(m_ContentBox);
	target.draw(m_TitleText);
	
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		target.draw(*m_Buttons.at(i));
	}
}

Menu::~Menu()
{
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		delete(m_Buttons.at(i));
		m_Buttons.at(i) = NULL;
	}
}