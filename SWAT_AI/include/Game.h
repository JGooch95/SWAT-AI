#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Character.h"
#include "TextureLoader.h"
#include "Object.h"
#include "Map.h"
#include "AStar.h"

class Game : public sf::Drawable
{
	private:
		std::vector<Character*> m_Units;
		std::vector<Character*> m_Enemies;
		std::vector<Object*> m_Walls;
		TextureLoader m_Textures;
		AStar m_Pathfinder;
		Map m_CurrentMap;
		std::vector<sf::Vector2f> Edges;
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Game(sf::Vector2u WindowSize);
		void Update(sf::Vector2i MousePos);
		void ClickRight(sf::Vector2i MousePos);
		void ClickLeft(sf::Vector2i MousePos);
};