#include <iostream>
#include <string.h>
#include <vector>

#include "towers.h"
#include "invaders.h"

using namespace std;


int main()
{
	//create list of units
	vector < C_Shooter* > gameUnitsList;
	// Load Towers into the list
	gameUnitsList.push_back(new C_Towers );
	gameUnitsList.push_back(new C_Towers(1));

	gameUnitsList.push_back(new C_invaders(2));

	//displayList
	cout << "=====================" << endl;
	for (unsigned int i(0); i < gameUnitsList.size(); i++)
		{
		gameUnitsList[i]->displayStatus();
		}

	cout << "=====================" << endl;
	gameUnitsList[0]->shoot(*gameUnitsList[2]);
	gameUnitsList[2]->displayStatus();

	cout << "=====================" << endl;
	//destroy list
	for (unsigned int i(0); i < gameUnitsList.size(); i++)
		{
		delete gameUnitsList[i];
		gameUnitsList[i]=0;
		}

	return 0;
}
