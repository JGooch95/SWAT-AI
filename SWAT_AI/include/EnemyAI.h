#pragma once
#include "AIController.h"

class EnemyAI : public AIController
{
	private:
		float fLastHealth;
		sf::Vector2f lastPos;

	public:
		EnemyAI();
		void update();
		bool hearsSound(WaveEffect* soundArea);
};