#include "../include/AStar.h"

AStar::AStar()
{

}

AStar::AStar(Map* map1)
{
	setup(map1);
}

void AStar::setup(Map* map1)
{
	m_CurrentMap = map1;
	m_Grid.resize(m_CurrentMap->getGridDims().x * m_CurrentMap->getGridDims().y);
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

std::deque<Node*> AStar::findPath(sf::Vector2f startPos, sf::Vector2f endPos)
{
	//Calculates the Position of the Start Tile in the Grid
	int iXTile = (int)startPos.x / (int)m_CurrentMap->getTileSize().x;
	int iYTile = (int)startPos.y / (int)m_CurrentMap->getTileSize().y;
	int iStartTile = iXTile + ((int)m_CurrentMap->getGridDims().x * iYTile);

	//Calculates the Position of the End Tile in the Grid
	iXTile = (int)endPos.x / (int)m_CurrentMap->getTileSize().x;
	iYTile = (int)endPos.y / (int)m_CurrentMap->getTileSize().y;
	int iEndTile = iXTile +  ((int)m_CurrentMap->getGridDims().x * iYTile);

	m_bEndWall = false;

	int iCurrentTile = iStartTile; //Holds the tile being checked

	if (iStartTile != iEndTile && iStartTile < m_Grid.size() && iEndTile < m_Grid.size() && validVacinity(iEndTile))
	{
		//Checks that the end tile isn't on the closed list
		if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iEndTile)) != m_ClosedList.end())
		{
			m_bEndWall = true;
		}

		//Calculates the Manhattan distance for every node to the end node.
		for (int i = 0; i < m_Grid.size(); i++)
		{
			calculateManhattan(i, iEndTile);
			m_Grid.at(i).index = i;
		}

		int iNextIndex = NULL;
		while (!m_bPathFound)
		{
			//If the tile being checked isn't the end tile
			if (iCurrentTile != iEndTile)
			{
				//Pushes the node to the closed list
				m_ClosedList.push_back(&m_Grid.at(iCurrentTile));
			}

			//Calculates the move cost for the surrounding nodes
			calculateMoveCost(iCurrentTile, iEndTile);

			if (!m_bPathFound)
			{
				int iLowestF = 1000;
				//Checks for the lowest F value in the open list
				for (int i = 0; i < m_OpenList.size(); i++)
				{
					if (m_OpenList.at(i)->f < iLowestF)
					{
						iCurrentTile = m_OpenList.at(i)->index;
						iLowestF = m_OpenList.at(i)->f;
						iNextIndex = i;
					}
				}
				m_OpenList.erase(m_OpenList.begin() + iNextIndex);
			}
		}

		Node* CurrentNode;
		//If the end is not on the closed list
		if (!m_bEndWall)
		{
			CurrentNode = &m_Grid.at(iEndTile);
			m_Path.push_front(CurrentNode);
		}
		//Else set it's parent to be the end node
		else
		{
			CurrentNode = m_Grid.at(iEndTile).parent;
			m_Path.push_front(CurrentNode);
		}

		//If the current node isn't the start node
		if (CurrentNode->index != iStartTile)
		{
			//Loop tace back through the parents to find the path
			do{
				CurrentNode = CurrentNode->parent;
				m_Path.push_front(CurrentNode);
			} while (CurrentNode->parent != NULL);
		}
	}
	m_bPathFound = false;
	return m_Path;
}

void AStar::setupLists()
{
	//Clears the open and closed lists
	m_ClosedList.clear();
	m_OpenList.clear();
	m_Path.clear();

	for (int i = 0; i < m_CurrentMap->getMapData().size(); i++)
	{
		for (int j = 0; j < m_CurrentMap->getMapData().at(i).size(); j++)
		{
			if (m_CurrentMap->getMapData().at(i).at(j) == 'W')
			{
				m_ClosedList.push_back(&m_Grid.at((i*m_CurrentMap->getGridDims().x) + j));
			}
		}
	}

	for (int i = 0; i < m_Grid.size(); i++)
	{
		m_Grid.at(i).parent = NULL;
		m_Grid.at(i).h = 0;
		m_Grid.at(i).f = 0;
		m_Grid.at(i).g = 0;
	}
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

	m_Grid.at(iStartTile).h = iXDiff + iYDiff;
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
				if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x + 1)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile + 1)) != m_ClosedList.end()) {
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x + 1, iCurrentTile, 100);  //Make the cost so high its more probable to walk around it
				}
				else
				{
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x + 1, iCurrentTile, 14); //Otherwise allow diagonal movement
				}
			}

			//If also not on the bottom edge of the screen
			if (iCurrentTile + m_CurrentMap->getGridDims().x < m_Grid.size())
			{
				//Assign bottom right node a cost
				//If the tile to the right, the bottom or the bottom right of the tile is on the closed list
				if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x + 1)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile + 1)) != m_ClosedList.end()) {
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
				if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x - 1)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile - (int)m_CurrentMap->getGridDims().x)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile - 1)) != m_ClosedList.end()) {
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x - 1, iCurrentTile, 100);  //Make the cost so high its more probable to walk around it
				}
				else
				{
					checkNode(iCurrentTile - m_CurrentMap->getGridDims().x - 1, iCurrentTile, 14);//Otherwise allow diagonal movement
				}
			}

			//If also not on the bottom edge of the screen
			if (iCurrentTile + m_CurrentMap->getGridDims().x < m_Grid.size())
			{
				//Assign bottom left node a cost
				//If the tile to the left, the bottom or the bottom left of the tile is on the closed list
				if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x - 1)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile + (int)m_CurrentMap->getGridDims().x)) != m_ClosedList.end() ||
					std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iCurrentTile - 1)) != m_ClosedList.end()) {
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
		if (iCurrentTile + m_CurrentMap->getGridDims().x < m_Grid.size())
		{
			checkNode(iCurrentTile + m_CurrentMap->getGridDims().x, iCurrentTile, 10); //Assign bottom node a cost
		}
		
		
		//If the end wall isn't on the closed list
		if (m_bEndWall)
		{
			//Checks all the surrounding tiles for the end tile
			for (int i = 0; i < m_viTilesToCheck.size(); i++)
			{
				//If any of the surrounding tiles are the end tile parent it and set the path
				if (iCurrentTile + m_viTilesToCheck.at(i) == iEndTile)
				{
					m_bPathFound = true;
					m_Grid.at(iEndTile).parent = &m_Grid.at(iCurrentTile);
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
	if (std::find(m_OpenList.begin(), m_OpenList.end(), &m_Grid.at(iTile)) == m_OpenList.end()){
		//If the node isn't on the closed list
		if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iTile)) == m_ClosedList.end()) {
			m_OpenList.push_back(&m_Grid.at(iTile)); //Add the node to the open list
			makeCurrent(iTile, iCurrentTile, iMoveCost);
		}
	}
	//If it's faster to move to an already checked node from this node than the previous node
	else if(m_Grid.at(iCurrentTile).g + iMoveCost < m_Grid.at(iTile).g){
		makeCurrent(iTile, iCurrentTile, iMoveCost);
	}
}

void AStar::makeCurrent(int iTile, int iCurrentTile, int iMoveCost)
{
	m_Grid.at(iTile).parent = &m_Grid.at(iCurrentTile); //Make the parent the current node
	m_Grid.at(iTile).g = m_Grid.at(iTile).parent->g + iMoveCost; //The node gets its parents movecost plus the travel cost
	m_Grid.at(iTile).f = m_Grid.at(iTile).g + m_Grid.at(iTile).h; //Calculate the node's F value
}

//Used to check if the end node is surrounded by impassable nodes
bool AStar::validVacinity(int iEndTile)
{
	for (int i = 0; i < m_viTilesToCheck.size(); i++) // For every surrounding node
	{
		if (iEndTile + m_viTilesToCheck.at(i) > 0 && iEndTile + m_viTilesToCheck.at(i) < m_Grid.size())// If within the grid space
		{
			if (std::find(m_ClosedList.begin(), m_ClosedList.end(), &m_Grid.at(iEndTile + m_viTilesToCheck.at(i))) == m_ClosedList.end()) { //And not on the closed list
				return true;
			}
		}
	}

	return false; //If all of the tests are false return false
}

AStar::~AStar()
{
	for (int i = 0; i < m_OpenList.size(); i++)
	{
		m_OpenList.at(i) = NULL;
	}
	for (int i = 0; i < m_ClosedList.size(); i++)
	{
		m_ClosedList.at(i) = NULL;
	}
}