#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <map>
#include <stack>
#include "settings.h"

#define G_HV 10
#define G_DIAG 14


class C_Node
{
public:
	C_Node(const int x_grid,const int y_grid, const bool block);
	~C_Node();

	void setTown(bool town);
	bool getTown() const;
	void setBlock(bool block);
	bool getBlock() const;
	int getXGrid() const;
	int getYGrid() const;
	void setParent(C_Node * parent);
	C_Node* getParent();
	void displayStatus();

	void calcH(const C_Node* target);
	void calcG(C_Node* gridNode[GRID_SIZE][GRID_SIZE],
		    std::multimap <int,C_Node*>* m_openNodes);
	int getG() const;
	int getH() const;
	int getF() const;
	bool getOpen() const;
	void setOpen(bool open);
	void setG(int value);

protected:
	C_Coord * m_coord;
	bool m_block;
	int m_G; // G cost (distance from the starting node)
	int m_H; //H cost heuristic (distance from end to end)
	int m_F; //G cost + H Cost
	bool m_Town;
	bool m_open;
	C_Node* m_parent;
};


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
	void displayPath();
	std::stack<C_Node*> getPath();
	void goNextStep();
private:

	void loadPath();
	std::multimap<int, C_Node*> m_openNodes;
	C_Node* m_destination;
	C_Node* m_start;
	C_Node* m_gridNode[GRID_SIZE][GRID_SIZE];
	std::stack<C_Node*> m_path;
};


#endif
