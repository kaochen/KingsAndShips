#include <iostream>
#include <string.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "towers.h"
#include "invaders.h"
#include "surfaces.h"


using namespace std;


int main()
{
	//init SDL
 	initSDL();
	SDL_ClearError();

	//create main window

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
 	window = SDL_CreateWindow("TOWER",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      800,
			      600,
			      SDL_WINDOW_MOUSE_FOCUS);

	if (window == nullptr){
			logSDLerror("SDL_CreateWindow() failed");
			SDL_Quit();
		}
	else{
			// Create a renderer from the window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr){
					SDL_DestroyWindow(window);
					logSDLerror("SDL_GetWindowSurface() failed");
					SDL_Quit();
				}
		}
	cout << "The main window has been created successfully" << endl;

//-----------------------------------------------------------------------------

	//create list of units
	vector < C_Shooter* > gameUnitsList;
	// Load Towers into the list
	gameUnitsList.push_back(new C_Towers(renderer) );
	gameUnitsList.push_back(new C_Towers(renderer,1));
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

//-----------------------------------------------------------------------------

	SDL_Texture *image = loadTexture("data/img/original/Tower_01.png", renderer);
	//gameUnitsList[0]->updateImage(image);

	renderTexture(image, renderer, 10, 40);
	SDL_RenderPresent(renderer);

	cout << "marker 1" << endl;
	//gameUnitsList[1]->printOnScreen(renderer);
	//SDL_RenderPresent(renderer);

	cout << "start of delay" << endl;
	SDL_Delay(4000);
	cout << "end of delay" << endl;

	//Cleanup before leaving
	//quitProgram(window, renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	cout << "Bye" << endl;
	return 0;
}
