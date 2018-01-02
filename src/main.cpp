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
#include "menu.h"


using namespace std;


int main()
{
	//init settings
	C_Set& settings=C_Set::Instances();
	C_Time& time=C_Time::Instances();
	C_Window& win=C_Window::Instances();
	win.createWindow();
	//create main window
	SDL_Window* window = win.getWindow ();
	SDL_Renderer* renderer = win.getRenderer ();


//-----------------------------------------------------------------------------

	C_TextureList& t=C_TextureList::Instances();
	//C_Texture text;
	t.loadTexturesIntoMap();
	t.extractTSXfile("data/levels/boat_01.tsx");
	t.extractTSXfile("data/levels/Ground_01.tsx");
	t.extractTSXfile("data/levels/smoke_01.tsx");
	t.extractTSXfile("data/levels/turbine_00.tsx");
	t.extractTSXfile("data/levels/archerTower_00.tsx");
	t.extractTSXfile("data/levels/archerTower_01.tsx");
	t.extractTSXfile("data/levels/buttons.tsx");
	t.displayTexturesList();

//-----------------------------------------------------------------------------
	C_Grid& grid=C_Grid::Instances();
	grid.loadLevel(1);

	vector<C_GameUnits*> towerVector;
	list<C_GameUnits*> lB; //listOfBoats
	list<C_GameUnits*>::iterator itB;


	//load first level
	C_Level level;

	//displayStatus of the grid
	grid.displayStatus();

	C_Menu& menu=C_Menu::Instances();

//-----------------------------------------------------------------------------


bool quit = false, forceRefresh = false;
int xCursor = 1, yCursor = 1;
float xClicLeft = 0, yClicLeft = 0;
int xClicTable = 0, yClicTable = 0;
bool addingAnewTower = false, aTowerIsSelected = false;
bool mouseButtonDown = false;
int buttonType = NONE;

C_Towers* archerTower = new C_ArcherTower(0,0,0);
C_Towers* turbineTower = new C_Turbine(0,0,0);
SDL_Event event;
unsigned int windowID = SDL_GetWindowID(window);
//Start SDL2 loop
cout << "Loop Start" << endl;
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

			if (mouseButtonDown){
					if(aTowerIsSelected){
						C_GameUnits* current = grid.getSelectedUnit();
						if (current != nullptr){
							if (current->getName() == "Turbine")
								current->changeDirection(xCursor,yCursor);
							}
					}
			}


			break;


		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button ==  SDL_BUTTON_LEFT)
				{
				if (addingAnewTower || aTowerIsSelected){
					mouseButtonDown = true;
					}
				}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button ==  SDL_BUTTON_LEFT)
				{
					S_Coord clic;
					clic.x = event.button.x;
					clic.y = event.button.y;
					C_CoordScreen clicleft(clic);
					clicleft.displayStatus();

					//need to be remove
					xClicLeft = event.button.x;
					yClicLeft = event.button.y;
					xClicTable = grid.xScreenToXGrid(xClicLeft, yClicLeft);
					yClicTable = grid.yScreenToYGrid(xClicLeft, yClicLeft);



					//Select a Tower
					if(addingAnewTower == false) {
						aTowerIsSelected = grid.selectATower(xClicLeft, yClicLeft);
					}

					//Add a new Tower
					if(addingAnewTower == true && grid.isThisConstructible(xClicTable,yClicTable) == true)
						 {

						grid.addANewTower(buttonType,xClicTable,yClicTable,0);

						towerVector.push_back(grid.getUnits(clicleft.getGrid()));
						aTowerIsSelected = grid.selectATower(xClicLeft, yClicLeft);
						addingAnewTower = false;
						}
					else{
						addingAnewTower = false;
					}

				mouseButtonDown = false;
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
						(*itB)->move();
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
		grid.renderLayer (GROUND);
		grid.renderLayer (DEAD);
		grid.renderLayer (UNITS);


		//Clic on the addNewTower Button:
		C_MenuItem* menuButton;

		for (int i = 0; i < 2; i++){
			menuButton = menu.getMenuItem(i);
			int xl = menuButton->getXScreen();
			int xr = xl + menuButton->getWidth();
			int yt= menuButton->getYScreen();
			int yb = yt + menuButton->getHeight();
			if (xClicLeft > xl && xClicLeft < xr && yClicLeft > yt && yClicLeft < yb){
					if(menuButton->getName() == "addNewTower"){
						archerTower->drag(xCursor, yCursor);
						addingAnewTower = true;
						buttonType = ADDNEWTOWER;
						}
					if(menuButton->getName() == "addNewTurbine"){
						turbineTower->drag(xCursor, yCursor);
						addingAnewTower = true;
						buttonType = ADDNEWTURBINE;
						}
	 		}
 		}

		//display menu
		menu.render();

		//print the result
 		SDL_RenderPresent(win.getRenderer());
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
