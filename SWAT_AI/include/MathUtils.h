#pragma once

#include <SFML\Graphics.hpp>

namespace Util
{
	//Calculates the 2D cross product
	static float cross(sf::Vector2f pointA, sf::Vector2f pointB)
	{
		return (pointA.x * pointB.y - pointA.y * pointB.x);
	}

	//Calculates the point at which two lines intersect
	static sf::Vector2f lineIntersect(sf::Vector2f line1A, sf::Vector2f line1B, sf::Vector2f line2A, sf::Vector2f line2B)
	{
		// line intersect equation = a + tr = c + us
		sf::Vector2f line1Vect(line1B - line1A); //Axis 1 
		sf::Vector2f line2Vect(line2B - line2A); //Axis 2 

		float crossed = Util::cross(line1Vect, line2Vect);
		float line1Scalar = Util::cross(line2A - line1A, line2Vect) / crossed; //Finds the scalar value of how far up the first line the points cross
		float line2Scalar = Util::cross(line2A - line1A, line1Vect) / crossed; //Finds the scalar value of how far up the second line the points cross

		//If both scalars are within 0 and 1 then the lines intersect within the range
		if (line1Scalar >= 0 && line1Scalar <= 1 && line2Scalar >= 0 && line2Scalar <= 1)
		{
			return line1A + (line1Scalar * line1Vect); // a + tr
		}
		else
		{
			return line2B; //Out of range value
		}
	}

	
	static float magnitude(sf::Vector2f vect)
	{
		return sqrtf(pow(vect.x, 2.0f) + pow(vect.y, 2.0f));
	}
	static float setWithinRange(float fAngle, float fLowerBound, float fUpperBound)
	{
		while (fAngle < fLowerBound)
		{
			fAngle += (fUpperBound - fLowerBound);
		}
		while (fAngle >= fUpperBound)
		{
			fAngle -= (fUpperBound - fLowerBound);
		}
		return fAngle;
	}
}