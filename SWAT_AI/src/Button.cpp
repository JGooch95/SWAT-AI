#include "../include/Button.h"

Button::Button()
{
	m_FontManager = FontManager::getInstance();

	m_ButtonText.setFont(*m_FontManager->getFont(0));
	m_ButtonText.setFillColor(sf::Color(255, 255, 255, 255));
	m_ButtonText.setCharacterSize(100.0f);

	setBackgroundColor(sf::Color(100, 100, 100, 255));

	setCollisionBox(sf::FloatRect(getPosition().x,
		getPosition().y,
		getSize().x,
		getSize().y));
}

void Button::update(sf::Vector2i mousePos)
{
	m_Backdrop.setSize(m_MainSprite.getSize());
	m_Backdrop.setPosition(m_MainSprite.getPosition());

	setCollisionBox(sf::FloatRect(getPosition().x,
		getPosition().y,
		getSize().x,
		getSize().y));

	if (hovering(mousePos))
	{
		m_Backdrop.setFillColor(m_DefaultColor + sf::Color(30, 30, 30, m_Backdrop.getFillColor().a));
	}
	else
	{
		m_Backdrop.setFillColor(m_DefaultColor);
	}
}

bool Button::hovering(sf::Vector2i pos)
{
	//If the position is within the bounds of the button return true and increase the background brightness
	if (pos.x >= m_CollisionBox.left  &&
		pos.x <= m_CollisionBox.left + m_CollisionBox.width &&
		pos.y >= m_CollisionBox.top &&
		pos.y <= m_CollisionBox.top + m_CollisionBox.height)
	{
		return true;
	}

	return false;
}

void Button::setText(std::string sValue)
{
	//Sets the buttons string
	m_ButtonText.setString(sValue);
	
	//Resizes the characters to fit within the button
	while (m_ButtonText.getLocalBounds().width  > m_MainSprite.getSize().x ||
		   m_ButtonText.getLocalBounds().height > m_MainSprite.getSize().y*(2.0f / 3.0f))
	{
		m_ButtonText.setCharacterSize(m_ButtonText.getCharacterSize() - 1);
	}

	//Centers the text within the button
	m_ButtonText.setPosition(m_MainSprite.getPosition() + sf::Vector2f(getSize().x/2.0f,0.0f) - sf::Vector2f(m_ButtonText.getLocalBounds().width / 2.0f, 0.0f));
}

void Button::setBackgroundColor(sf::Color newColor)
{
	m_DefaultColor = newColor;
	m_Backdrop.setFillColor(newColor);
}

void Button::setCollisionBox(sf::FloatRect newArea)
{
	m_CollisionBox = newArea;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_Backdrop);
	target.draw(m_MainSprite);
	target.draw(m_ButtonText);
}