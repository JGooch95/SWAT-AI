#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Game.h"
#include "../include/TextureLoader.h"
#include "../include/Menu.h"
#include "../include/Editor.h"
#include "../include/SoundManager.h"


int main()
{
	//Sets up the window
	sf::RenderWindow mainWindow(sf::VideoMode(1920,1080), "SWAT AI", sf::Style::Close);

	//Initialises the event value for input readings
	sf::Event event;

	//Sets up the timer for tracking the frame times
	sf::Clock timer;

	TextureLoader* m_Textures = TextureLoader::getInstance();
	//Loads the main textures into the software
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
		"Assets/Sprites/Save.png"						//31
	});

	SoundManager* m_SoundManger = SoundManager::getInstance();
	
	m_SoundManger->loadSounds(std::vector<std::string>{
		"Assets/Sounds/Silenced.wav",
		"Assets/Sounds/AssaultRifle.wav",
		"Assets/Sounds/Sniper.wav",
		"Assets/Sounds/Shotgun.wav",
		"Assets/Sounds/Footstep.wav",
		"Assets/Sounds/Door.wav",
		"Assets/Sounds/AssaultRifleReload.wav",
		"Assets/Sounds/loadout.wav",
		"Assets/Sounds/ShotgunReload.wav",
		"Assets/Sounds/SniperReload.wav"
	});

	m_Textures->getTexture(14)->setRepeated(true);

	Map* m_CurrentMap = NULL;
	Settings* m_CurrentSettings = NULL;

	//Sets up the game ready for use
	Game* Game1 = NULL;
	Editor* Editor1 = NULL;
	Menu* Menu1 = new Menu(mainWindow.getSize());
	const float kfTargetFPS = 60; //Holds the target frames per second

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
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::D)
				{
					if (Game1 != NULL)
					{
						Game1->enableDebug();
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Right)
				{
					if (Game1 != NULL)
					{
						Game1->clickRight(sf::Mouse::getPosition(mainWindow));
					}
				}
				if (event.key.code == sf::Mouse::Left)
				{
					if (Game1 != NULL)
					{
						switch (Game1->clickLeft(sf::Mouse::getPosition(mainWindow)))
						{
							case 1:
								delete Game1;
								Game1 = NULL;
								Menu1 = new Menu(mainWindow.getSize());
								break;
						}
					}
					else if (Menu1 != NULL)
					{
						switch (Menu1->clickLeft(sf::Mouse::getPosition(mainWindow)))
						{
							case 1:
								delete Menu1;
								Menu1 = NULL;
								Game1 = new Game(mainWindow.getSize());
								break;

							case 2:
								delete Menu1;
								Menu1 = NULL;
								Editor1 = new Editor(mainWindow.getSize());
								break;

							case 4:
								delete Menu1;
								Menu1 = NULL;
								mainWindow.close();
								break;
						}
					}
					else if (Editor1 != NULL)
					{
						switch (Editor1->clickLeft(sf::Mouse::getPosition(mainWindow)))
						{
							case 1:
								delete Editor1;
								Editor1 = NULL;
								Menu1 = new Menu(mainWindow.getSize());
								break;
						}
					}
				}
			}
		}

		if (timer.getElapsedTime().asSeconds() > 1.0f / kfTargetFPS)
		{
			//Update
			if (Game1 != NULL)
			{
				Game1->update(sf::Mouse::getPosition(mainWindow));
			}

			if (Menu1 != NULL)
			{
				Menu1->update(sf::Mouse::getPosition(mainWindow));
			}
			if (Editor1 != NULL)
			{
				Editor1->update(sf::Mouse::getPosition(mainWindow));
			}

			//Draw
			mainWindow.clear(sf::Color(0,0,0,0));
			if (Game1 != NULL)
			{
				mainWindow.draw(*Game1);
			}
			if (Menu1 != NULL)
			{
				mainWindow.draw(*Menu1);
			}
			if (Editor1 != NULL)
			{
				mainWindow.draw(*Editor1);
			}
			mainWindow.display();

			timer.restart(); //Resets the timer for the next frame check
		}

	}

	if (Game1 != NULL)
	{
		delete Game1;
		Game1 = NULL;
	}
	if (Menu1 != NULL)
	{
		delete Menu1;
		Menu1 = NULL;
	}
	if (Editor1 != NULL)
	{
		delete Editor1;
		Editor1 = NULL;
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