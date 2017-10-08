#include <iostream>
#include <string.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "settings.h"
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
			      C_Settings::getWindowHeight(),
			      C_Settings::getWindowWith(),
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
	//creating the main table
	size_t const tableSizeX = C_Settings::getWindowWith() / SPRITE_SIZE;
	size_t const tableSizeY = C_Settings::getWindowWith() /SPRITE_SIZE;

	C_GameUnits* grid_units[tableSizeX][tableSizeY];
	//init the table
	for (size_t y = 0; y < tableSizeY; y++){
		for (size_t x = 0; x < tableSizeX; x++){
		grid_units[x][y] = nullptr;
		}
	}
	grid_units[1][1] = new C_Towers(renderer,5);
	grid_units[3][3] = new C_invaders;
	//displayStatus of the grid
	for (size_t y = 0; y < tableSizeY; y++){
		for (size_t x = 0; x < tableSizeX; x++){
			if (grid_units[x][y] != nullptr)
				grid_units[x][y]->displayStatus();
		}
	}


	grid_units[1][1]->shoot(*grid_units[3][3]);

	grid_units[3][3]->displayStatus();

// delete main unit table
	for (size_t y = 0; y < tableSizeY; y++){
		for (size_t x = 0; x < tableSizeX; x++){
			if (grid_units[x][y] != nullptr){
				delete grid_units[x][y];
				grid_units[x][y] = nullptr;
				}
		}
	}


//-----------------------------------------------------------------------------

bool quit = false, forceRefresh = false;
int xCursor = 0, yCursor = 0, currentTime = 0, previousTime = 0;
SDL_Event event;
unsigned int windowID = SDL_GetWindowID(window);
while(!quit)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		 case SDL_QUIT:
			quit = true;
			break;
		 case SDL_WINDOWEVENT:
		 	if (event.window.windowID == windowID && event.window.event == SDL_WINDOWEVENT_EXPOSED)
		 		{ forceRefresh = true;
		 		break;
		 		}
		case SDL_MOUSEMOTION:
		// get x cursor position
			if(event.button.x < 0)
				xCursor = 0;
			else if(event.button.x > WINDOW_HEIGHT)
				xCursor = WINDOW_HEIGHT;
			else
				xCursor = event.button.x;

		// get y cursor position
			if(event.button.y < 0)
				yCursor = 0;
			else if(event.button.y > WINDOW_WIDTH)
				yCursor = WINDOW_WIDTH;
			else
				yCursor = event.button.y;
			break;
		case SDL_KEYDOWN:

			//listen keyboard
			switch(event.key.keysym.sym)
			{
			case SDLK_q:
				quit = true;
				cout << "The quit command (q) has been pressed." << endl;
				break;
			}
		break;

		} // end of switch(event.type)

	}//SDL_PollEvent(&event)

	//if (forceRefresh == true)
	//	cout << "Cursor: x:"<< xCursor << " y:" << yCursor << endl;


	// stop while loop according to the framerate setting
		currentTime = SDL_GetTicks();
		if ((currentTime - previousTime) < (1000 / FRAMERATE))
			SDL_Delay((1000/ FRAMERATE) - (currentTime - previousTime));
		else
			previousTime = currentTime;


}//end of while(!quit)

//-----------------------------------------------------------------------------

	//Cleanup before leaving
	//quitProgram(window, renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	cout << "Bye" << endl;
	return 0;
}
