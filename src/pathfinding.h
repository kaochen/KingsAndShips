#ifndef PATHFINDING_H
#define PATHFINDING_H
#include "settings.h"

#define G_HV 10
#define G_DIAG 14


class C_Node
{
public:
	C_Node(const int x_grid,const int y_grid);
	~C_Node();

	void setTown(bool town);
	bool getTown() const;
	void setBlock(bool block);
	bool getBlock() const;
	int getXGrid() const;
	int getYGrid() const;
	void displayStatus();

	void calcH(const C_Node* target);
	void calcG();
	int getG() const;
	int getH() const;
	void setG(int value);

protected:
	int m_x_grid;
	int m_y_grid;
	bool m_block;
	int m_G; // G cost (distance from the starting node)
	int m_H; //H cost heuristic (distance from end to end)
	int m_F; //G cost + H Cost
	bool m_Town;
	C_Node* m_Parent;
};


#endif
