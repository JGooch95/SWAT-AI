#pragma once

#include <SFML/Graphics.hpp>
#include "MathUtils.h"

/// \brief A bar which displays a level of a value between 0 and a max limit graphically
class Bar : public sf::Drawable
{
	private:
		sf::RectangleShape m_BackBox; //!< The backdrop of the bar
		sf::RectangleShape m_FrontBox; //!< The level bar of the bar
		Util::Limits m_LevelLimits; //!< The values for the bar (current Level : Max Limit)
					
		/// \brief Draws all of the bar's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		Bar();

		/// \brief Updates the visuals of the bar to the current data
		void update();

		//Setters
		/// \brief Sets the size of the bar
		/// \param newDimensions The new dimensions of the bar
		void setSize(sf::Vector2f newDimensions);

		/// \brief Sets the postion of the bar
		/// \param newPosition The new position of the bar
		void setPosition(sf::Vector2f newPosition);

		/// \brief Sets the color of the front bar
		/// \param newColor The new color of the bar
		void setLevelColor(sf::Color newColor);

		/// \brief Sets the color of the back bar
		/// \param newColor The new color of the bar
		void setBarColor(sf::Color newColor);

		/// \brief Sets the level of the front bar
		/// \param fLevel The value of the bar
		void setLevel(float fLevel);

		/// \brief Sets the level limit of the front bar
		/// \param fLimit The value of the bar limit
		void setLimit(float fLimit);

		//Getters
		/// \brief Gets the dimensions of the bar
		/// \return returns the dimensions of the bar
		sf::Vector2f getSize();

		/// \brief Gets the position of the bar
		/// \return returns the position of the bar
		sf::Vector2f getPosition();

		/// \brief Gets the level and the limits of the bar
		/// \return returns the limits of the bar
		sf::Vector2f getLevelLimits();
};