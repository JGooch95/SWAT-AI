#pragma once
#include "AIController.h"

class EnemyAI : public AIController
{
	private:
		float m_fLastHealth; //!< Health in the last check
		sf::Vector2f m_LastPos; //!< The last position an enemy was seen

	public:
		/// \brief Default constructor
		EnemyAI();

		/// \brief Updates the states of data
		void update();

		///\brief checks if the AI character hears the sound and acts accordingly
		///\param soundArea The sound being checked
		bool hearsSound(WaveEffect* soundArea);
};