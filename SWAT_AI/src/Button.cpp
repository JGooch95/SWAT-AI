#include "../include/Button.h"

Button::Button()
{

}

bool Button::clicked(sf::Vector2i pos)
{
	if (pos.x > m_MainSprite.getGlobalBounds().left &&
		pos.x < m_MainSprite.getGlobalBounds().left + m_MainSprite.getGlobalBounds().width &&
		pos.y > m_MainSprite.getGlobalBounds().top &&
		pos.y <  m_MainSprite.getGlobalBounds().top + m_MainSprite.getGlobalBounds().height)
	{
		return true;
	}
	return false;
}