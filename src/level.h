#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>
#include <list>
#include "game.h"
#include "gameUnits.h"
#include "invaders.h"


class C_Level
{

	public:

	//methods
	C_Level();
	~C_Level();
	void status();
	void sendNextWave(std::list<C_GameUnits*> boatList);

	protected:
	//attibuts
	std::string m_name;
	int m_count;
	int m_id;
};







#endif
