#include "../include/Button.h"

Button::Button()
{

}

bool Button::clicked(sf::Vector2i pos)
{
	//If the position is within the bounds of the button return true
	if (pos.x > m_MainSprite.getGlobalBounds().left &&
		pos.x < m_MainSprite.getGlobalBounds().left + m_MainSprite.getGlobalBounds().width &&
		pos.y > m_MainSprite.getGlobalBounds().top &&
		pos.y <  m_MainSprite.getGlobalBounds().top + m_MainSprite.getGlobalBounds().height)
	{
		return true;
	}
	return false;
}