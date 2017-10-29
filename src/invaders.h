#ifndef INVADERS_H
#define INVADERS_H

#include <iostream>
#include <string>
#include "shooter.h"

class C_invaders: public C_Shooter
{
	public:
	//methods
	C_invaders(int x_grid, int y_grid,int rank,C_GameUnits* grid_units[][TABLE_SIZE]);
	~C_invaders();

	virtual void move(int direction,
		      C_GameUnits* grid_units[][TABLE_SIZE]);

	virtual int getXoffset();
	virtual int getYoffset();

	virtual void renderLifeBar(int x_screen, int y_screen, SDL_Renderer *renderer);

	virtual void render(int x_iso, int y_iso, SDL_Renderer *renderer);
	protected:
	//attibuts
	int m_x_screen_offset;
	int m_y_screen_offset;

};

#endif
