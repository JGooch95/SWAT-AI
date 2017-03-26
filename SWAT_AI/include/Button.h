#pragma once
#include "Object.h"

class Button : public Object
{
	private:
		sf::Text m_ButtonText;
		sf::Font m_CurrentFont;
		sf::RectangleShape m_Selector;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Button();
		bool hovering(sf::Vector2i pos); // Returns whether the mouse is on the button
		void setText(std::string sValue);
};
