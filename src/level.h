#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "game.h"
#include "gameUnits.h"
#include "invaders.h"

struct S_boat{
	int rank;
	int x;
	int y;
};

class C_Level
{

	public:

	//methods
	C_Level();
	~C_Level();
	void status();
	void sendNextWave(std::vector <S_boat>& l);

	protected:
	//attibuts
	std::string m_name;
	int m_count;
	int m_id;
};







#endif