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
	C_invaders(int x_grid, int y_grid,int rank, std::string strDirection);
	~C_invaders();

	virtual void move(int direction);

	virtual void renderLifeBar(int x_screen, int y_screen);

	virtual void render(int x_screen, int y_screen);
	protected:
	//attibuts
	bool m_moving;

};

#endif
