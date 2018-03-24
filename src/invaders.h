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

	virtual void play();
	virtual void move();

	virtual void receiveDamage(S_Weapon weapon);

	protected:
	virtual void updateDirection();
	virtual void render(S_Coord screen);
	virtual void renderLifeBar(int x_screen, int y_screen);

	//attributs
	bool m_moving;
	C_Path* m_C_Path;
	int m_speed;
};

#endif
