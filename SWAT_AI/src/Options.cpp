#include "../include/Options.h"

Options::Options(sf::Vector2u windowSize)
{
	m_CurrentSettings = Settings::getInstance();
	m_Textures = TextureLoader::getInstance();
	m_Fonts = FontManager::getInstance();

	m_Textures = TextureLoader::getInstance();
	m_Fonts = FontManager::getInstance();

	//Sets up the background
	m_Background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_Background.setFillColor(sf::Color(77, 77, 78, 255));
	m_Background.setPosition(0, 0);

	//Sets up the box containing the content
	m_ContentBox.setPosition(windowSize.x / 3.0f, windowSize.y* (1.0f / 6.0f));
	m_ContentBox.setSize(sf::Vector2f(windowSize.x / 3.0f, windowSize.y * (2.0f / 3.0f)));
	m_ContentBox.setFillColor(sf::Color(42, 42, 42, 255));

	//Create the buttons
	for (int i = 0; i < 2; i++)
	{
		m_Buttons.push_back(new Button());
		m_UI.push_back(m_Buttons.at(m_Buttons.size() - 1));
	}

	float fButtonGap = m_ContentBox.getSize().x / 30.0f; //The gap between each button
	float fButtonArea = (m_ContentBox.getSize().y * (2.0f / 3.0f)) - (fButtonGap); //The size in the y axis of where the buttons are kept
	float fButtonStart = m_ContentBox.getPosition().y + (m_ContentBox.getSize().y / 3.0f) + fButtonGap; //The y position of where the buttons are kept

																										//Setup every button
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		m_Buttons.at(i)->setColor(sf::Color(0, 0, 0, 0));
		m_Buttons.at(i)->setBackgroundColor(sf::Color(0, 186, 255, 255));
		m_Buttons.at(i)->setSize(sf::Vector2f(m_ContentBox.getSize().x / 2 - (fButtonGap * ((m_Buttons.size() + 1)/2.0f)), 50));
		m_Buttons.at(i)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + (m_Buttons.at(i)->getSize().x * i )+  (fButtonGap * (i + 1)),
												  m_ContentBox.getPosition().y + m_ContentBox.getSize().y - m_Buttons.at(i)->getSize().y - fButtonGap));
	}

	//Sets the button text
	m_Buttons.at(0)->setText("Cancel");
	m_Buttons.at(1)->setText("Accept");

	resolutionList.push_back(sf::Vector2f(800, 600));
	resolutionList.push_back(sf::Vector2f(1366, 768));
	resolutionList.push_back(sf::Vector2f(1920, 1080));

	for (int i = 0; i < 2; i++)
	{
		m_Sliders.push_back(new Slider);
		m_UI.push_back(m_Sliders.at(m_Sliders.size()-1));
	}

	m_Sliders.at(0)->setSize(sf::Vector2f(m_ContentBox.getSize().x - (fButtonGap * 2), 10));
	m_Titles.push_back(sf::Text(sf::String("Resolution"), *m_Fonts->getFont(0), 30));
	//m_Titles.at(0).setPosition(m_Sliders.at(0)->getPosition().x, m_Sliders.at(0)->getPosition().y - 30);
	m_Titles.at(0).setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap + (m_Sliders.at(0)->getSize().x / 2), fButtonStart) - sf::Vector2f(m_Titles.at(0).getLocalBounds().width / 2.0f, 0));

	m_Sliders.at(0)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap, m_Titles.at(0).getPosition().y + m_Titles.at(0).getLocalBounds().height + fButtonGap+ 10));
	m_Sliders.at(0)->setCursorTexture(m_Textures->getTexture(1));
	m_Sliders.at(0)->setNumberOfOptions(resolutionList.size());

	m_DataText.push_back(sf::Text(sf::String("Resolution"), *m_Fonts->getFont(0), 30));
	m_DataText.at(0).setPosition(m_Sliders.at(0)->getPosition().x, m_Sliders.at(0)->getPosition().y + 20);

	m_Sliders.at(1)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + fButtonGap, m_ContentBox.getPosition().y + 300));
	m_Sliders.at(1)->setSize(sf::Vector2f(m_ContentBox.getSize().x - (fButtonGap * 2), 10));
	m_Sliders.at(1)->setCursorTexture(m_Textures->getTexture(1));
	m_Sliders.at(1)->setNumberOfOptions(101);
	m_Sliders.at(1)->setLevel(m_CurrentSettings->getVolume());

	m_Titles.push_back(sf::Text(sf::String("Volume"), *m_Fonts->getFont(0), 30));
	m_Titles.at(1).setPosition(m_Sliders.at(1)->getPosition().x, m_Sliders.at(1)->getPosition().y - 30);

	m_DataText.push_back(sf::Text(sf::String("Volume"), *m_Fonts->getFont(0), 30));
	m_DataText.at(1).setPosition(m_Sliders.at(1)->getPosition().x, m_Sliders.at(1)->getPosition().y + 20);

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

	m_SoundManager = SoundManager::getInstance();
	m_TestSound.setBuffer(*m_SoundManager->getSound(10));
	m_TestSound.setVolume(m_CurrentSettings->getVolume());

	ActiveSlider = -1;
}

void Options::update(sf::Vector2i mousePos)
{
	//Checks buttons for a hovering mouse
	for (int i = 0; i < m_UI.size(); i++)
	{
		m_UI.at(i)->update(mousePos);
	}

	if (ActiveSlider != -1)
	{
		m_Sliders.at(ActiveSlider)->setCursorPos(mousePos);

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (ActiveSlider == 1)
			{
				m_TestSound.setVolume(m_Sliders.at(1)->getLevel());
				m_TestSound.play();
			}
			ActiveSlider = -1;
		}
	}
	else
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			for (int i = 0; i < m_Sliders.size(); i++)
			{
				if (m_Sliders.at(i)->hovering(mousePos))
				{
					ActiveSlider = i;
				}
			}
		}
	}

	m_DataText.at(0).setString(std::to_string(int(resolutionList.at(m_Sliders.at(0)->getLevel()).x)) + "x" + std::to_string(int(resolutionList.at(m_Sliders.at(0)->getLevel()).y)));
	m_DataText.at(1).setString(std::to_string(int(m_Sliders.at(1)->getLevel())));
}

int Options::clickLeft(sf::Vector2i mousePos)
{
	//Checks if the buttons have been clicked
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		if (m_Buttons.at(i)->hovering(mousePos))
		{
			if (i == 1)
			{
				m_CurrentSettings->setResolution(resolutionList.at(m_Sliders.at(0)->getLevel()));
				m_CurrentSettings->setVolume(m_Sliders.at(1)->getLevel());
			}
			return i + 1;
		}
	}
	return 0;
}

void Options::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Background);
	target.draw(m_ContentBox);
	target.draw(m_TitleText);

	for (int i = 0; i < m_Buttons.size(); i++)
	{
		target.draw(*m_Buttons.at(i));
	}
	for (int i = 0; i < m_Sliders.size(); i++)
	{
		target.draw(*m_Sliders.at(i));
	}
	for (int i = 0; i < m_Titles.size(); i++)
	{
		target.draw(m_Titles.at(i));
	}
	for (int i = 0; i < m_DataText.size(); i++)
	{
		target.draw(m_DataText.at(i));
	}
}

Options::~Options()
{
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		delete(m_Buttons.at(i));
		m_Buttons.at(i) = NULL;
	}

	for (int i = 0; i < m_Sliders.size(); i++)
	{
		delete(m_Sliders.at(i));
		m_Sliders.at(i) = NULL;
	}
}