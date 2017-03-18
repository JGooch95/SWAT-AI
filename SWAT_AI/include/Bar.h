#pragma once

#include <SFML/Graphics.hpp>

class Bar : public sf::Drawable
{
	private:
		sf::RectangleShape m_BackBox;
		sf::RectangleShape m_FrontBox;
		sf::Vector2f m_LevelLimits;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Bar();
		void setSize(sf::Vector2f newDimensions);
		void setPosition(sf::Vector2f newPosition);
		void setLevelColor(sf::Color newColor);
		void setBarColor(sf::Color newColor);
		void setLevel(float fLevel);
		void setLimit(float fLimit);
		void update();

		sf::Vector2f getSize();
		sf::Vector2f getPosition();
		sf::Vector2f getLevelLimits();
};