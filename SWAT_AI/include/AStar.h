#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include "Map.h"

/// \brief A node within the pathfinding which acts as a place within the pathfinding grid
struct Node
{
	Node* parent; //!< Holds the parent node
	float f; //!< Holds the calculated overall cost of the node
	float g; //!< Holds the cumulative move cost to the node
	float h; //!< Holds the heuristic value of the node
	int index; //!< Holds the nodes index within the grid
};


/// \brief A pathfinder which will calculate a path between 2 given locations within a grid.
class AStar
{
	private:
		std::vector<Node> m_vGrid; //!< Holds all of the nodes to move between within the map
		std::vector<Node*> m_vOpenList; //!< Holds all of that are being searched
		std::vector<Node*> m_vClosedList; //!< Holds all of the nodes which cannot be accessed
		std::deque<Node*> m_Path; //!< Holds the path being calculated
		Map* m_CurrentMap; //!< A pointer pointing to the map data
		std::vector<int> m_viTilesToCheck;  //!< Holds the index offsets to get the adjacent tiles
		bool m_bPathFound; //!< States whether a path has been calculated
		bool m_bEndWall; //!< States if the end of the path is a wall tile

	public:
		/// \brief Default constructor
		AStar();

		/// \brief Sets up the grid, index offsets and the open and closed lists
		void setup();

		/// \brief Sets up the open and closed lists
		void setupLists();

		/// \brief Finds a path between two given locations
		/// \param startPos The starting position of the path
		/// \param endPos The end position of the path
		/// \return Returns the path found
		std::deque<Node*> findPath(sf::Vector2f startPos, sf::Vector2f endPos);

		/// \brief Calculates the manhattan distance of the node
		/// \param iStartTile The starting tile index of the distance calculation
		/// \param iEndTile The end tile index of the path
		void calculateManhattan(int iStartTile, int iEndTile);

		/// \brief Calculates the movecost for the surrounding nodes of the current node
		/// \param iCurrentTile The tile index of the current tile
		/// \param iEndTile The end tile index of the path
		void calculateMoveCost(int iCurrentTile, int iEndTile);

		/// \brief Checks if the node has been calculated and applies the movecost
		/// \param iTile The tile index of the tile being set
		/// \param iCurrentTile The tile index of the current main tile
		/// \param iMoveCost The movecost to move to the tile
		void checkNode(int iTile, int iCurrentTile, int iMoveCost);

		/// \brief Sets the tiles g, f and parent values.
		/// \param iTile The tile index of the tile being set
		/// \param iCurrentTile The tile index of the current main tile
		/// \param iMoveCost The movecost to move to the tile
		void makeCurrent(int iTile, int iCurrentTile, int iMoveCost);

		/// \brief Checks if the tile given has an open space around it
		/// \param iEndTile The tile being checked (Typically performed on the end tile)
		/// \return Teturns whether there is an open space near the tile
		bool validVacinity(int iEndTile);

		/// \brief Default deconstructor
		~AStar();
};

/// \class AStar
/// This class is the AStar pathfinding class. The purpose of this class is to calculate a path 
/// between two points in a given map containing inaccessible locations.
/// \see Node
