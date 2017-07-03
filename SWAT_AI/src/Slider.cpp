#include "..\include\Slider.h"

Slider::Slider()
{
	setColor(sf::Color(255,255,255,125));

	setSize(sf::Vector2f(100, 10));
	setCursorSize(sf::Vector2f(20, 20));
	m_Cursor.setPosition(sf::Vector2f(getPosition().x, getPosition().y + (getSize().y / 2)));
	m_Cursor.setColor(sf::Color(0, 186, 255, 255));
	m_Cursor.setBackgroundColor(sf::Color(0, 0, 0, 0));

	m_NumberOfOptions = 9;
}

void Slider::update(sf::Vector2i mousePos)
{
	setCollisionBox(sf::FloatRect(m_MainSprite.getGlobalBounds().left,
		m_Cursor.getRect().top,
		m_MainSprite.getGlobalBounds().width,
		m_Cursor.getSize().y));

	m_Cursor.setPosition(getPosition() + sf::Vector2f(m_CursorLevel * (getSize().x / m_NumberOfOptions), getSize().y / 2));
}

void Slider::setCursorTexture(sf::Texture* tex2)
{
	m_Cursor.setTexture(tex2);
	m_Cursor.setColor(sf::Color(255,255,255, 255));
}

void Slider::setCursorColour(sf::Color newColour)
{
	m_Cursor.setBackgroundColor(newColour);
}

void Slider::setCursorSize(sf::Vector2f newSize)
{
	m_Cursor.setSize(newSize);
	m_Cursor.setOrigin(m_Cursor.getSize() / 2.0f);
}

void Slider::setNumberOfOptions(int iNumber)
{
	iNumber--;

	m_NumberOfOptions = iNumber;

	//If the cursor level is outside the range change the level
	if (m_CursorLevel > m_NumberOfOptions)
	{
		m_CursorLevel = m_NumberOfOptions;
	}
	if (m_CursorLevel < 0)
	{
		m_CursorLevel = 0;
	}
}

void Slider::setCursorPos(sf::Vector2i mousePos)
{
	if (mousePos.x > m_MainSprite.getPosition().x + m_MainSprite.getSize().x)
	{
		m_CursorLevel = m_NumberOfOptions;
	}
	else if (mousePos.x < m_MainSprite.getPosition().x)
	{
		m_CursorLevel = 0;
	}
	else
	{
		float fProgress = (mousePos.x - m_MainSprite.getPosition().x) / m_MainSprite.getSize().x;
		m_CursorLevel = rint(m_NumberOfOptions * fProgress);
	}
}

void Slider::setLevel(int iNewLevel)
{
	m_CursorLevel = iNewLevel;
}

sf::Vector2f Slider::getCursorSize()
{
	return m_Cursor.getSize();
}

int Slider::getNumberOfOptions()
{
	return m_NumberOfOptions;
}

int Slider::getLevel()
{
	return m_CursorLevel;
}

void Slider::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
	target.draw(m_Cursor);
}

Slider::~Slider()
{
}
