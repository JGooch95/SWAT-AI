#include "../include/AStar.h"

AStar::AStar()
{
	m_CurrentMap = Map::getInstance();
	setup();
}

void AStar::setup()
{
	m_vGrid.resize(m_CurrentMap->getGridDims().x * m_CurrentMap->getGridDims().y);
	setupLists();
	m_viTilesToCheck = {
		1,										 //Right
		-1,										 //Left
		-(int)m_CurrentMap->getGridDims().x,	 //Top
		-(int)m_CurrentMap->getGridDims().x + 1, //Top right
		-(int)m_CurrentMap->getGridDims().x - 1, //Top left
		(int)m_CurrentMap->getGridDims().x,      //Bottom
		(int)m_CurrentMap->getGridDims().x + 1,  //Bottom right
		(int)m_CurrentMap->getGridDims().x - 1}; //Bottom left
}

void AStar::setupLists()
{
	//Clears the open and closed lists
	m_vClosedList.clear();
	m_vOpenList.clear();
	m_Path.clear();

	//Resets Grid data
	for (int i = 0; i < m_vGrid.size(); ++i)
	{
		m_vGrid.at(i).parent = NULL;
		m_vGrid.at(i).h = 0;
		m_vGrid.at(i).f = 0;
		m_vGrid.at(i).g = 0;
	}

	//Adds all invalid map nodes to the closed list
	for (int i = 0; i < m_CurrentMap->getMapData().size(); ++i)
	{
		for (int j = 0; j < m_CurrentMap->getMapData().at(i).size(); ++j)
		{
			if (m_CurrentMap->getMapData().at(i).at(j) == 'W' || m_CurrentMap->getMapData().at(i).at(j) == 'B')
			{
				m_vClosedList.push_back(&m_vGrid.at((i*m_CurrentMap->getGridDims().x) + j));
			}
		}
	}
}

std::deque<Node*> AStar::findPath(sf::Vector2f startPos, sf::Vector2f endPos)
{
	//Calculates the Position of the Start Tile in the Grid
	int iXTile = ((int)startPos.x - m_CurrentMap->getPosition().x) / (int)m_CurrentMap->getTileSize().x;
	int iYTile = ((int)startPos.y - m_CurrentMap->getPosition().y) / (int)m_CurrentMap->getTileSize().y;
	int iStartTile = iXTile + ((int)m_CurrentMap->getGridDims().x * iYTile);

	//Calculates the Position of the End Tile in the Grid
	iXTile = ((int)endPos.x - m_CurrentMap->getPosition().x) / (int)m_CurrentMap->getTileSize().x;
	iYTile = ((int)endPos.y - m_CurrentMap->getPosition().y) / (int)m_CurrentMap->getTileSize().y;
	int iEndTile = iXTile +  ((int)m_CurrentMap->getGridDims().x * iYTile);

	m_bEndWall = false;
	m_bPathFound = false;

	int iCurrentTile = iStartTile; //Holds the tile being checked

	//If the path isnt the same location, off the grid or within an invlid area
	if (iStartTile != iEndTile && iStartTile < m_vGrid.size() && iEndTile < m_vGrid.size() && validVacinity(iEndTile))
	{
		//Checks that the end tile isn't on the closed list
		if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iEndTile)) != m_vClosedList.end())
		{
			m_bEndWall = true;
		}

		//Calculates the Manhattan distance for every node to the end node.
		for (int i = 0; i < m_vGrid.size(); ++i)
		{
			calculateManhattan(i, iEndTile);
			m_vGrid.at(i).index = i;
		}

		int iNextIndex = NULL;
		while (!m_bPathFound)
		{
			//If the tile being checked isn't the end tile
			if (iCurrentTile != iEndTile)
			{
				//Pushes the node to the closed list
				m_vClosedList.push_back(&m_vGrid.at(iCurrentTile));
			}

			//Calculates the move cost for the surrounding nodes
			calculateMoveCost(iCurrentTile, iEndTile);

			if (!m_bPathFound)
			{
				int iLowestF = 1000;
				//Checks for the lowest F value in the open list
				for (int i = 0; i < m_vOpenList.size(); ++i)
				{
					if (m_vOpenList.at(i)->f < iLowestF)
					{
						iCurrentTile = m_vOpenList.at(i)->index;
						iLowestF = m_vOpenList.at(i)->f;
						iNextIndex = i;
					}
				}
				m_vOpenList.erase(m_vOpenList.begin() + iNextIndex);
			}
		}

		Node* CurrentNode;
		//If the end is not on the closed list
		if (!m_bEndWall)
		{
			CurrentNode = &m_vGrid.at(iEndTile);
			m_Path.push_front(CurrentNode);
		}
		//Else set it's parent to be the end node
		else
		{
			CurrentNode = m_vGrid.at(iEndTile).parent;
			m_Path.push_front(CurrentNode);
		}

		//If the current node isn't the start node
		if (CurrentNode->index != iStartTile)
		{
			//Loop trace back through the parents to find the path
			do{
				CurrentNode = CurrentNode->parent;
				m_Path.push_front(CurrentNode);
			} while (CurrentNode->parent != NULL);
		}
	}

	return m_Path;
}

void AStar::calculateManhattan(int iStartTile, int iEndTile)
{	
	//Calculates the difference between the nodes in the y axis
	int iStartPos = (iStartTile / (int)m_CurrentMap->getGridDims().y);
	int iEndPos = (iEndTile / (int)m_CurrentMap->getGridDims().y);
	int iYDiff = abs(iEndPos - iStartPos);

	//Calculates the difference between the nodes in the x axis
	iStartPos = (iStartTile % (int)m_CurrentMap->getGridDims().x);
	iEndPos = (iEndTile % (int)m_CurrentMap->getGridDims().x);
	int iXDiff = abs(iEndPos - iStartPos);

	m_vGrid.at(iStartTile).h = iXDiff + iYDiff;
}

void AStar::calculateMoveCost(int iCurrentTile, int iEndTile)
{
	if (!m_bPathFound)
	{
		//If not on the right edge of the screen
		if (iCurrentTile % (int)m_CurrentMap->getGridDims().x < (int)m_CurrentMap->getGridDims().x - 1)
		{
			checkNode(iCurrentTile + 1, iCurrentTile, 10); //Assign right node a cost

			//If also not on the top edge of the screen
			if (iCurrentTile - m_CurrentMap->getGridDims().x > 0)
			{
				//Assign top right node a cost
				//If the tile to the right, the top or the top right of the tile is on the closed list
				if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x + 1)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile + 1)) != m_vClosedList.end()) {
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x + 1, iCurrentTile, 100);  //Make the cost so high its more probable to walk around it
				}
				else
				{
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x + 1, iCurrentTile, 14); //Otherwise allow diagonal movement
				}
			}

			//If also not on the bottom edge of the screen
			if (iCurrentTile + m_CurrentMap->getGridDims().x < m_vGrid.size())
			{
				//Assign bottom right node a cost
				//If the tile to the right, the bottom or the bottom right of the tile is on the closed list
				if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x + 1)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile + 1)) != m_vClosedList.end()) {
					checkNode(iCurrentTile + m_CurrentMap->getGridDims().x + 1, iCurrentTile, 100); //Make the cost so high its more probable to walk around it
				}
				else
				{
					checkNode(iCurrentTile + m_CurrentMap->getGridDims().x + 1, iCurrentTile, 14);//Otherwise allow diagonal movement
				}
			}
		}
		//If not on the left edge of the screen
		if (iCurrentTile % (int)m_CurrentMap->getGridDims().x > 0)
		{
			checkNode(iCurrentTile - 1, iCurrentTile, 10); //Assign left node a cost

			//If also not on the top edge of the screen
			if (iCurrentTile - m_CurrentMap->getGridDims().x > 0)
			{
				//Assign top left node a cost
				//If the tile to the left, the top or the top left of the tile is on the closed list
				if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x - 1)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile - 1)) != m_vClosedList.end()) {
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x - 1, iCurrentTile, 100);  //Make the cost so high its more probable to walk around it
				}
				else
				{
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x - 1, iCurrentTile, 14);//Otherwise allow diagonal movement
				}
			}

			//If also not on the bottom edge of the screen
			if (iCurrentTile + m_CurrentMap->getGridDims().x < m_vGrid.size())
			{
				//Assign bottom left node a cost
				//If the tile to the left, the bottom or the bottom left of the tile is on the closed list
				if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x - 1)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x)) != m_vClosedList.end() ||
					std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iCurrentTile - 1)) != m_vClosedList.end()) {
					checkNode(iCurrentTile + m_CurrentMap->getGridDims().x - 1, iCurrentTile, 100); //Make the cost so high its more probable to walk around it
				}
				else
				{
					checkNode(iCurrentTile + m_CurrentMap->getGridDims().x - 1, iCurrentTile, 14);//Otherwise allow diagonal movement
				}
			}
		}

		//If not on the top edge of the screen
		if (iCurrentTile - m_CurrentMap->getGridDims().x > 0)
		{
			checkNode(iCurrentTile - m_CurrentMap->getGridDims().x, iCurrentTile, 10); //Assign top node a cost
		}

		//If not on the bottom edge of the screen.
		if (iCurrentTile + m_CurrentMap->getGridDims().x < m_vGrid.size())
		{
			checkNode(iCurrentTile + m_CurrentMap->getGridDims().x, iCurrentTile, 10); //Assign bottom node a cost
		}
		
		
		//If the end wall isn't on the closed list
		if (m_bEndWall)
		{
			//Checks all the surrounding tiles for the end tile
			for (int i = 0; i < m_viTilesToCheck.size(); ++i)
			{
				//If any of the surrounding tiles are the end tile parent it and set the path
				if (iCurrentTile + m_viTilesToCheck.at(i) == iEndTile)
				{
					m_bPathFound = true;
					m_vGrid.at(iEndTile).parent = &m_vGrid.at(iCurrentTile);
				}
			}
		}
		//If current tile is the end tile set the path
		else if (iCurrentTile == iEndTile)
		{
			m_bPathFound = true;
		}
	}
}

void AStar::checkNode(int iTile, int iCurrentTile, int iMoveCost)
{
	//If the node isn't on the open list
	if (std::find(m_vOpenList.begin(), m_vOpenList.end(), &m_vGrid.at(iTile)) == m_vOpenList.end()){
		//If the node isn't on the closed list
		if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iTile)) == m_vClosedList.end()) {
			m_vOpenList.push_back(&m_vGrid.at(iTile)); //Add the node to the open list
			makeCurrent(iTile, iCurrentTile, iMoveCost);
		}
	}
	//If it's faster to move to an already checked node from this node than the previous node
	else if(m_vGrid.at(iCurrentTile).g + iMoveCost < m_vGrid.at(iTile).g){
		makeCurrent(iTile, iCurrentTile, iMoveCost);
	}
}

void AStar::makeCurrent(int iTile, int iCurrentTile, int iMoveCost)
{
	m_vGrid.at(iTile).parent = &m_vGrid.at(iCurrentTile); //Make the parent the current node
	m_vGrid.at(iTile).g = m_vGrid.at(iTile).parent->g + iMoveCost; //The node gets its parents movecost plus the travel cost
	m_vGrid.at(iTile).f = m_vGrid.at(iTile).g + m_vGrid.at(iTile).h; //Calculate the node's F value
}

//Used to check if the end node is surrounded by impassable nodes
bool AStar::validVacinity(int iEndTile)
{
	for (int i = 0; i < m_viTilesToCheck.size(); ++i) // For every surrounding node
	{
		if (iEndTile + m_viTilesToCheck.at(i) > 0 && iEndTile + m_viTilesToCheck.at(i) < m_vGrid.size())// If within the grid space
		{
			if (std::find(m_vClosedList.begin(), m_vClosedList.end(), &m_vGrid.at(iEndTile + m_viTilesToCheck.at(i))) == m_vClosedList.end()) { //And not on the closed list
				return true;
			}
		}
	}

	return false; //If all of the tests are false return false
}

AStar::~AStar()
{
	for (int i = 0; i < m_vOpenList.size(); ++i)
	{
		m_vOpenList.at(i) = NULL;
	}
	for (int i = 0; i < m_vClosedList.size(); ++i)
	{
		m_vClosedList.at(i) = NULL;
	}
}