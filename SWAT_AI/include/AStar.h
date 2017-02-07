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
		Map* m_CurrentMap;
		std::vector<int>m_TilesToCheck;
		bool m_bPathFound;
		bool m_bEndWall;

	public:
		AStar();
		AStar(Map* Map1);
		void Setup(Map* Map1);
		std::deque<Node*> FindPath(sf::Vector2f StartPos, sf::Vector2f EndPos);
		void SetupLists();
		void CalculateManhattan(int iStartTile, int iEndTile);
		void CalculateMoveCost(int iCurrentTile, int iEndTile);
		void CheckNode(int iTile, int iCurrentTile, int iMoveCost);
		bool ValidVacinity(int iEndTile);
		std::deque<Node*> m_Path;
};