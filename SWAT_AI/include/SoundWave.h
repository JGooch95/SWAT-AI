#pragma once
#include <SFML/Graphics.hpp>

class soundWave :public sf::Drawable
{
	private:
		sf::CircleShape m_Wave;
		float m_fDesiredRadius;
		float m_fCurrentRadius;
		float m_fIncreaseRate;
		float m_fDecreaseRate;
		int iDirection;
		bool m_bDone;

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	public:
		soundWave();
		soundWave(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position);

		void update();
		bool isDone();
};