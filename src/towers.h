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
	C_Towers(SDL_Renderer* renderer);
	C_Towers(SDL_Renderer* renderer,int rank);
	void printOnScreen(SDL_Renderer *renderer);

	protected:
};




#endif
