#pragma once
#include <SFML/Graphics.hpp>

class soundWave :public sf::Drawable
{
	private:
		sf::CircleShape m_Wave; //Holds the wave being emitted
		float m_fDesiredRadius; //Holds the radius where the wave ends
		float m_fIncreaseRate; //Holds the rate at which the radius increases
		float m_fDecreaseRate;  //Holds the rate at which the radius decreases

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	public:
		soundWave(); //Default Constructor
		soundWave(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position);
		
		void update(); //Updates the waves data
		bool isDone(); //States whether the the wave has reached the desired radius
		sf::Vector2f getPosition();
		float getRadius();
};