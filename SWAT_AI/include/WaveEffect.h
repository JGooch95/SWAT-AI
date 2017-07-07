#pragma once
#include <SFML/Graphics.hpp>

enum waveType{Sound, Flash, Explosion};

/// \brief A soundwave which emits from its center point location
class WaveEffect :public sf::Drawable
{
	private:
		sf::CircleShape m_Wave; //!< Holds the wave being emitted
		float m_fDesiredRadius; //!< Holds the radius where the wave ends
		float m_fIncreaseRate;  //!< Holds the rate at which the wave emerges
		float m_fDecreaseRate;  //!< Holds the rate at which the wave fades
		waveType currentType;

		/// \brief Draws all of the soundWave's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		/// \param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		WaveEffect();

		/// \brief Constructor
		/// \param fRadius The desired radius of the sound wave
		/// \param fIncreaseRate The rate at which the sound wave reaches its peak
		/// \param fDecreaseRate  The rate at which the sound wave moves from its peak
		/// \param position The centerpoint of the wave
		WaveEffect(float fRadius, float fIncreaseRate, float fDecreaseRate, sf::Vector2f position, waveType newType);
		
		/// \brief Updates the waves data
		void update(); 

		//Getters
		/// \brief States whether the wave has reached the desired radius 
		/// \return Returns whether the wave has reached the desired radius 
		bool isDone(); 

		/// \brief Gets the position of the wave center
		/// \return Returns the position of the wave center
		sf::Vector2f getPosition();

		/// \brief Gets the current radius of the soundwave
		/// \return Returns the current radius of the soundwave
		float getRadius();

		waveType getType();
};