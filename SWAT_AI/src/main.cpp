#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Game.h"


int main()
{
	//Sets up the window
	sf::RenderWindow mainWindow(sf::VideoMode(1920, 1080), "SWAT AI", sf::Style::Close);

	//Initialises the event value for input readings
	sf::Event event;

	//Sets up the timer for tracking the frame times
	sf::Clock timer;

	//Sets up the game ready for use
	Game Game1(mainWindow.getSize());

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
				return 0;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Right)
				{
					Game1.clickRight(sf::Mouse::getPosition() - mainWindow.getPosition() - sf::Vector2i(8, 30));
				}
				if (event.key.code == sf::Mouse::Left)
				{
					Game1.clickLeft(sf::Mouse::getPosition() - mainWindow.getPosition() - sf::Vector2i(8, 30));
				}
			}
		}

		if (timer.getElapsedTime().asSeconds() > 1.0f / kfTargetFPS)
		{
			//Update
			Game1.update(sf::Mouse::getPosition() - mainWindow.getPosition());

			//Draw
			mainWindow.clear(sf::Color(0,0,0,0));
			mainWindow.draw(Game1);
			mainWindow.display();

			timer.restart(); //Resets the timer for the next frame check
		}

	}
}