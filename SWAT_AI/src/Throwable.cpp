#include "../include/Throwable.h"

Throwable::Throwable(throwableType newType, sf::Vector2i startPos)
{
	m_Textures = TextureLoader::getInstance();

	m_StartLocation = startPos;
	m_CurrentType = newType;

	float fSize = std::min(m_CurrentMap->getTileSize().x, m_CurrentMap->getTileSize().y) / 3.0f;
	//Default throwable values
	setSize(sf::Vector2f(fSize, fSize));
	setColor(sf::Color(255, 255, 255, 255));
	setPosition(sf::Vector2f(startPos));
	setOrigin(getSize() / 2.0f);

	//Sets up the throwable type
	switch (m_CurrentType)
	{
		case Grenade:
			setTexture(m_Textures->getTexture(35));
			m_fFuseTime = 3.0f;
			break;

		case Flashbang:
			setTexture(m_Textures->getTexture(36));
			m_fFuseTime = 2.0f;
			break;

		case Rock:
			setTexture(m_Textures->getTexture(37));
			m_fFuseTime = 10.0f;
			break;
	}

	//Sets up the throwing line
	m_ThrowLine.resize(2);
	m_ThrowLine.setPrimitiveType(sf::Lines);
	for (int i = 0; i < m_ThrowLine.getVertexCount(); i++)
	{
		m_ThrowLine[i].color = sf::Color(255, 255, 0, 255);
	}
	m_ThrowLine[0].position = sf::Vector2f(m_StartLocation);
	m_DropArea.setFillColor(sf::Color(255, 255, 255, 50));

	m_CurrentState = Aiming;
}

void Throwable::update(sf::Vector2f startPos, sf::Vector2i pos)
{
	switch (m_CurrentState)
	{
		case Aiming:
			aimAt(pos); //Aim at the position given

			//If the user has stopped holding
			if (m_CurrentType == Grenade && sf::Keyboard::isKeyPressed(sf::Keyboard::G) ||
				m_CurrentType == Flashbang && sf::Keyboard::isKeyPressed(sf::Keyboard::F) ||
				m_CurrentType == Rock && sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			{
				m_ThrowLine[0].position = startPos;
				setPosition(sf::Vector2f(startPos));
			}
			else
			{
				//Throw object and start fuse
				m_FuseTimer.restart();
				m_CurrentState = Thrown;
				throwAt(pos);
			}
			break;
		case Thrown:
			//Move towards location
			m_MainSprite.setPosition( m_MainSprite.getPosition() + (m_MoveDir* 2.0f));
			m_MainSprite.setRotation(m_MainSprite.getRotation()+15);

			//If location reached set state to landed
			if (m_MainSprite.getPosition().x >= m_TargetLocation.x - 1 &&
				m_MainSprite.getPosition().x <= m_TargetLocation.x + 1 &&
				m_MainSprite.getPosition().y <= m_TargetLocation.y + 1 &&
				m_MainSprite.getPosition().y >= m_TargetLocation.y - 1)
			{
				m_CurrentState = Landed;
			}
			break;
		case Landed:
			break;
	}
}

void Throwable::aimAt(sf::Vector2i pos)
{
	//Updates aiming UI and positions
	m_ThrowLine[0].position = sf::Vector2f(m_StartLocation);
	m_ThrowLine[1].position = sf::Vector2f(pos);
	m_DropArea.setRadius(Util::magnitude((m_ThrowLine[1].position - m_ThrowLine[0].position)/2.0f)); //Makes the radius bigger / smaller dependant on distance
	m_DropArea.setOrigin(sf::Vector2f(m_DropArea.getRadius(), m_DropArea.getRadius()));
	m_DropArea.setPosition(sf::Vector2f(pos));
}

void Throwable::throwAt(sf::Vector2i pos)
{
	sf::Vector2f randomArea; //Holds the randomised location

	//Min and max values for throwing
	sf::Vector2f start = sf::Vector2f(m_DropArea.getPosition().x - m_DropArea.getRadius(), m_DropArea.getPosition().y - m_DropArea.getRadius()); 
	sf::Vector2f end= sf::Vector2f(m_DropArea.getPosition().x + m_DropArea.getRadius(), m_DropArea.getPosition().y + m_DropArea.getRadius());

	bool inCircle = false;
	while (!inCircle)
	{
		//Randomises a location
		randomArea.x = (rand() % (int)end.x) + start.x;
		randomArea.y = (rand() % (int)end.y) + start.y;

		//If within the accuracy circle end the loop
		if (Util::magnitude(randomArea - m_DropArea.getPosition()) < m_DropArea.getRadius())
		{
			inCircle = true;
		}
	}
	//Set the target location and set up the movement direction for throwing
	m_TargetLocation = sf::Vector2i(randomArea);
	m_MoveDir = sf::Vector2f(m_TargetLocation - m_StartLocation) / Util::magnitude(sf::Vector2f(m_TargetLocation - m_StartLocation));
}

bool Throwable::isDone()
{
	if (m_FuseTimer.getElapsedTime().asSeconds() > m_fFuseTime && m_CurrentState != Aiming ||
		m_CurrentState == Landed && m_CurrentType == Rock)
	{
		return true;
	}
	return false;
}

throwableType Throwable::getType()
{
	return m_CurrentType;
}

void Throwable::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_CurrentState == Aiming)
	{
		target.draw(m_ThrowLine);
		target.draw(m_DropArea);
	}
	target.draw(m_MainSprite);
}
