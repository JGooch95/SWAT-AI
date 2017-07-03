#pragma once

#include <SFML\Graphics.hpp>
#include "Map.h"
#include "Settings.h"
#include "TextureLoader.h"

/// \brief A generic game object
class Object : public sf::Drawable
{
	protected:
		Map* m_CurrentMap; //!< Holds the data for the map
		Settings* m_CurrentSettings; //!< Holds the current settings
		TextureLoader* m_Textures;
		sf::RectangleShape m_MainSprite; //!< Holds the sprite
		const sf::Texture* m_CurrentTexture; //!< Holds a pointer to the texture
		/// \brief Draws all of the object's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		/// \param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		Object();

		//Setters
		/// \brief Sets the texture of the object
		/// \param tex2 Holds a point to the new texture
		void setTexture(sf::Texture* tex2);

		/// \brief Sets the position of the object
		/// \param pos Contains the new position
		virtual void setPosition(sf::Vector2f pos);

		/// \brief Sets the origin of the object
		/// \param pos Contains the new origin
		virtual void setOrigin(sf::Vector2f pos);

		/// \brief Sets the size of the object
		/// \param dims Contains the new size
		virtual void setSize(sf::Vector2f dims);

		/// \brief Sets an area of repeatable textures on the object
		/// \param dims Contains the new size
		void setArea(sf::Vector2f dims);

		/// \brief Sets the rotation of the object
		/// \param dims The new angle
		void setRotation(float fRot);

		/// \brief Sets the color of the rectangle
		/// \param newColor The new color
		void setColor(sf::Color newColor);

		//Getters
		/// \brief Gets the position of the object
		/// \return returns the position of the object
		sf::Vector2f getPosition();

		/// \brief Gets the origin of the object
		/// \return returns the origin of the object
		sf::Vector2f getOrigin();

		/// \brief Gets the dimensions of the object
		/// \return returns the dimensions of the object
		sf::Vector2f getSize();

		/// \brief Gets the bounding box of the object
		/// \return returns the bounding box of the object
		sf::FloatRect getRect();

		/// \brief Gets the scale of the object
		/// \return returns the scale of the object
		sf::Vector2f getScale();

		/// \brief Gets the edges around the object
		/// \return returns all of the edges around the object
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getEdges();
};