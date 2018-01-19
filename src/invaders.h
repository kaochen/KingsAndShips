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
	C_invaders(int x_grid, int y_grid,int rank);
	~C_invaders();

	virtual void move();

	virtual void renderLifeBar(int x_screen, int y_screen);

	virtual void render(S_Coord screen);
	protected:
	//attibuts
	bool m_moving;
	C_Path* m_C_Path;
	int m_speed;
	virtual void updateDirection();
};

#endif
