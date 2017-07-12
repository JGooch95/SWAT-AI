#pragma once
#include "Object.h"
#include "MathUtils.h"
#include "TextureLoader.h"

enum throwableType { Grenade, Flashbang, Rock};

class Throwable : public Object
{
	private:
		TextureLoader* m_Textures;

		throwableType m_CurrentType;

		sf::Vector2i m_StartLocation;
		sf::Vector2i m_TargetLocation;
		sf::Vector2f m_MoveDir;

		//UI
		sf::VertexArray m_ThrowLine;
		sf::CircleShape m_DropArea;

		sf::Clock m_FuseTimer; //!< Holds how long its been since firing the weapon
		float m_fFuseTime;

		enum throwState{Thrown, Aiming, Landed};
		throwState m_CurrentState;

		/// \brief Draws all of the object's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		/// \param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Throwable(throwableType newType, sf::Vector2i startPos);
		void update(sf::Vector2f startPos, sf::Vector2i pos);
		void aimAt(sf::Vector2i pos);
		void throwAt(sf::Vector2i pos);
		void updateUI();
		bool isDone();
		throwableType getType();
		


};