#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>

#include "gameUnits.h"

enum layers{GROUND,DEAD,UNITS};

//singleton
class C_Grid
{
public:
	static	C_Grid& Instances();

	void loadLevel(int levelNbr);
	void renderLayer(int layer);

	void addANewBoat(int x, int y, int rank);
	void addANewTower(int x, int y, int rank);
	void moveUnit(int x_from, int y_from, int x_dest, int y_dest);
	void delUnit(int x_grid, int y_grid);
	void moveToDead(int x_grid, int y_grid);

	C_GameUnits* getUnits(int x, int y);
	void setGround(int x, int y, int id);
	int getGround(int x, int y);
	bool isThisConstructible(int x, int y);

	void displayStatus();
	void deleteGrid();

	int xGridToXScreen(int xGrid, int yGrid);
	int yGridToYScreen(int xGrid, int yGrid);
	int xScreenToXGrid(int x_screen, int y_screen);
	int yScreenToYGrid(int x_screen, int y_screen);

private:
	C_Grid& operator= (const C_Grid&){return *this;}
	C_Grid (const C_Grid&){}

	static C_Grid m_instance;
	C_Grid();
	~C_Grid();

	C_GameUnits::S_layer m_grid[GRID_SIZE][GRID_SIZE];
};

#endif
