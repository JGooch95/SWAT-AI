#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Game.h"
#include "../include/TextureLoader.h"
#include "../include/Menu.h"
#include "../include/Editor.h"
#include "../include/SoundManager.h"
#include "../include/FontManager.h"
#include "../include/MathUtils.h"
#include "../include/Options.h"

int main()
{
	Settings* m_CurrentSettings = Settings::getInstance();
	m_CurrentSettings->load("./Assets/Options/settings.txt");

	//Sets up the window
	sf::RenderWindow mainWindow(sf::VideoMode(m_CurrentSettings->getResolution().x, m_CurrentSettings->getResolution().y), "SWAT AI", sf::Style::Close);

	//Initialises the event value for input readings
	sf::Event event;

	//Sets up the timer for tracking the frame times
	sf::Clock timer;

	//Loads the textures
	TextureLoader* m_Textures = TextureLoader::getInstance();
	m_Textures->loadTextures(std::vector<std::string>{
		"Assets/Sprites/Wood.jpg",						//0
		"Assets/Sprites/Unit.png",						//1
		"Assets/Sprites/Enemy.png",						//2
		"Assets/Sprites/SniperIcon.png",				//3
		"Assets/Sprites/Sniper.png",					//4
		"Assets/Sprites/LMG.png",						//5
		"Assets/Sprites/Shotgun.png",					//6
		"Assets/Sprites/Assault.png",					//7
		"Assets/Sprites/SupportIcon.png",				//8
		"Assets/Sprites/ShotgunnerIcon.png",			//9
		"Assets/Sprites/AssaultIcon.png",				//10
		"Assets/Sprites/Muzzle.png",					//11
		"Assets/Sprites/Health.png",					//12
		"Assets/Sprites/Ammo.png",						//13
		"Assets/Sprites/Grass.jpg",						//14
		"Assets/Sprites/Empty.png",						//15
		"Assets/Sprites/Lazer.png",						//16
		"Assets/Sprites/Silencer.png",					//17
		"Assets/Sprites/Scope.png",						//18
		"Assets/Sprites/ScopeSprite.png",				//19
		"Assets/Sprites/Back.png",						//20
		"Assets/Sprites/Plus.png",						//21
		"Assets/Sprites/Minus.png",						//22
		"Assets/Sprites/Eraser.png",					//23
		"Assets/Sprites/Door.png",						//24
		"Assets/Sprites/Floor.png",						//25
		"Assets/Sprites/Concrete.png",					//26
		"Assets/Sprites/KitchenTile.png",				//27
		"Assets/Sprites/BlueCarpet.jpg",				//28
		"Assets/Sprites/RedCarpet.jpg",					//29
		"Assets/Sprites/Bricks.jpg",					//30
		"Assets/Sprites/Save.png",						//31
		"Assets/Sprites/Dead.png",						//32
		"Assets/Sprites/Select.png",					//33
		"Assets/Sprites/PathMaker.png",					//34
		"Assets/Sprites/Grenade.png",					//35
		"Assets/Sprites/Flashbang.png",					//36
		"Assets/Sprites/Rock.png"						//37
	});

	//Loads the sounds
	SoundManager* m_SoundManger = SoundManager::getInstance();
	m_SoundManger->loadSounds(std::vector<std::string>{
		"Assets/Sounds/Silenced.wav",					//0
		"Assets/Sounds/AssaultRifle.wav",				//1
		"Assets/Sounds/Sniper.wav",						//2
		"Assets/Sounds/Shotgun.wav",					//3
		"Assets/Sounds/Footstep.wav",					//4
		"Assets/Sounds/Door.wav",						//5
		"Assets/Sounds/AssaultRifleReload.wav",			//6
		"Assets/Sounds/loadout.wav",					//7
		"Assets/Sounds/ShotgunReload.wav",				//8
		"Assets/Sounds/SniperReload.wav",				//9
		"Assets/Sounds/Death.wav"						//10
	});

	//Loads the fonts
	FontManager* m_FontManager = FontManager::getInstance();
	m_FontManager->loadFonts(std::vector<std::string>{
		"Assets/Fonts/arial.ttf"						//0
	});

	Map* m_CurrentMap = NULL;

	const float kfTargetFPS = 60; //Holds the target frames per second
	mainWindow.setKeyRepeatEnabled(false);

	//Sets the window Icon to the logo
	mainWindow.setIcon(m_Textures->getTexture(3)->copyToImage().getSize().x,
					   m_Textures->getTexture(3)->copyToImage().getSize().y,
					   m_Textures->getTexture(3)->copyToImage().getPixelsPtr());

	ContentScreen* currentScreen = new Menu(mainWindow.getSize());

	//Main Loop
	while (mainWindow.isOpen())
	{
		//Inputs
		while (mainWindow.pollEvent(event))
		{
			//If the Close button is pushed close the software
			if (event.type == sf::Event::Closed)
			{
				mainWindow.close();
			}

			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::D)
				{
					//Toggles Debug mode if the settings have been initialised
					if (m_CurrentSettings != NULL)
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
				}
			}

			if (currentScreen != NULL)
			{
				if (currentScreen->processInput(event, sf::Mouse::getPosition(mainWindow)) == S_Game ||
					currentScreen->processInput(event, sf::Mouse::getPosition(mainWindow)) == S_Menu ||
					currentScreen->processInput(event, sf::Mouse::getPosition(mainWindow)) == S_Editor ||
					currentScreen->processInput(event, sf::Mouse::getPosition(mainWindow)) == S_Options ||
					currentScreen->processInput(event, sf::Mouse::getPosition(mainWindow)) == Exit)
				{
					int iScreenType = currentScreen->processInput(event, sf::Mouse::getPosition(mainWindow));

					if (m_CurrentSettings->getUpdateState())
					{
						m_CurrentSettings->updateWindow(false);
						mainWindow.setSize(sf::Vector2u(m_CurrentSettings->getResolution()));
						mainWindow.setView(sf::View(sf::FloatRect(0, 0, m_CurrentSettings->getResolution().x, m_CurrentSettings->getResolution().y)));
					}

					delete currentScreen;
					currentScreen = NULL;

					switch (iScreenType)
					{
						case S_Game:
							currentScreen = new Game(mainWindow.getSize());
							break;

						case S_Menu:
							currentScreen = new Menu(mainWindow.getSize());
							break;

						case S_Editor:
							currentScreen = new Editor(mainWindow.getSize());
							break;

						case S_Options:
							currentScreen = new Options(mainWindow.getSize());
							break;

						case Exit:
							mainWindow.close();
							break;
					}
				}
			}
		}

		if (currentScreen != NULL)
		{
			if (timer.getElapsedTime().asSeconds() > 1.0f / kfTargetFPS)
			{
				currentScreen->update(sf::Mouse::getPosition(mainWindow));
				timer.restart(); //Resets the timer for the next frame check
			}

			mainWindow.clear(sf::Color(0, 0, 0, 0));
			mainWindow.draw(*currentScreen);
		}

		mainWindow.display();
	}

	if (currentScreen != NULL)
	{
		delete currentScreen;
		currentScreen = NULL;
	}

	delete m_Textures;
	m_Textures = NULL;

	if (m_CurrentMap != NULL)
	{
		delete(m_CurrentMap);
		m_CurrentMap = NULL;
	}
	if (m_CurrentSettings != NULL)
	{
		delete(m_CurrentSettings);
		m_CurrentSettings = NULL;
	}

	delete m_SoundManger;
	m_SoundManger = NULL;

}