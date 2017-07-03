#pragma once
#include "Object.h"
#include "Button.h"

class Slider : public Button
{
	private:
		Button m_Cursor; //Contains sprite data for the indicating cursor
		int m_NumberOfOptions; //Holds the extents of the Slider
		int m_CursorLevel; //Holds the current slider value

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Slider(); //Default constructor

		void update(sf::Vector2i mousePos);

		//bool hovering(sf::Vector2i pos);
		
		//Setters
		void setCursorTexture(sf::Texture* tex2);
		void setCursorColour(sf::Color newColour);
		void setCursorSize(sf::Vector2f newSize);
		void setNumberOfOptions(int iNumber);
		void setCursorPos(sf::Vector2i mousePos);
		void setLevel(int iNewLevel);

		//Getters
		sf::Vector2f getCursorSize();
		int getNumberOfOptions();
		int getLevel();


		~Slider(); //Default deconstructor
};