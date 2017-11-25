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
#include "grid.h"
#include "window.h"
#include "towers.h"
#include "invaders.h"
#include "gameUnits.h"
#include "surfaces.h"
#include "level.h"



using namespace std;


int main()
{
	//init SDL
 	initSDL();
	SDL_ClearError();

	//init settings
	C_Set& settings=C_Set::Instances();
	C_Time& time=C_Time::Instances();

	//create main window
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
 	window = SDL_CreateWindow("TOWER",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      settings.getWindowWidth(),
			      settings.getWindowHeight(),
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
	C_Grid& grid=C_Grid::Instances();
	grid.loadLevel(1);


	vector<C_GameUnits*> towerVector;
	list<C_GameUnits*> lB; //listOfBoats
	list<C_GameUnits*>::iterator itB;

	vector <C_Texture*> textureList;
	C_Texture t;
	t.extractTSXfile(textureList);
	t.displayTexturesList(textureList);

	//load first level
	C_Level level;

	//displayStatus of the grid
	grid.displayStatus();


//-----------------------------------------------------------------------------
C_Texture::loadTexturesIntoMap(renderer);
bool quit = false, forceRefresh = false;
int xCursor = 0, yCursor = 0;
float xClicLeft = 0, yClicLeft = 0;
int xClicTable = 0, yClicTable = 0;
bool towerSelected = false;
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
		 	break;
		case SDL_MOUSEMOTION:
		// get x cursor position
			if(event.button.x < 0)
				xCursor = 0;
			else if(event.button.x > settings.getWindowWidth())
				xCursor = settings.getWindowWidth();
			else
				xCursor = event.button.x;

		// get y cursor position
			if(event.button.y < 0)
				yCursor = 0;
			else if(event.button.y > settings.getWindowHeight())
				yCursor = settings.getWindowHeight();
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
					float xOffset = (settings.getWindowWidth() /2);
					float yOffset = (settings.getWindowHeight() /2);
					//cout << "x:" << xClicLeft << " y:" << yClicLeft << "with offset\n";
					float tempX = 0.0, tempY = 0.0;
					tempY = ( (yClicLeft + yOffset )/(TILE_HALF_HEIGHT*2) - (xClicLeft - xOffset)/(TILE_HALF_WIDTH*2));
					tempX = ( ((xClicLeft - xOffset ) / TILE_HALF_WIDTH + (yClicLeft + yOffset)/TILE_HALF_HEIGHT )/2);
					yClicTable = tempY;
					xClicTable = tempX;

					//cout << "\tfloat x:" << tempX << " y:" << tempY << endl;
					cout << "\tx_grid:" << xClicTable << " y_grid:" << yClicTable << endl;
					if(towerSelected == true && grid.getUnits(xClicTable,yClicTable) == nullptr) {
						grid.addANewTower(xClicTable,yClicTable,0);
						towerVector.push_back(grid.getUnits(xClicTable,yClicTable));
						towerSelected = false;
						}

				}
			break;
		case SDL_KEYDOWN:

			//listen keyboard
			switch(event.key.keysym.sym)
			{
			case SDLK_q:
				quit = true;
				cout << "The quit command (q) has been pressed." << endl;
				break;
			case SDLK_n:
				level.sendNextWave(lB);
				break;
			}

		} // end of switch(event.type)

	}//SDL_PollEvent(&event)

	time.updateTime();
	//update status
	//move boats every two frames
		if (time.testNewFrame()){
				//cout << "########## Start New Frame " << time.getFrameNbr() << " ##########"<< endl;
				long frameStartTime = SDL_GetTicks();
				//cout << "Time: " << frameStartTime << endl <<"update status" << endl;
				forceRefresh = true;
				//move
				for (itB = lB.begin(); itB != lB.end(); itB++){
						C_GameUnits* tmp = *itB;
						tmp->move(EAST);
				}

				for (size_t i = 0; i < towerVector.size(); i++){
					size_t nbrOfBoats = 0;
					map<int, C_GameUnits*> boatDistanceList;
					priority_queue<int> closestList;
					for (itB = lB.begin(); itB != lB.end(); itB++){
						C_GameUnits* boat = *itB;
						if(boat != nullptr){
							int dist = towerVector[i]->testFirerange(*boat);
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
					else {
						towerVector[i]->stopShooting();
						}
				}


		// drop dead boats
				itB = lB.begin();
				while (itB != lB.end()){
					C_GameUnits* boat = *itB;
					if (boat->alive() == false){
						lB.erase(itB++);
						boat->kill();
					}
					else{
						itB++;
					}
				}

//render image

	//cout << "render image" << endl;
	if (forceRefresh){
		//cout << "Event Cursor " << event.button.x <<" x:" << xCursor <<"/" << settings.getWindowWidth() << endl;
		//cout << "Event Cursor " << event.button.y <<" y:" << yCursor <<"/" << settings.getWindowHeight() << endl;
		//change background color before clear
		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
		SDL_RenderClear(renderer);
		//display game content
		grid.renderFloor(renderer);
		grid.renderUnits(renderer);

		//display menu
 		renderTexture(C_Texture::getText("CrossBow_01.png"), renderer, 30,100);
		//show cursor :
		if (xClicLeft > 0 && xClicLeft < 64 && yClicLeft > 100 && yClicLeft < 164){
			drawElipse(renderer,xCursor,yCursor,100);
 			renderTexture(C_Texture::getText("Tile_Highlight_Green.png"), renderer, xCursor,yCursor -100);
 			renderTexture(C_Texture::getText("Tower_00_00.png"), renderer, xCursor,yCursor -150);
 			towerSelected = true;
 		}
 		SDL_RenderPresent(renderer);
 		}

		time.updateFrameNbr(frameStartTime, SDL_GetTicks());
 		//cout << "########## End Frame "  " Duration: "  " ##########"<< endl;
	}

// pause the game loop according to the framerate setting

//cout << "update time & delay" << endl;
	//time.displayTime();
	time.delayGameLoop();
}//end of while(!quit)

//-----------------------------------------------------------------------------

	//Cleanup before leaving

	// delete main unit table
	grid.deleteGrid();
	//quitProgram(window, renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	cout << "Bye" << endl;
	return 0;
}
