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


#include "window.h"
#include "settings.h"

#include "texture.h"
#include "menu.h"
#include "level/grid.h"
#include "level/gameUnits.h"
#include "level/level.h"
#include "level/landscape.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;
C_Window C_Window::m_instance=C_Window();

C_Window::C_Window()
{
}

C_Window::~C_Window(){
}

C_Window& C_Window::Instances()
{
	return m_instance;
}

void C_Window::initSDL()
{
    C_Message m;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0){
		m.printSDLerror("SDL_Init() failed");
			exit (EXIT_FAILURE);
		}

	if(TTF_Init() < 0){
			m.printTTFerror("TTF_init() failed");
			exit (EXIT_FAILURE);
		}


	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		m.printSDLerror("IMG_Init");
		//SDL_Quit;
		}
}

void C_Window::createWindow(){
 	initSDL();
    C_Message m;
	C_Settings& settings=C_Settings::Instances();
 	m_window = SDL_CreateWindow("KingsAndShips",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      settings.getWindowWidth(),
			      settings.getWindowHeight(),
			      SDL_WINDOW_MOUSE_FOCUS);

	if (m_window == nullptr)
		{
    		m.printSDLerror("SDL_CreateWindow() failed");
			SDL_Quit();
		}
	else
		{
			// Create a renderer from the window
			m_renderer = SDL_CreateRenderer(m_window, -1,SDL_RENDERER_ACCELERATED);
			if (m_renderer == nullptr)
				{
					SDL_DestroyWindow(m_window);
            		m.printSDLerror("SDL_GetWindowSurface() failed");
					SDL_Quit();
				}
		}
        m.printM("The main window has been created successfully\n");
}



SDL_Window* C_Window::getWindow(){
	return m_window;
};

SDL_Renderer* C_Window::getRenderer(){
	return m_renderer;
};

void C_Window::loadGame(){
	C_Settings& settings=C_Settings::Instances();
	C_TextureList& t=C_TextureList::Instances();

	int size = 14;
	string tsxList[size] {"Ground_01.tsx","boat_01.tsx","town_01.tsx",
	                    "Rocks_00.tsx","Trees_00.tsx","turbine_00.tsx",
	                    "archerTower_00.tsx","archerTower_01.tsx","buttons.tsx",
	                    "Water_00.tsx","smoke_01.tsx","charaters.tsx","boat_dead_01.tsx","Weapons.tsx"};
	//create texture from the path
	SDL_Color color = {0,0,0,255};

    for(int i = 0; i < size; i++){
        string imgPath = settings.getImgFolder() + tsxList[i];
        t.loadTextAsTexturesIntoMap(imgPath, imgPath, 20, color);
        loadingPage(i+1, imgPath,size+1);
	    t.extractTSXfile(imgPath);
    }
	t.displayTexturesList();
}

void C_Window::loadingPage(int progress, string label, int stepsNbr){
	    SDL_SetRenderDrawColor(m_renderer, 64, 64, 64, 255);
	    SDL_RenderClear(m_renderer);
        renderProgressBar(progress, label, stepsNbr);
	    SDL_RenderPresent(m_renderer);
	}



void C_Window::renderProgressBar(int progress, string label, int stepsNbr)
	{
		C_Settings& settings=C_Settings::Instances();
		int height = 40, width = settings.getWindowWidth()/2;
		int x_screen = 	(settings.getWindowWidth()- width) /2;
		int y_screen =  (settings.getWindowHeight() - height)/2;
		//add a life status above the boat
		int step = width/stepsNbr;

		SDL_Rect back, fill;
		    back.x = x_screen;
		    back.y = y_screen;
		    back.w = width;
		    back.h = height;

		    fill.x = back.x + 1;
		    fill.y = back.y + 1;
		    fill.w = step*progress;
		    fill.h = back.h - 2;

		    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255 );
		    SDL_RenderFillRect(m_renderer, &back);
		    SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255 );
		    SDL_RenderFillRect(m_renderer, &fill);

		SDL_Rect stripes;
		    stripes.w = 1;
		    stripes.h = height;
		    stripes.y= back.y;
    		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255 );
    		//cout << step <<":"<< width << "/" << stepsNbr << " p:"<< progress<< endl;
		    for(int i = 1; i < stepsNbr;i++){
		        stripes.x = x_screen + i*step;
		        SDL_RenderFillRect(m_renderer, &stripes);
		    }

		    C_TextureList& t=C_TextureList::Instances();
		    t.renderTexture(label,settings.getWindowWidth()/2, settings.getWindowHeight()/2 + 2*height);
	}


void C_Window::quitProgram()
{
    C_Message m;
	//Cleanup before leaving
    C_Grid& grid=C_Grid::Instances();
	grid.deleteGrid();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	TTF_Quit();
	SDL_Quit();
	m.printM("Bye\n");
}


void C_Window::gameLoop(){

    C_Settings& settings=C_Settings::Instances();
	C_Message message;
	C_Time& time=C_Time::Instances();
    C_Grid& grid=C_Grid::Instances();

	//load first level
	C_Level* level = new C_Level;
	int levelNbr = settings.getCurrentLevelNbr();
	level->load(levelNbr);

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
unsigned int windowID = SDL_GetWindowID(m_window);
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
			case SDLK_l:
			    levelNbr = settings.setCurrentLevelNbr(levelNbr +1);
                delete level;
                level = new C_Level;
            	level->load(levelNbr);
            	menu.resetValues();
				break;
			case SDLK_n:
				level->sendNextWave();
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
            	level->load(levelNbr);
            	menu.resetValues();
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
        listenButtons(clic, cursor, buttonType, addingAnewTower, archerTower, turbineTower);

		//display menu
		menu.render();
		time.showFPS ();
		//print the result
 		SDL_RenderPresent(m_renderer);
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
    delete level;
	// delete main unit table
}


void C_Window::listenButtons(S_Coord clic, S_Coord cursor, int &buttonType, bool &addingAnewTower,
        C_Towers* archerTower, C_Towers* turbineTower){
    	C_Menu& menu=C_Menu::Instances();

		C_MenuItem* menuButton;

		for (int i = 0; i < 2; i++){
			menuButton = menu.getMenuItem(i);
			int xl = menuButton->getXScreen();
			int xr = xl + menuButton->getWidth();
			int yt= menuButton->getYScreen();
			int yb = yt + menuButton->getHeight();
            //reset state
	 		menuButton->setState(ACTIVE);

			if (clic.x > xl && clic.x < xr && clic.y > yt && clic.y < yb){
					if(menuButton->getName() == "AddTower"){
						archerTower->drag(cursor);
						addingAnewTower = true;
						buttonType = ADDNEWTOWER;
						}
					if(menuButton->getName() == "AddTurbine"){
						turbineTower->drag(cursor);
						addingAnewTower = true;
						buttonType = ADDNEWTURBINE;
						}
	 		}
	 		//mouse Over
	 		if (cursor.x > xl && cursor.x < xr && cursor.y > yt && cursor.y < yb){
                    if(menuButton->getName() == "AddTower" || menuButton->getName() == "AddTurbine"){
                        menuButton->setState(HOVER);
						}
	 		}
 		}

}
