#include "..\include\Slider.h"

Slider::Slider()
{
	//Sets the bar data
	setColor(sf::Color(255,255,255,125));
	setSize(sf::Vector2f(100.0f, 10.0f));
	m_iNumberOfOptions = 9;

	//Sets the cursor data
	setCursorSize(sf::Vector2f(20.0f, 20.0f));
	m_Cursor.setPosition(sf::Vector2f(getPosition().x, getPosition().y + (getSize().y / 2)));
	m_Cursor.setColor(sf::Color(0, 186, 255, 255));
	m_Cursor.setBackgroundColor(sf::Color(0, 0, 0, 0));
}

void Slider::update(sf::Vector2i mousePos)
{
	//Sets the collision boxes to go around entire slider
	setCollisionBox(sf::FloatRect(m_MainSprite.getGlobalBounds().left,
		m_Cursor.getRect().top,
		m_MainSprite.getGlobalBounds().width,
		m_Cursor.getSize().y));

	//Updates the cursor location
	m_Cursor.setPosition(getPosition() + sf::Vector2f(m_iCursorLevel * (getSize().x / m_iNumberOfOptions), getSize().y / 2));
}

void Slider::setCursorTexture(sf::Texture* tex2)
{
	m_Cursor.setTexture(tex2);
	m_Cursor.setColor(sf::Color(255,255,255, 255));
}

void Slider::setCursorColour(sf::Color newColour)
{
	m_Cursor.setColor(newColour);
}

void Slider::setCursorSize(sf::Vector2f newSize)
{
	m_Cursor.setSize(newSize);
	m_Cursor.setOrigin(m_Cursor.getSize() / 2.0f);
}

void Slider::setNumberOfOptions(int iNumber)
{
	iNumber--; //Accounts for 0 value

	m_iNumberOfOptions = iNumber;

	//If the cursor level is outside the range change the level
	if (m_iCursorLevel > m_iNumberOfOptions)
	{
		m_iCursorLevel = m_iNumberOfOptions;
	}
	if (m_iCursorLevel < 0)
	{
		m_iCursorLevel = 0;
	}
}

void Slider::setCursorPos(sf::Vector2i mousePos)
{
	//If the mouse is out of the bounds of the bar then set the cursor to
	//Max
	if (mousePos.x > m_MainSprite.getPosition().x + m_MainSprite.getSize().x)
	{
		m_iCursorLevel = m_iNumberOfOptions;
	}
	//Min
	else if (mousePos.x < m_MainSprite.getPosition().x)
	{
		m_iCursorLevel = 0;
	}
	else
	{
		//Interpolate Position of cursor to get the level value
		float fProgress = (mousePos.x - m_MainSprite.getPosition().x) / m_MainSprite.getSize().x;
		m_iCursorLevel = rint(m_iNumberOfOptions * fProgress);
	}
}

void Slider::setLevel(int iNewLevel)
{
	m_iCursorLevel = iNewLevel;
}

sf::Vector2f Slider::getCursorSize()
{
	return m_Cursor.getSize();
}

int Slider::getNumberOfOptions()
{
	return m_iNumberOfOptions;
}

int Slider::getLevel()
{
	return m_iCursorLevel;
}

void Slider::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_MainSprite);
	target.draw(m_Cursor);
}

Slider::~Slider()
{
}
