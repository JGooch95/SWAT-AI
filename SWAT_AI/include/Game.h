#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Character.h"
#include "TextureLoader.h"
#include "Object.h"
#include "Entrance.h"
#include "Map.h"
#include "AStar.h"
#include "Button.h"
#include "Settings.h"
#include "HUDWindow.h"
#include "WaveEffect.h"
#include "Throwable.h"
#include "ContentScreen.h"

/// \brief The main game screen
class Game : public ContentScreen
{
	private:
		//UI
		sf::RectangleShape m_Toolbar; //!< Holds the toolbar across the top of the screen
		std::vector<HUDWindow*> m_vUnitUI; //!< Holds the units UI boxes

		//Objects
		std::vector<Character*> m_vCharacters; //!< Holds all of the characters
		std::vector<Character*> m_vUnits; //!< Holds the friendly units
		std::vector<Character*> m_vEnemies; //!< Holds the enemy units
		std::vector<Object*> m_vWalls; //!< Holds the walls
		std::vector<Entrance*> m_vDoors; //!< Holds the doors
		std::vector<Object*> m_Background;

		std::vector<Throwable*> m_vThrowables; //Any thrown objects
		std::vector<WaveEffect*> m_vWaves;


		Button* exitButton; //!< Holds the button which exits to the menu

		//DEBUGGING LINES
		sf::VertexArray m_EdgeLines; //!< Holds the debug lines for the edges of walls

		/// \brief Draws all of the game's entities to the screen.
		/// \param target Holds where to draw the entities to.		   
		///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		/// \param windowSize The size of the current window in context
		Game(sf::Vector2u windowSize);

		/// \brief Updates all objects in game
		/// \param mousePos The position of the mouse
		void update(sf::Vector2i mousePos); 

		/// \brief Processes interactions between 2 sets of characters
		/// \param charSet1 The first set of characters
		/// \param charSet1 The second set of characters
		void characterInteractions(std::vector<Character*> charSet1, std::vector<Character*>  charSet2);

		int processInput(sf::Event keyCode, sf::Vector2i mousePos);

		/// \brief Reduces the amounts of edges for each wall by combining adjacent edges and removing unreachable edges.
		/// \param vXEdges The edges in the going along the X axis
		/// \param vYEdges The edges in the going along the Y axis
		/// \return Returns the reduced amount of edges
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edgeReduction(std::vector<sf::Vector2f> vXEdges, std::vector<sf::Vector2f> vYEdges);

		/// \brief Loads patrol paths from the directory given
		/// \param sDir The directory containing the path
		void loadPatrolPaths(std::string sDir);

		/// \brief Adds an edge to a corner and edge list
		/// \param points The edge to give
		/// \param CornerList The list of corners to be added to
		/// \param EdgeList The list of edges to be added to
		void addEdge(sf::Vector2f points[2], std::vector<sf::Vector2f*>* vCornerList, std::vector<std::pair<sf::Vector2f*, sf::Vector2f*>>* vEdgeList);

		/// \brief Default deconstructor
		~Game();
};