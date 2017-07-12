#include "..\include\Collider.h"

bool Collider::AABBBoxCollision(Object * Object1, Object * Object2)
{
	if (Object1->getRect().left < Object2->getRect().left + Object2->getRect().width &&
		Object1->getRect().top  < Object2->getRect().top + Object2->getRect().height &&
		Object1->getRect().left + Object1->getRect().width > Object2->getRect().left &&
		Object1->getRect().top + Object1->getRect().height > Object2->getRect().top)
	{
		float left = (Object1->getRect().left + Object1->getRect().width) - Object2->getRect().left;
		float right = (Object2->getRect().left + Object2->getRect().width) - Object1->getRect().left;
		float bottom = (Object2->getRect().top + Object2->getRect().height) - Object1->getRect().top;
		float top = (Object1->getRect().top + Object1->getRect().height) - Object2->getRect().top;

		float xIntersect = abs(std::min(right, left));
		float yIntersect = abs(std::min(top , bottom));

		if (xIntersect < yIntersect)
		{
			if (left < right)
			{
				Object1->setPosition(sf::Vector2f(Object1->getPosition().x - xIntersect, Object1->getPosition().y));
			}
			else
			{
				Object1->setPosition(sf::Vector2f(Object1->getPosition().x + xIntersect, Object1->getPosition().y));
			}
		}
		else
		{
			if (top < bottom)
			{
				Object1->setPosition(sf::Vector2f(Object1->getPosition().x, Object1->getPosition().y - yIntersect));
			}
			else
			{
				Object1->setPosition(sf::Vector2f(Object1->getPosition().x, Object1->getPosition().y + yIntersect));
			}
		}
		return true;
	}
	return false;
}
