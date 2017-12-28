#ifndef PATHFINDING_H
#define PATHFINDING_H
#include "settings.h"


class C_Node
{
public:
	C_Node(const int x_grid,const int y_grid);
	~C_Node();

	void setTown(bool town);
	void displayStatus();

private:
	int m_x_grid;
	int m_y_grid;
	int m_G; // G cost (distance from the starting node)
	int m_H; //H cost heuristic (distance from end to end)
	int m_F; //G cost + H Cost
	bool m_Town;
	C_Node* m_Parent;
};


#endif
