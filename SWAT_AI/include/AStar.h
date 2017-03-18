#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include "Map.h"

struct Node
{
	Node *parent;
	float f;
	float g;
	float h;
	int index;
};

class AStar
{
	private:
		std::vector<Node> m_Grid;
		std::vector<Node*> m_OpenList;
		std::vector<Node*> m_ClosedList;
		std::deque<Node*> m_Path;
		Map* m_CurrentMap;
		std::vector<int> m_viTilesToCheck;
		bool m_bPathFound;
		bool m_bEndWall;

	public:
		AStar();
		AStar(Map* map1);
		void setup(Map* map1);
		std::deque<Node*> findPath(sf::Vector2f startPos, sf::Vector2f endPos);
		void setupLists();
		void calculateManhattan(int iStartTile, int iEndTile);
		void calculateMoveCost(int iCurrentTile, int iEndTile);
		void checkNode(int iTile, int iCurrentTile, int iMoveCost);
		void makeCurrent(int iTile, int iCurrentTile, int iMoveCost);
		bool validVacinity(int iEndTile);
		void setInvalid(sf::Vector2f Node);
		~AStar();
};