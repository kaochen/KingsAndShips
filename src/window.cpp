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
#include "message.h"
#include "level/grid.h"
#include "level/gameUnits.h"
#include "level/barricade.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <queue>

using namespace std;
C_Window C_Window::m_instance=C_Window();

C_Window::C_Window()
{
	C_Settings& settings=C_Settings::Instances();
    m_forceRefresh = false;

    m_level = new C_Level;
    m_levelNbr = settings.getCurrentLevelNbr();

    m_landscape = new C_Landscape();

    m_buttonType = "";
    m_cursor.x = m_cursor.y = 1;
    m_clic.x = m_clic.y = 0;
    m_mouseButtonDown = false;

    m_archerTower = new C_ArcherTower(0,0,0);
    m_turbineTower = new C_Turbine(0,0,0);
    m_barricade = new C_Barricade(0,0,1);
    m_addingAnewTower = false;
    m_aTowerIsSelected = false;

    m_quit = false;
}

C_Window::~C_Window(){
    delete m_level;
    delete m_landscape;
    delete m_archerTower;
    delete m_turbineTower;
    delete m_barricade;
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

void C_Window::loadGame(){
	C_TextureList& t=C_TextureList::Instances();
	C_Settings& settings=C_Settings::Instances();
    queue<string> *list = settings.getTSXfileList();

	//create texture from the path
	SDL_Color color = {0,0,0,255};
    int all = list->size();
    while(list->size()>0){
        string imgPath = list->front();
        t.loadTextAsTexturesIntoMap(imgPath, imgPath, 20, color);
        loadingPage(all-list->size(), imgPath,all+1);
        list->pop();
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

	C_Time& time=C_Time::Instances();
    C_Grid& grid=C_Grid::Instances();
	C_Menu& menu=C_Menu::Instances();
	grid.displayStatus();

    //load the first level
    m_level->load(m_levelNbr);
    while(!m_quit)
    {

        listenSDL_Events();

		if (time.testNewFrame()){

				m_forceRefresh = true;
				time.updateFrameTime();
				//play all units
				grid.playAllUnits();

            //render image
	        if (m_forceRefresh){

		        //display game content from bottom to top
                m_landscape->render();
		        grid.renderLayer (GRAVEYARD);
		        grid.renderLayer (GROUND);
		        grid.renderLayer (FIELD);

                listenButtons();
                menu.updateInfos(m_level);
        		menu.render();
        		time.showFPS ();
        		//print the final render
                SDL_RenderPresent(m_renderer);
 		        }

	    }
        // pause the game loop in order to respect FPS settings
	    time.delayGameLoop();


    }

}

void C_Window::listenSDL_Events(){

    SDL_Event event;
    unsigned int windowID = SDL_GetWindowID(m_window);

	    while (SDL_PollEvent(&event))
	    {
		    switch (event.type)
		    {
		     case SDL_QUIT:
			    m_quit = true;
			    break;
		     case SDL_WINDOWEVENT:
		     	if (event.window.windowID == windowID && event.window.event == SDL_WINDOWEVENT_EXPOSED)
		     		{ m_forceRefresh = true;
		     		break;
		     		}
		     	break;
		    case SDL_MOUSEMOTION:
                listenMouseMotion(event);
			    break;

		    case SDL_MOUSEBUTTONDOWN:
			    if (event.button.button ==  SDL_BUTTON_LEFT)
				    {
				    if (m_addingAnewTower || m_aTowerIsSelected){
					    m_mouseButtonDown = true;
					    }
				    }
			    break;
		    case SDL_MOUSEBUTTONUP:
		        listenMouseButtonUP(event);
                break;
		    case SDL_KEYDOWN:
		        listenKeyboard(event);
		    }
    }
}

void C_Window::listenButtons(){
    	C_Menu& menu=C_Menu::Instances();

		C_MenuItem* menuButton;
        string list[3] = {"AddTower","AddTurbine","AddBarricade"};
		for (int i = 0; i < 3; i++){
			menuButton = menu.getMenuItem(list[i]);
			int xl = menuButton->getXScreen();
			int xr = xl + menuButton->getWidth();
			int yt= menuButton->getYScreen();
			int yb = yt + menuButton->getHeight();
            //reset state
	 		menuButton->setState(ACTIVE);
            string name = menuButton->getName();
			if (m_clic.x > xl && m_clic.x < xr && m_clic.y > yt && m_clic.y < yb){
					if(name == "AddTower"){
						m_archerTower->drag(m_cursor,false);
						m_addingAnewTower = true;
						m_buttonType = name;
						}
					else if(name == "AddTurbine"){
						m_turbineTower->drag(m_cursor,false);
						m_addingAnewTower = true;
						m_buttonType = name;
						}
					else if(name == "AddBarricade"){
						m_barricade->drag(m_cursor,true);
						m_addingAnewTower = true;
						m_buttonType = name;
						}
	 		}
	 		//mouse Over
	 		if (m_cursor.x > xl && m_cursor.x < xr && m_cursor.y > yt && m_cursor.y < yb){
                    if(menuButton->getName() == "AddTower" || menuButton->getName() == "AddTurbine" || menuButton->getName() == "AddBarricade"){
                        menuButton->setState(HOVER);
						}
	 		}
 		}

}

void C_Window::listenKeyboard(SDL_Event &event){
    C_Settings& settings=C_Settings::Instances();
    C_Menu& menu=C_Menu::Instances();
    switch(event.key.keysym.sym)
			    {
			    case SDLK_d:
				    settings.setDebugMode();
				    settings.displayDebugMode();
				    break;
			    case SDLK_l:
			        m_levelNbr = settings.setCurrentLevelNbr(m_levelNbr +1);
                    delete m_level;
                    m_level = new C_Level;
                	m_level->load(m_levelNbr);
                	menu.resetValues();
				    break;
			    case SDLK_n:
				    m_level->sendNextWave();
				    break;
			    case SDLK_p:
			        if(settings.getDebugMode() == true){
				        settings.setDebugPathMode();
				        settings.displayDebugMode();
				    }
				    break;
			    case SDLK_q:
				    m_quit = true;
				    break;
			    case SDLK_r:
                	m_level->load(m_levelNbr);
                	menu.resetValues();
				    break;
			    }
}

void C_Window::listenMouseMotion(SDL_Event &event){
        C_Settings& settings=C_Settings::Instances();
        C_Grid& grid=C_Grid::Instances();
		    // get x cursor position
			    if(event.button.x < 0)
				    m_cursor.x = 0;
			    else if(event.button.x > settings.getWindowWidth())
				    m_cursor.x = settings.getWindowWidth();
			    else
				    m_cursor.x = event.button.x;

		    // get y cursor position
			    if(event.button.y < 0)
				    m_cursor.y = 0;
			    else if(event.button.y > settings.getWindowHeight())
				    m_cursor.y = settings.getWindowHeight();
			    else
				    m_cursor.y = event.button.y;

			    if (m_mouseButtonDown){
					    if(m_aTowerIsSelected){
						    grid.getSelectedUnit();
					    }
			    }
        }

void C_Window::listenMouseButtonUP(SDL_Event &event){
        C_Grid& grid=C_Grid::Instances();
			    if (event.button.button ==  SDL_BUTTON_LEFT)
				    {
					    m_clic.x = event.button.x;
					    m_clic.y = event.button.y;
					    C_CoordScreen clicleft(m_clic);

					    //Select a Tower
					    if(m_addingAnewTower == false) {
						    m_aTowerIsSelected = grid.selectATower(clicleft);
					    }

					    //Add a new Tower
					    if(m_addingAnewTower == true){
						    grid.addUnit(m_buttonType,clicleft.getXGrid (),clicleft.getYGrid (),0);
						    m_aTowerIsSelected = grid.selectATower(clicleft);
						    m_addingAnewTower = false;
						}
					    else{
						    m_addingAnewTower = false;
					    }

				    m_mouseButtonDown = false;
				    }
}
