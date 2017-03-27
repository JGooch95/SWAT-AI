#pragma once

#include <SFML/Graphics.hpp>
#include "MathUtils.h"

class Bar : public sf::Drawable
{
	private:
		sf::RectangleShape m_BackBox; //The backdrop of the bar
		sf::RectangleShape m_FrontBox; //The level bar of the bar
		Util::Limits m_LevelLimits; // The values for the bar (current Level : Max Limit)
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Bar(); //Constructor
		void update(); //Updates the visuals of the bar to the current data

		//Setters
		void setSize(sf::Vector2f newDimensions);
		void setPosition(sf::Vector2f newPosition);
		void setLevelColor(sf::Color newColor);
		void setBarColor(sf::Color newColor);
		void setLevel(float fLevel);
		void setLimit(float fLimit);

		//Getters
		sf::Vector2f getSize();
		sf::Vector2f getPosition();
		sf::Vector2f getLevelLimits();
};