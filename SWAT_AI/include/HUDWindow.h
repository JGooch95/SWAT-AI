#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Bar.h"
#include "MathUtils.h"
#include "Map.h"
#include "TextureLoader.h"
#include "FontManager.h"

enum BarType {HealthBar, AmmoBar};
class HUDWindow : public sf::Drawable
{
	private:
		Map* m_CurrentMap; //Holds a pointer to the map
		TextureLoader* m_Textures;
		FontManager* m_Fonts;

		sf::RectangleShape m_BackDrop; //A box to stand as a background for the button
		std::vector<Bar*> m_UIBars; //The health and ammo bars
		std::vector<Object> m_Icons; //Holds the icons to go beside the bars
		std::vector<sf::Text*> m_UIText; //Holds the text to be put on the bars
		Button m_ClassButton; //Button used for switching classes
		std::vector<Button*> m_LoadoutButton; //Button holding the current loadout slot of the unit
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		HUDWindow();

		void scaleUI(); //Scales the UI to the correct proportions

		//Setters
		void setSize(sf::Vector2f size);
		void setPosition(sf::Vector2f pos);
		void setClassTexture(sf::Texture* newTex);
		void setBarLevels(BarType currentBarType, Util::Limits barSettings);
		void setBarIcon(BarType currentBarType, sf::Texture* newTex);
		void setBarText(BarType currentBarType, std::string barText);
		void setLoadoutTexture(int iIndex, sf::Texture* newTex);

		//Getters
		sf::Vector2f getSize();
		Button* getClassButton();
		Button* getLoadoutButton(int iIndex);

		~HUDWindow();
};