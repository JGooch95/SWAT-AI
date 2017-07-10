#pragma once
#include <SFML/Graphics.hpp>

/// \brief A soundwave which emits from its center point location
class soundWave :public sf::Drawable
{
	private:
		sf::CircleShape m_Wave; //!< Holds the wave being emitted
		float m_fDesiredRadius; //!< Holds the radius where the wave ends
		float m_fIncreaseRate; //!< Holds the rate at which the radius increases
		float m_fDecreaseRate;  //!< Holds the rate at which the radius decreases

		/// \brief Draws all of the soundWave's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		/// \param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		soundWave();

		/// \brief Constructor
		/// \param fRadius The desired radius of the sound wave
		/// \param fIncreaseRate The rate at which the sound wave reaches its peak
		/// \param fDecreaseRate  The rate at which the sound wave moves from its peak
		/// \param position The centerpoint of the wave
		soundWave(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position);
		
		/// \brief Updates the waves data
		void update(); 

		/// \brief States whether the wave has reached the desired radius 
		/// \return returns whether the wave has reached the desired radius 
		bool isDone(); 

		/// \brief Gets the position of the wave center
		/// \return returns the position of the wave center
		sf::Vector2f getPosition();

		/// \brief Gets the current radius of the soundwave
		/// \return Returns the current radius of the soundwave
		float getRadius();
};