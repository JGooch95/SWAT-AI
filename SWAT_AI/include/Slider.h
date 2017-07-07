#pragma once
#include "Object.h"
#include "Button.h"

class Slider : public Button
{
	private:
		Button m_Cursor; //!< Contains sprite data for the indicating cursor
		int m_iNumberOfOptions; //!< Holds the extents of the Slider
		int m_iCursorLevel; //!< Holds the current slider value

		/// \brief Draws the slider entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		///\brief Default constructor
		Slider();

		///\brief Updates slider data
		///\param mousePos The mouse position
		void update(sf::Vector2i mousePos);
		
		//Setters
		///\brief Sets the texture for the cursor
		///\param tex2 The new texture
		void setCursorTexture(sf::Texture* tex2);

		///\brief Sets the colour for the cursor
		///\param The new cursor colour
		void setCursorColour(sf::Color newColour);

		///\brief Sets the size for the cursor
		///\param newSize The new cursor dimensions
		void setCursorSize(sf::Vector2f newSize);

		///\brief Sets the number of options within the slider
		///\param iNumber The quantity of options to set
		void setNumberOfOptions(int iNumber);

		///\brief Sets the position of the cursor on the slider
		///\param mousePos The position of the mouse
		void setCursorPos(sf::Vector2i mousePos);

		///\brief Sets the level of the cursor on the slider
		///\param iNewLevel The new cursor value
		void setLevel(int iNewLevel);

		//Getters
		///\brief Returns the dimensions of the cursor
		///\return The dimensions of the cursor
		sf::Vector2f getCursorSize();

		///\brief Returns the number of options
		///\retun The number of options
		int getNumberOfOptions();

		///\brief Returns the level of the cursor
		///\retun The level of the cursor
		int getLevel();

		///\brief Default deconstructor
		~Slider();
};