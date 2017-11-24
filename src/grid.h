#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>

#include "gameUnits.h"

//singleton
class C_Grid
{
public:
	static	C_Grid& Instances();

	void loadLevel(int levelNbr);

private:
	C_Grid& operator= (const C_Grid&){return *this;}
	C_Grid (const C_Grid&){}

	static C_Grid m_instance;
	C_Grid();
	~C_Grid();

	C_GameUnits::S_layer m_grid[GRID_SIZE][GRID_SIZE];
};

#endif
