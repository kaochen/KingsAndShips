#include <iostream>
#include <string.h>
#include <vector>

#include "towers.h"
#include "invaders.h"

using namespace std;


int main()
{
	//create list of units
	vector < C_GameUnits* > gameUnitsList;
	// Load Towers into the list
	gameUnitsList.push_back(new C_Towers );
	gameUnitsList.push_back(new C_Towers("TOWER1",1,"GUN",30,4) );

	//displayList
	for (unsigned int i(0); i < gameUnitsList.size(); i++)
		{
		gameUnitsList[i]->displayStatus();
		}

	C_invaders soldier1("SOLDIER","GUN",10,3);
	soldier1.displayStatus();
	soldier1.displayStatus();

	//destroy list
	for (unsigned int i(0); i < gameUnitsList.size(); i++)
		{
		delete gameUnitsList[i];
		gameUnitsList[i]=0;
		}

	return 0;
}
