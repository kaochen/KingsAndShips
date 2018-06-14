/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//Pathfinding A* functions to help boats to find their way to the town.

#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <map>
#include <stack>

#include "../coord.h"
#include "node.h"

class C_Path
{
public:
	C_Path(int x_dest, int y_dest);
	~C_Path();

	void calcPath(int x_start,int y_start, int x_dest, int y_dest);
	void displayOpenList();

	C_Node* searchOpenList(int F);
	void setTown(int x_grid,int y_grid);

	void showPath();
	void show_H_G_F();
	C_Node* closestNode();
  bool closeToDestination(int x_grid, int y_grid, int steps);
	void displayPath();
	std::stack<C_Node*> getPath();
	void goNextStep();
  void addANodeAtTheStartOfThePath(S_Coord grid);
private:
	int findLowestF();
	void loadPath();
	std::multimap<int, C_Node*> m_openNodes;
	C_Node* m_destination;
	C_Node* m_start;
	C_Node* m_gridNode[GRID_SIZE][GRID_SIZE];
	std::stack<C_Node*> m_path;
};


#endif