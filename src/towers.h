#ifndef TOWERS_H
#define TOWERS_H

#include <iostream>
#include <string>
#include "weapons.h"
#include "shooter.h"
#include "surfaces.h"

class C_Towers: public C_Shooter
{
	public:
	//methods
	C_Towers();
	C_Towers(int x_grid, int y_grid, int rank, C_GameUnits* grid_units[][TABLE_SIZE]);

	protected:
};




#endif
