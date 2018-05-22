/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
#include "level.h"
#include "menu.h"
#include "landscape.h"


using namespace std;


int main()
{
	//init settings
	C_Set& settings=C_Set::Instances();
	C_Message message;
	C_Time& time=C_Time::Instances();
	C_Window& win=C_Window::Instances();
	win.createWindow();
	//create main window
	SDL_Window* window = win.getWindow ();
	SDL_Renderer* renderer = win.getRenderer ();


//-----------------------------------------------------------------------------

    win.loadGame();

//-----------------------------------------------------------------------------
	C_Grid& grid=C_Grid::Instances();

	//load first level
	C_Level level;
	int levelNbr = 0;
	level.load(levelNbr);

	//displayStatus of the grid
	grid.displayStatus();
	C_Landscape* landscape = new C_Landscape();


	C_Menu& menu=C_Menu::Instances();

//-----------------------------------------------------------------------------


bool quit = false, forceRefresh = false;

//Setup mouse clic
S_Coord cursor;
cursor.x = cursor.y = 1;
S_Coord clic;
clic.x = clic.y = 0;

bool addingAnewTower = false, aTowerIsSelected = false;
bool mouseButtonDown = false;
int buttonType = NONE;

C_Towers* archerTower = new C_ArcherTower(0,0,0);
C_Towers* turbineTower = new C_Turbine(0,0,0);
SDL_Event event;
unsigned int windowID = SDL_GetWindowID(window);
//Start SDL2 loop
message.printM("SDL main loop start \n");

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
				cursor.x = 0;
			else if(event.button.x > settings.getWindowWidth())
				cursor.x = settings.getWindowWidth();
			else
				cursor.x = event.button.x;

		// get y cursor position
			if(event.button.y < 0)
				cursor.y = 0;
			else if(event.button.y > settings.getWindowHeight())
				cursor.y = settings.getWindowHeight();
			else
				cursor.y = event.button.y;

			if (mouseButtonDown){
					if(aTowerIsSelected){
						grid.getSelectedUnit();
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
					clic.x = event.button.x;
					clic.y = event.button.y;
					C_CoordScreen clicleft(clic);


					//Select a Tower
					if(addingAnewTower == false) {
						aTowerIsSelected = grid.selectATower(clicleft);
					}

					//Add a new Tower
					if(addingAnewTower == true && grid.isThisConstructible(clicleft.getGrid ()) == true)
						 {

						grid.addANewTower(buttonType,clicleft.getXGrid (),clicleft.getYGrid (),0);

						aTowerIsSelected = grid.selectATower(clicleft);
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
			case SDLK_d:
				settings.setDebugMode();
				settings.displayDebugMode();
				break;
			case SDLK_n:
				level.sendNextWave();
				break;
			case SDLK_p:
			    if(settings.getDebugMode() == true){
				    settings.setDebugPathMode();
				    settings.displayDebugMode();
				}
				break;
			case SDLK_q:
				quit = true;
                message.printM("The quit command (q) has been pressed.\n");
				break;
			case SDLK_r:
            	level.load(levelNbr);
				break;
			}

		} // end of switch(event.type)

	}//SDL_PollEvent(&event)

	//update status

		if (time.testNewFrame()){
				//cout << "########## Start New Frame " << time.getFrameNbr() << " ##########"<< endl;
				forceRefresh = true;
				time.updateFrameTime();
				//play all units
				grid.playAllUnits();


//render image

	//cout << "render image" << endl;
	if (forceRefresh){
		//cout << "Event Cursor " << event.button.x <<" x:" << xCursor <<"/" << settings.getWindowWidth() << endl;
		//cout << "Event Cursor " << event.button.y <<" y:" << yCursor <<"/" << settings.getWindowHeight() << endl;


		//display game content
        landscape->display();
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

			if (clic.x > xl && clic.x < xr && clic.y > yt && clic.y < yb){
					if(menuButton->getName() == "addNewTower"){
						archerTower->drag(cursor);
						addingAnewTower = true;
						buttonType = ADDNEWTOWER;
						}
					if(menuButton->getName() == "addNewTurbine"){
						turbineTower->drag(cursor);
						addingAnewTower = true;
						buttonType = ADDNEWTURBINE;
						}
	 		}
 		}

		//display menu
		menu.render();
		time.showFPS ();
		//print the result
 		SDL_RenderPresent(win.getRenderer());
 		}

 		//cout << "########## End Frame "  " Duration: "  " ##########"<< endl;
	}

// pause the game loop according to the framerate setting

//cout << "update time & delay" << endl;
	time.delayGameLoop();
}//end of while(!quit)

//-----------------------------------------------------------------------------

	//Cleanup before leaving
    delete landscape;
    delete archerTower;
    delete turbineTower;
	// delete main unit table
	grid.deleteGrid();
	//quitProgram(window, renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	message.printM("Bye\n");
	SDL_Quit();
	return 0;
}
