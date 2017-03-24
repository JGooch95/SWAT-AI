#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Bar.h"
#include "MathUtils.h"
#include "Map.h"

enum BarType {HealthBar, AmmoBar};
class HUDWindow : public sf::Drawable
{
	private:
		sf::RectangleShape m_BackDrop;
		std::vector<Bar*> m_UIBars;
		std::vector<sf::Text*> m_UIText;
		Button m_ClassButton;
		std::vector<Button> m_LoadoutButton;
		std::vector<Object> m_Icons;
		Map* m_CurrentMap;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		HUDWindow();

		//Setters
		void setSize(sf::Vector2f size);
		void setPosition(sf::Vector2f pos);
		void setClassTexture(sf::Texture* newTex);
		void setBarLevels(BarType currentBarType, Util::Limits barSettings);
		void setBarIcon(BarType currentBarType, sf::Texture* newTex);
		void setBarText(BarType currentBarType, std::string barText);
		void setFont(sf::Font* newFont);
		void scaleUI();
		void setLoadoutTexture(int iIndex, sf::Texture* newTex);

		//Getters
		sf::Vector2f getSize();
		Button* getClassButton();
		Button* getLoadoutButton(int iIndex);

		~HUDWindow();
};