#include <iostream>
#include <string.h>
#include <vector>

#include "window.h"
#include "towers.h"
#include "invaders.h"

using namespace std;


int main()
{
	//create main window
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	createWindow(window,renderer);


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

	cout << "start of delay" << endl;
	SDL_Delay(4000);
	cout << "end of delay" << endl;

	//Cleanup before leaving
	quitProgram(window, renderer);
	return 0;
}
