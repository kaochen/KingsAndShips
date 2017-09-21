#ifndef TOWERS_H
#define TOWERS_H

#include <iostream>
#include <string>
#include "weapons.h"
#include "shooter.h"

class C_Towers: public C_Shooter
{
	public:
	//methods
	C_Towers();
	C_Towers(int rank);
};




#endif
