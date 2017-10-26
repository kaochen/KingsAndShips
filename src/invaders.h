#ifndef INVADERS_H
#define INVADERS_H

#include <iostream>
#include <string>
#include "shooter.h"

class C_invaders: public C_Shooter
{
	public:
	//methods
	C_invaders();
	C_invaders(int rank);
	~C_invaders();
	virtual	void displayStatus() const;

	virtual void move(int x_grid,
		      int y_grid,
		      int direction,
		      C_GameUnits* grid_units[][TABLE_SIZE]);
	protected:
	//attibuts
};

#endif
