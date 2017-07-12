#include "../include/Options.h"

Options::Options(sf::Vector2u windowSize)
{
	//Sets up the background
	m_Background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_Background.setFillColor(sf::Color(77, 77, 78, 255));
	m_Background.setPosition(0, 0);

	//Sets up the box containing the content
	m_ContentBox.setPosition(windowSize.x / 3.0f, windowSize.y* (1.0f / 6.0f));
	m_ContentBox.setSize(sf::Vector2f(windowSize.x / 3.0f, windowSize.y * (2.0f / 3.0f)));
	m_ContentBox.setFillColor(sf::Color(42, 42, 42, 255));

	m_fButtonGap = m_ContentBox.getSize().x / 30.0f; //The gap between each button

	//Sets up the title
	m_TitleText.setString("SWAT AI");
	m_TitleText.setFont(*m_Fonts->getFont(0));
	m_TitleText.setPosition(sf::Vector2f(m_ContentBox.getPosition().x + m_fButtonGap, m_ContentBox.getPosition().y + m_fButtonGap));

	//Fits the title to be within the bounds of the content box
	m_TitleText.setCharacterSize(200);
	while (m_TitleText.getLocalBounds().width > m_ContentBox.getSize().x - (2 * m_fButtonGap) ||
		   m_TitleText.getLocalBounds().height > (m_ContentBox.getSize().y / 3) - (2 * m_fButtonGap))
	{
		m_TitleText.setCharacterSize(m_TitleText.getCharacterSize() - 1);
	}

	//Creates the sliders
	for (int i = 0; i < 2; i++)
	{
		m_vSliders.push_back(new Slider);

		//Set general slider settings
		m_vSliders.at(i)->setSize(sf::Vector2f(m_ContentBox.getSize().x - (m_fButtonGap * 2), (m_ContentBox.getSize().x - (m_fButtonGap * 2)) / 20));
		m_vSliders.at(i)->setCursorSize(sf::Vector2f(m_vSliders.at(0)->getSize().y * 2, m_vSliders.at(0)->getSize().y * 2));
		m_vSliders.at(i)->setCursorTexture(m_Textures->getTexture(1));

		m_vUI.push_back(m_vSliders.at(m_vSliders.size()-1));

	}

	//Resolution Slider Title
	m_vTitles.push_back(sf::Text(sf::String("Resolution"), *m_Fonts->getFont(0), m_vSliders.at(0)->getSize().y * 2));
	m_vTitles.at(0).setPosition(sf::Vector2f(m_ContentBox.getPosition().x + m_fButtonGap + (m_vSliders.at(0)->getSize().x / 2), m_TitleText.getPosition().y + m_TitleText.getLocalBounds().height + m_fButtonGap + (m_ContentBox.getSize().y / 5)) - sf::Vector2f(m_vTitles.at(0).getLocalBounds().width / 2.0f, 0));

	//Resolution Slider 
	m_vSliders.at(0)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + m_fButtonGap, m_vTitles.at(0).getPosition().y + m_vTitles.at(0).getLocalBounds().height + m_fButtonGap));
	loadResolutionList("./Assets/Options/resList.txt");

	//Resolution Data Text
	m_vDataText.push_back(sf::Text(sf::String(std::to_string((int)m_CurrentSettings->getResolution().x) + "x" + std::to_string((int)m_CurrentSettings->getResolution().y)), *m_Fonts->getFont(0), m_vSliders.at(0)->getSize().y * 2));
	m_vDataText.at(0).setPosition((m_ContentBox.getPosition().x + m_fButtonGap + (m_vSliders.at(0)->getSize().x / 2)) - (m_vDataText.at(0).getLocalBounds().width / 2.0f), m_vSliders.at(0)->getPosition().y + m_fButtonGap);

	//Sound Volume Slider Title
	m_vTitles.push_back(sf::Text(sf::String("Volume"), *m_Fonts->getFont(0), m_vSliders.at(0)->getSize().y * 2));
	m_vTitles.at(1).setPosition((m_ContentBox.getPosition().x + m_fButtonGap + (m_vSliders.at(0)->getSize().x / 2)) - (m_vTitles.at(1).getLocalBounds().width / 2.0f), m_vDataText.at(0).getPosition().y + m_vDataText.at(0).getLocalBounds().height + m_fButtonGap);

	//Sound Volume Slider
	m_vSliders.at(1)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + m_fButtonGap, m_vTitles.at(1).getPosition().y + m_vTitles.at(1).getLocalBounds().height + m_fButtonGap));
	m_vSliders.at(1)->setNumberOfOptions(101);
	m_vSliders.at(1)->setLevel(m_CurrentSettings->getVolume());

	//Sound Volume Data Text
	m_vDataText.push_back(sf::Text(sf::String(std::to_string((int)m_CurrentSettings->getVolume())), *m_Fonts->getFont(0), m_vSliders.at(1)->getSize().y * 2));
	m_vDataText.at(1).setPosition((m_ContentBox.getPosition().x + m_fButtonGap + (m_vSliders.at(1)->getSize().x / 2)) - (m_vDataText.at(1).getLocalBounds().width / 2.0f), m_vSliders.at(1)->getPosition().y + m_fButtonGap);

	//Sets up the sound for the volume slider
	m_TestSound.setBuffer(*m_SoundManager->getSound(10));
	m_TestSound.setVolume(m_CurrentSettings->getVolume());

	int iNumOfButtons = 2;
	//Create the buttons
	for (int i = 0; i < iNumOfButtons; i++)
	{
		m_vButtons.push_back(new Button());
		m_vButtons.at(i)->setColor(sf::Color(0, 0, 0, 0));
		m_vButtons.at(i)->setBackgroundColor(sf::Color(0, 186, 255, 255));
		m_vButtons.at(i)->setSize(sf::Vector2f(m_ContentBox.getSize().x / 2 - (m_fButtonGap * ((iNumOfButtons + 1) / 2.0f)), 50));
		m_vButtons.at(i)->setPosition(sf::Vector2f(m_ContentBox.getPosition().x + (m_vButtons.at(i)->getSize().x * i) + (m_fButtonGap * (i + 1)),
												  m_ContentBox.getPosition().y + m_ContentBox.getSize().y - m_vButtons.at(i)->getSize().y - m_fButtonGap));
		m_vUI.push_back(m_vButtons.at(m_vButtons.size() - 1));
	}

	//Sets the button text
	m_vButtons.at(0)->setText("Cancel");
	m_vButtons.at(1)->setText("Accept");

	m_iActiveSlider = -1; //Sets no sliders to be active
}

void Options::update(sf::Vector2i mousePos)
{
	//Updates all UI objects
	for (int i = 0; i < m_vUI.size(); i++)
	{
		m_vUI.at(i)->update(mousePos);
	}

	if (m_iActiveSlider != -1) //If there is an active slider
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) //If the left mouse is removed
		{
			//IF the slider used was the volume slider
			if (m_iActiveSlider == 1)
			{
				//Set the volume and play an indicator
				m_TestSound.setVolume(m_vSliders.at(1)->getLevel());
				m_TestSound.play();
			}
			m_iActiveSlider = -1; //Set the active slider to none
		}
		else
		{
			m_vSliders.at(m_iActiveSlider)->setCursorPos(mousePos); //Update the slider cursor position
			m_ContentBox.getSize().x / 30.0f; //The gap between each button

			//Update the slider text and positioning
			switch (m_iActiveSlider)
			{
				case 0:
					m_vDataText.at(0).setString(std::to_string(int(resolutionList.at(m_vSliders.at(0)->getLevel()).x)) + "x" + std::to_string(int(resolutionList.at(m_vSliders.at(0)->getLevel()).y)));
					m_vDataText.at(0).setPosition((m_ContentBox.getPosition().x + m_fButtonGap + (m_vSliders.at(0)->getSize().x / 2)) - (m_vDataText.at(0).getLocalBounds().width / 2.0f), m_vSliders.at(0)->getPosition().y + m_fButtonGap);
					break;
				case 1:
					m_vDataText.at(1).setString(std::to_string(int(m_vSliders.at(1)->getLevel())));
					m_vDataText.at(1).setPosition((m_ContentBox.getPosition().x + m_fButtonGap + (m_vSliders.at(1)->getSize().x / 2)) - (m_vDataText.at(1).getLocalBounds().width / 2.0f), m_vSliders.at(1)->getPosition().y + m_fButtonGap);
					break;
			}
		}
	}
	else //If there is no active slider
	{
		//Check if any sliders are being clicked on
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			for (int i = 0; i < m_vSliders.size(); i++)
			{
				if (m_vSliders.at(i)->hovering(mousePos))
				{
					m_iActiveSlider = i; //Set the active slider
				}
			}
		}
	}
}

int Options::processInput(sf::Event keyCode, sf::Vector2i mousePos)
{
	if (keyCode.type == sf::Event::MouseButtonPressed)
	{
		switch (keyCode.key.code)
		{
			case sf::Mouse::Left:
				//Checks if the buttons have been clicked
				for (int i = 0; i < m_vButtons.size(); i++)
				{
					if (m_vButtons.at(i)->hovering(mousePos))
					{
						if (i == 1) //If Accept is clicked
						{
							//Set the new values
							m_CurrentSettings->setResolution(resolutionList.at(m_vSliders.at(0)->getLevel()));
							m_CurrentSettings->setVolume(m_vSliders.at(1)->getLevel());
							m_CurrentSettings->save("./Assets/Options/settings.txt");
							m_CurrentSettings->updateWindow(true);
						}
						return S_Menu;
					}
				}
				break;
		}
	}

	return S_None;
}

void Options::loadResolutionList(std::string sDir)
{
	std::ifstream resFile;
	resFile.open(sDir); //Open the file

	if (resFile.is_open()) //If the file opened correctly
	{
		//Initialise reading variables
		std::string sLine;
		std::string sToken;
		int iCounter = 0;

		while (!resFile.eof()) //while the end of file hasnt been reached
		{
			getline(resFile, sLine); //Get the next line
			
			std::istringstream sWord(sLine); //Adds the line to a string stream

			if (sLine != "") //If the line isnt empty 
			{
				sf::Vector2f res;
				
				//Read the x coordinate
				sWord >> sToken;
				res.x = stoi(sToken);

				//Read the y coordinate
				sWord >> sToken;
				res.y = stoi(sToken);

				resolutionList.push_back(res); //Add the resolution to the list

				//If the current resolution is in the list set the slider level to the resolution
				if (res == m_CurrentSettings->getResolution())
				{
					m_vSliders.at(0)->setLevel(iCounter);
				}
				iCounter++;
			}
		}
		m_vSliders.at(0)->setNumberOfOptions(resolutionList.size()); //Change the number of options for the slider
	}
	else
	{
		//Ouptut an error
		std::cout << "Resolution File: " << sDir << " could not be opened." << "\n";
	}

	resFile.close();
}

void Options::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Background);
	target.draw(m_ContentBox);
	target.draw(m_TitleText);

	for (int i = 0; i < m_vButtons.size(); i++)
	{
		target.draw(*m_vButtons.at(i));
	}
	for (int i = 0; i < m_vSliders.size(); i++)
	{
		target.draw(*m_vSliders.at(i));
	}
	for (int i = 0; i < m_vTitles.size(); i++)
	{
		target.draw(m_vTitles.at(i));
	}
	for (int i = 0; i < m_vDataText.size(); i++)
	{
		target.draw(m_vDataText.at(i));
	}
}

Options::~Options()
{
	for (int i = 0; i < m_vButtons.size(); i++)
	{
		delete(m_vButtons.at(i));
		m_vButtons.at(i) = NULL;
	}

	for (int i = 0; i < m_vSliders.size(); i++)
	{
		delete(m_vSliders.at(i));
		m_vSliders.at(i) = NULL;
	}
}