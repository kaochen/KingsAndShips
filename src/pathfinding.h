#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <map>
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
	void calcG(std::multimap <int,C_Node*>* m_openNodes);
	int getG() const;
	int getH() const;
	int getF() const;
	bool getOpen() const;
	void setOpen(bool open);
	void setG(int value);

protected:
	int m_x_grid;
	int m_y_grid;
	bool m_block;
	int m_G; // G cost (distance from the starting node)
	int m_H; //H cost heuristic (distance from end to end)
	int m_F; //G cost + H Cost
	bool m_Town;
	bool m_open;
	C_Node* m_Parent;
};


class C_Path
{
public:
	C_Path();
	~C_Path();

	void calcPath(C_Node* start, C_Node* destination);
	void displayOpenList();

	C_Node* searchOpenList(int F);
private:
	std::multimap<int, C_Node*> m_openNodes;
	C_Node* m_destination;
	C_Node* m_start;
};


#endif
