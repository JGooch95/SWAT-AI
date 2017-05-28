#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <iostream>

enum MapType {ObjectMap, FloorMap};

/// \brief A container of the map screen data
class Map : public sf::Drawable
{
	private:
		static bool m_bInstanceFlag; //!< Holds whether the map has been created
		static Map* m_MapLocation; //!< Holds the memory location of the object

		sf::Vector2f m_Position; //!< Holds the position where the map starts
		sf::Vector2f m_TileSize; //!< Holds the size of each tile on the grid
		sf::Vector2f m_GridDimensions; //!< Holds how many tiles there are in each axis
		sf::Vector2u m_WindowSize; //!< Holds the size of the area containing the map

		sf::VertexArray m_Grid; //!< Holds the lines for the grid
		std::vector<std::vector<char>> m_vcLevelBits; //!< Holds the map layout loaded from the file
		std::vector<std::vector<char>> m_vcFloorBits; //!< Holds the floor layout loaded from the file

	    /// \brief Draws all of the Map's entities to the screen.
	    /// \param target Holds where to draw the entities to.		   
	    ///	\param states 
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		/// \brief Default constructor
		Map();

		std::vector<sf::Vector2f> m_vCorners;
		std::vector<std::pair<int, int>> m_vEdges;

		/// \brief Gets a pointer to the singleton object and creates one if it doesnt exist.
		/// \return returns the pointer to the object
		static Map* getInstance(); 

		/// \brief Performs the main setup of the map
		/// \param area Holds the position and size of the map
		/// \param newGridDims Holds the grid dimenisons
		void setup(sf::FloatRect area, sf::Vector2f newGridDims);

		/// \brief Loads a map file containing the layout
		/// \param newMap Holds the type of map being loaded
		/// \param sDir Holds the directory of the file being loaded
		void load(MapType newMap, std::string sDir);

		/// \brief Sets up the grid for being drawn to the screen
		void setupGrid(); 

		//Setters
		/// \brief Sets the postion of the map
		/// \param pos The new position of the map
		void setPosition(sf::Vector2f pos);

		/// \brief Sets a new layout of objects for the map
		/// \param vcNewBits The new layout of the map
		void setLevelBits(std::vector<std::vector<char>> vcNewBits);

		/// \brief Sets a new layout of the floor for the map
		/// \param vcNewBits The new layout of the map
		void setFloorBits(std::vector<std::vector<char>> vcNewBits);

		/// \brief Sets the dimensions of the grid
		/// \param newGridDims The new grid dimensions
		void setDimensions(sf::Vector2f newGridDims);

		//Getters
		/// \brief Gets the layout of the map
		/// \return returns the layout of the map
		std::vector<std::vector<char>> getMapData();

		/// \brief Gets the layout of the floor map
		/// \return returns the layout of the floor map
		std::vector<std::vector<char>> getFloorData();

		/// \brief Gets the size of a tile
		/// \return returns the size of a tile
		sf::Vector2f getTileSize();

		/// \brief Gets the dimensions of the grid
		/// \return returns the dimensions of the grid
		sf::Vector2f getGridDims();

		/// \brief Gets the size of the map window
		/// \return returns the size of the map window
		sf::Vector2u getWindowSize();

		/// \brief Gets the position of the map
		/// \return returns the position of the map
		sf::Vector2f getPosition();
};