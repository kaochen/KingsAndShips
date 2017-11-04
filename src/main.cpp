#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "settings.h"
#include "time.h"
#include "window.h"
#include "towers.h"
#include "invaders.h"
#include "gameUnits.h"
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
			      C_Settings::getWindowWidth(),
			      C_Settings::getWindowHeight(),
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
	//creating the main table to store C_GameUnits position
	size_t const gridSize = C_Settings::getGridSize();

	C_GameUnits* grid_units[TABLE_SIZE][TABLE_SIZE];

	vector<C_GameUnits*> towerVector;
	list<C_GameUnits*> lB; //listOfBoats
	list<C_GameUnits*>::iterator itB;

	//init the table
	for (size_t y = 0; y < gridSize; y++){
		for (size_t x = 0; x < gridSize; x++){
		grid_units[x][y] = nullptr;
		}
	}


	//fill table with tiles for testing
	//towerVector.push_back(new C_Towers(10,10,0, grid_units));
	//towerVector.push_back(new C_Towers(15,15,1, grid_units));
	//towerVector.push_back(new C_Towers(11,17,1, grid_units));

	lB.push_back(new C_invaders(1,14,1, grid_units));
	lB.push_back(new C_invaders(5,12,1, grid_units));

	//displayStatus of the grid
	displayGridStatus(grid_units);


//-----------------------------------------------------------------------------
C_Texture::loadTexturesIntoMap(renderer);
C_Time t;
bool quit = false, forceRefresh = false;
int xCursor = 0, yCursor = 0;
float xClicLeft = 0, yClicLeft = 0;
int xClicTable = 0, yClicTable = 0;
bool towerSelected = false;
SDL_Event event;
unsigned int windowID = SDL_GetWindowID(window);
while(!quit)
{
	t.displayTime();

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
			else if(event.button.x > C_Settings::getWindowWidth())
				xCursor = C_Settings::getWindowWidth();
			else
				xCursor = event.button.x;

		// get y cursor position
			if(event.button.y < 0)
				yCursor = 0;
			else if(event.button.y > C_Settings::getWindowHeight())
				yCursor = C_Settings::getWindowHeight();
			else
				yCursor = event.button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button ==  SDL_BUTTON_LEFT)
				{
					xClicLeft = event.button.x;
					yClicLeft = event.button.y;
					cout << "\tx_screen:" << xClicLeft << " y_screen:" << yClicLeft << endl;
					//find the match point into the grid
					float xOffset = (C_Settings::getWindowWidth() /2);
					float yOffset = (C_Settings::getWindowHeight() /2);
					//cout << "x:" << xClicLeft << " y:" << yClicLeft << "with offset\n";
					float tempX = 0.0, tempY = 0.0;
					tempY = ( (yClicLeft + yOffset )/(TILE_HALF_HEIGHT*2) - (xClicLeft - xOffset)/(TILE_HALF_WIDTH*2));
					tempX = ( ((xClicLeft - xOffset ) / TILE_HALF_WIDTH + (yClicLeft + yOffset)/TILE_HALF_HEIGHT )/2);
					yClicTable = tempY;
					xClicTable = tempX;

					//cout << "\tfloat x:" << tempX << " y:" << tempY << endl;
					cout << "\tx_grid:" << xClicTable << " y_grid:" << yClicTable << endl;
					if(towerSelected == true && grid_units[xClicTable][yClicTable] == nullptr) {
						towerVector.push_back(new C_Towers(xClicTable, yClicTable,0, grid_units));
						towerSelected = false;
						}
				}

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

	//move boats every two frames
			if (t.getFrameNbr() % 2 == 0){
				for (itB = lB.begin(); itB != lB.end(); itB++){
						C_GameUnits* tmp = *itB;
						tmp->move(EAST, grid_units);
				}
				forceRefresh = true;
			}


		if (t.getSec() % 1 == 0 && t.getFrameNbr() == 1){
					for (size_t i = 0; i < towerVector.size(); i++){
						size_t nbrOfBoats = 0;
						map<int, C_GameUnits*> boatDistanceList;
						priority_queue<int> closestList;
						for (itB = lB.begin(); itB != lB.end(); itB++){
							C_GameUnits* boat = *itB;
							if(boat != nullptr){
								int x = boat->getXScreen();
								int y = boat->getYScreen();
								int dist = towerVector[i]->testFirerange(x,y);
								if (dist >= 0){
									boatDistanceList[dist] = boat;
									closestList.push(dist*(-1));
									nbrOfBoats++;
								} // -1 to reverse the list
							}
						}
						if (nbrOfBoats > 0){
							int closest = closestList.top()*(-1);
							towerVector[i]->shoot(*boatDistanceList[closest]);
						}
					}
			}



// drop dead boats
		if (t.getFrameNbr() == t.getFramerate()){
				itB = lB.begin();
				while (itB != lB.end()){
					C_GameUnits* boat = *itB;
					if (boat->alive() == false){
						lB.erase(itB++);
						boat->del(grid_units);
					}
					else{
						itB++;
					}
				}
			}




	if (forceRefresh == true){
		//cout << "Event Cursor " << event.button.x <<" x:" << xCursor <<"/" << C_Settings::getWindowWidth() << endl;
		//cout << "Event Cursor " << event.button.y <<" y:" << yCursor <<"/" << C_Settings::getWindowHeight() << endl;

		SDL_RenderClear(renderer);

		//add a setup background
		renderTexture(C_Texture::getText("SetupBackground.png"), renderer, (C_Settings::getWindowWidth()/2),0);

		//display game content
		displayGridContent(renderer, grid_units);
		//display menu
 		renderTexture(C_Texture::getText("CrossBow_01.png"), renderer, 30,100);
		//show cursor :
		if (xClicLeft > 0 && xClicLeft < 64 && yClicLeft > 100 && yClicLeft < 164){
 			renderTexture(C_Texture::getText("Tile_Highlight_Green.png"), renderer, xCursor,yCursor -100);
 			renderTexture(C_Texture::getText("Tower_00_00.png"), renderer, xCursor,yCursor -150);
 			towerSelected = true;
 		}
 		SDL_RenderPresent(renderer);
 		}


	// pause the game loop according to the framerate setting
		t.updateTime();
		t.updateFrameNbr();


}//end of while(!quit)

//-----------------------------------------------------------------------------

	//Cleanup before leaving

	// delete main unit table
	for (size_t y = 0; y < gridSize; y++){
		for (size_t x = 0; x < gridSize; x++){
			if (grid_units[x][y] != nullptr){
				delete grid_units[x][y];
				grid_units[x][y] = nullptr;
				}
		}
	}
	//quitProgram(window, renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	cout << "Bye" << endl;
	return 0;
}
