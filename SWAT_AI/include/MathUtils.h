#pragma once

#include <SFML\Graphics.hpp>
#include <set>

namespace Util
{
	//Holds an upper and lower bound of an object such as a health bar
	struct Limits
	{
		float upper;
		float lower;
	};

	//Calculates the 2D cross product
	static float cross(sf::Vector2f vectA, sf::Vector2f vectB)
	{
		return (vectA.x * vectB.y - vectA.y * vectB.x);
	}

	//Calculates the point at which two lines intersect
	static sf::Vector2f lineIntersect(sf::Vector2f line1A, sf::Vector2f line1B, sf::Vector2f line2A, sf::Vector2f line2B)
	{
		// line intersect equation = a + tr = c + us
		sf::Vector2f line1Vect(line1B - line1A); //Axis 1 
		sf::Vector2f line2Vect(line2B - line2A); //Axis 2 

		float fCrossed = Util::cross(line1Vect, line2Vect);
		float fLine1Scalar = Util::cross(line2A - line1A, line2Vect) / fCrossed; //Finds the scalar value of how far up the first line the points cross
		float fLine2Scalar = Util::cross(line2A - line1A, line1Vect) / fCrossed; //Finds the scalar value of how far up the second line the points cross

		//If both scalars are within 0 and 1 then the lines intersect within the range
		if (fLine1Scalar >= 0 && fLine1Scalar <= 1 && fLine2Scalar >= 0 && fLine2Scalar <= 1)
		{
			return line1A + (fLine1Scalar * line1Vect); // a + tr
		}
		else
		{
			return line2B; //Out of range value
		}
	}

	//Finds the magnitude of a vector
	static float magnitude(sf::Vector2f vect)
	{
		return sqrtf(pow(vect.x, 2.0f) + pow(vect.y, 2.0f));
	}

	//Keeps an angle between a range of values
	static float setWithinRange(float fAngle, float fLowerBound, float fUpperBound)
	{
		//While lower than the lower bound increase the angle by the range
		while (fAngle < fLowerBound)
		{
			fAngle += (fUpperBound - fLowerBound);
		}
		//While higher than the upper bound decrease the angle by the range
		while (fAngle >= fUpperBound)
		{
			fAngle -= (fUpperBound - fLowerBound);
		}
		return fAngle;
	}

	//Finds the closest point at which a ray intersects with a set of edges
	static std::pair<bool, sf::Vector2f> findLowestIntersect(std::vector<sf::Vector2f>vEdges, std::vector<sf::Vector2f> newRay)
	{
		sf::Vector2f lowestIntersect = newRay.at(1); //Sets the lowest intersect to be the original length of the ray
		bool bIntersected = false; //Holds whether an intersection has been found

		//For every edge
		for (int j = 0; j < vEdges.size(); j += 2)
		{
			//Checks where the ray and the edge intersect
			sf::Vector2f currentIntersect = Util::lineIntersect(vEdges.at(j), vEdges.at(j + 1), newRay.at(0), newRay.at(1));

			//If the ray is shorter than the previous rays then set the ray to be the shortest ray
			if (Util::magnitude(currentIntersect - newRay.at(0)) < Util::magnitude(lowestIntersect - newRay.at(0)))
			{
				lowestIntersect = currentIntersect;
				bIntersected = true;
			}
		}
		return std::pair<bool, sf::Vector2f>{ bIntersected, lowestIntersect };
	}

	//Gets the angle a vector is pointing towards
	static float getAngle(sf::Vector2f vect)
	{
		return atan2f(vect.y, vect.x) * (180.0f / 3.14f); 
	}

	//Rotates a vector by a given angle.
	static sf::Vector2f rotateVect(sf::Vector2f vect, float fAngle)
	{
		return sf::Vector2f(vect.x * cos((fAngle + 90.0f) * (3.14159265359 / 180)),
							vect.y * sin((fAngle + 90.0f) * (3.14159265359 / 180)));
	}
}