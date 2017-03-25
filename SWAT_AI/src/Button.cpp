#include "../include/Button.h"

Button::Button()
{
	m_CurrentFont.loadFromFile("Assets/Fonts/arial.ttf");

	m_ButtonText.setFont(m_CurrentFont);
	m_bUsingText = false;
	m_ButtonText.setFillColor(sf::Color(255, 255, 255, 255));
	m_ButtonText.setCharacterSize(100.0f);
	m_ButtonText.setPosition(0, 0);
	setText("");
}

bool Button::hovering(sf::Vector2i pos)
{
	//If the position is within the bounds of the button return true
	if (pos.x > m_MainSprite.getPosition().x &&
		pos.x < m_MainSprite.getPosition().x + m_MainSprite.getSize().x &&
		pos.y > m_MainSprite.getPosition().y &&
		pos.y <  m_MainSprite.getPosition().y + m_MainSprite.getSize().y)
	{
		m_Selector.setSize(m_MainSprite.getSize());
		m_Selector.setPosition(m_MainSprite.getPosition());
		m_Selector.setFillColor(sf::Color(255,255,255,30));
		return true;
	}
	m_Selector.setFillColor(sf::Color(255, 255, 255, 0));
	return false;
}

void Button::setText(std::string sValue)
{
	m_bUsingText = true;
	m_ButtonText.setString(sValue);

	while (m_ButtonText.getLocalBounds().width > getSize().x &&m_ButtonText.getLocalBounds().height > getSize().y)
	{
		m_ButtonText.setCharacterSize(m_ButtonText.getCharacterSize() - 1);
	}

	m_ButtonText.setPosition(m_MainSprite.getPosition() + sf::Vector2f(getSize().x/2.0f,0) - sf::Vector2f(m_ButtonText.getLocalBounds().width / 2.0f, 0));
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
	target.draw(m_ButtonText);
	target.draw(m_Selector);
}