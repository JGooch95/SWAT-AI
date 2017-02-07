#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Game.h"


int main()
{
	//Sets up the window
	sf::RenderWindow MainWindow(sf::VideoMode(1920, 1080), "SWAT AI");
	
	//Initialises the event value for input readings
	sf::Event event;

	//Sets up the timer for tracking the frame times
	sf::Clock Timer;

	//Sets up the game ready for use
	Game Game1(MainWindow.getSize());

	const float kfTargetFPS = 60; //Holds the target frames per second

	//Main Loop
	while (MainWindow.isOpen())
	{
		//Inputs
		while (MainWindow.pollEvent(event))
		{
			//If the Close button is pushed close the software
			if (event.type == sf::Event::Closed)
			{
				MainWindow.close();
				return 0;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Right)
				{
					Game1.ClickRight(sf::Mouse::getPosition() - MainWindow.getPosition() - sf::Vector2i(8, 30));
				}
			}
		}

		if (Timer.getElapsedTime().asSeconds() > 1.0f / kfTargetFPS)
		{
			//Update
			Game1.Update(sf::Mouse::getPosition() - MainWindow.getPosition());

			//Draw
			MainWindow.clear(sf::Color(0,0,0,0));
			MainWindow.draw(Game1);
			MainWindow.display();

			Timer.restart(); //Resets the timer for the next frame check
		}

	}
}