#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Bar.h"
#include "MathUtils.h"
#include "Map.h"
#include "TextureLoader.h"
#include "FontManager.h"

enum BarType {HealthBar, AmmoBar};

/// \brief A container of the Units UI
class HUDWindow : public sf::Drawable
{
	private:
		Map* m_CurrentMap; //!< Holds a pointer to the map
		TextureLoader* m_Textures;  //!< Holds the location of all of the textures
		FontManager* m_Fonts; //!< Holds the location of all of the fonts

		sf::RectangleShape m_BackDrop; //!< A box to stand as a background for the button
		std::vector<Bar*> m_UIBars; //!< The health and ammo bars
		std::vector<Object> m_Icons; //!< Holds the icons to go beside the bars
		std::vector<sf::Text*> m_UIText; //!< Holds the text to be put on the bars
		Button m_ClassButton; //!< Button used for switching classes
		std::vector<Button*> m_LoadoutButton; //!< Button holding the current loadout slot of the unit

		/// \brief Draws all of the HUDWindow's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		HUDWindow();

		/// \brief Scales the UI to be relative to the outside container
		void scaleUI(); //Scales the UI to the correct proportions

		//Setters
		/// \brief Sets the size of the HUD window
		/// \param size The new dimensions of the HUD window
		void setSize(sf::Vector2f size);

		/// \brief Sets the postion of the HUD window
		/// \param pos The new position of the HUD window
		void setPosition(sf::Vector2f pos);

		/// \brief Sets the texture of the class button
		/// \param newTex Holds a pointer to the new texture
		void setClassTexture(sf::Texture* newTex);

		/// \brief Sets the levels of the bars
		/// \param currentBarType States the type of bar being edited
		/// \param barSettings Holds the new settings
		void setBarLevels(BarType currentBarType, Util::Limits barSettings);

		/// \brief Sets the Bar Icon
		/// \param currentBarType States the type of bar being edited
		/// \param newTex  Holds a pointer to the new texture
		void setBarIcon(BarType currentBarType, sf::Texture* newTex);

		/// \brief Sets the Bar Text
		/// \param currentBarType States the type of bar being edited
		/// \param barText Holds the new text to be used
		void setBarText(BarType currentBarType, std::string barText);

		/// \brief Sets the loadout button texture
		/// \param iIndex Holds the index of the button
		/// \param newTex  Holds a pointer to the new texture
		void setLoadoutTexture(int iIndex, sf::Texture* newTex);

		//Getters
		/// \brief Gets the dimensions of the HuD window
		/// \return returns the dimensions of the window
		sf::Vector2f getSize();

		/// \brief Gets a pointer to the class button
		/// \return returns a pointer to the class button
		Button* getClassButton();

		/// \brief Gets the loadout button at the index given
		/// \param iIndex Holds the index of the loadout button being retrieved
		/// \return returns a pointer to the button
		Button* getLoadoutButton(int iIndex);

		/// \brief Default deconstructor
		~HUDWindow();
};