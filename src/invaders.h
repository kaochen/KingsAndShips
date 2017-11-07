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

	virtual void move(int direction,
		      C_GameUnits::S_layer grid[][GRID_SIZE]);

	virtual void renderLifeBar(int x_screen, int y_screen, SDL_Renderer *renderer);

	virtual void render(int x_screen, int y_screen, SDL_Renderer *renderer);
	protected:
	//attibuts

};

#endif
