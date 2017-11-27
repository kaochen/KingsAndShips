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
	C_Towers(int x_grid, int y_grid, int rank);

	virtual void render(int x_screen, int y_screen);
	void renderSmoke();
	protected:
	long m_lastSmokeTime;
	int m_smokeNbr;
	bool m_justAdded;
};




#endif
