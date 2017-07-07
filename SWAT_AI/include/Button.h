#pragma once
#include "Object.h"
#include "../include/FontManager.h"

/// \brief A button which takes in a press and performs an action
class Button : public Object
{
	private:
		FontManager* m_FontManager; //!< Holds the font used
		sf::Text m_ButtonText; //!< Holds the text on the button
		sf::RectangleShape m_Backdrop; //!< Holds the background
		sf::FloatRect m_CollisionBox; //!< The box used for collision checking
		sf::Color m_DefaultColor; //!< Holds the base color of the background

		/// \brief Draws all of the button's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		Button();

		/// \brief Performs all base update operations 
		/// \param mousePos Holds the mouse position
		virtual void update(sf::Vector2i mousePos); 

		/// \brief Gets whether the mouse is on the button
		/// \param pos The position of the mouse
		/// \return Returns whether the mouse is on the button
		bool hovering(sf::Vector2i pos);

		//Setters
		/// \brief Sets the text within the button
		/// \param sValue The string containing the text
		void setText(std::string sValue);

		/// \brief Sets the background color of the button
		/// \param newColor The new color
		void setBackgroundColor(sf::Color newColor);

		/// \brief Sets the collision box for the button
		/// \param newArea Holds the dimensions and position for the collision box
		void setCollisionBox(sf::FloatRect newArea);
};
