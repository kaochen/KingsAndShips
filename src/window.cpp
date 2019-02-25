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
#include "locator.h"
#include "texture.h"
#include "menu/menu.h"
#include "menu/command.h"
#include "message.h"
#include "wallet.h"
#include "level/grid.h"
#include "level/gameUnits.h"
#include "level/towers.h"
#include "level/barricade.h"

#include <queue>

using namespace std;
C_Window C_Window::m_instance=C_Window();

C_Window::C_Window()
{
	initSDL();
	C_Settings& settings=C_Settings::Instances();
	m_forceRefresh = false;

	m_levelFactory = nullptr;
	m_level = nullptr;
	m_levelNbr = settings.getCurrentLevelNbr();



	m_buttonType = "";
	m_cursor.x = m_cursor.y = 1;
	m_clic.x = m_clic.y = 0;
	m_dragLeft = {0,0};
	m_mouseButtonDown = false;
	m_mouseDragWindow = false;

	m_dragAndDropTower = false;
	m_addingAnewTower = false;
	m_aTowerIsSelected = false;

	m_quit = false;
}

C_Window::~C_Window()
{
	delete m_level;
	delete m_levelFactory;
}


void C_Window::initSDL()
{
	C_Message m;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0) {
		m.printSDLerror("SDL_Init() failed");
		exit (EXIT_FAILURE);
	} else {
		m.printM("SDL_Init() succeed\n");
	}

	if(TTF_Init() < 0) {
		m.printTTFerror("TTF_init() failed");
		exit (EXIT_FAILURE);
	} else {
		m.printM("TTF_Init() succeed\n");
	}


	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		m.printSDLerror("IMG_Init");
		//SDL_Quit;
	} else {
		m.printM("IMG_Init succeed\n");
	}
}

void C_Window::createWindow()
{
	C_Message m;
	C_Settings& settings=C_Settings::Instances();
	m_window = SDL_CreateWindow("KingsAndShips",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								settings.getWindowWidth(),
								settings.getWindowHeight(),
								SDL_WINDOW_MOUSE_FOCUS);

	if (m_window == nullptr) {
		m.printSDLerror("SDL_CreateWindow() failed");
		SDL_Quit();
	} else {
		// Create a renderer from the window
		m_renderer = SDL_CreateRenderer(m_window, -1,SDL_RENDERER_ACCELERATED);
		if (m_renderer == nullptr) {
			SDL_DestroyWindow(m_window);
			m.printSDLerror("SDL_GetWindowSurface() failed");
			SDL_Quit();
		}
	}
	m.printM("The main window has been created successfully\n");
}

void C_Window::loadGame()
{
	C_TextureList& t= C_Locator::getTextureList();
	C_Settings& settings=C_Settings::Instances();
	queue<string> *list = settings.getTSXfileList();

	//create texture from the path
	SDL_Color color = {0,0,0,255};
	int all = list->size();
	while(list->size()>0) {
		string imgPath = list->front();
		t.loadTextAsTexturesIntoMap(imgPath, imgPath, 20, color);
		loadingPage(all-list->size(), imgPath,all+1);
		list->pop();
		t.extractTSXfile(imgPath);
	}
	t.displayTexturesList();
}

void C_Window::listLevels()
{
	C_Message m;
	m.printM("Start listing levels\n");
	if(m_levelFactory !=nullptr) {
		delete m_levelFactory;
	}
	m_levelFactory = new C_LevelFactory;
	m.printM("Listing levels completed\n");
	//Now level factory is init, it is possible to generate the menuBanner
	C_Menu& menu=C_Locator::getMenu();
	menu.menuBanner();
}

void C_Window::loadingPage(int progress, string label, int stepsNbr)
{
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
	for(int i = 1; i < stepsNbr; i++) {
		stripes.x = x_screen + i*step;
		SDL_RenderFillRect(m_renderer, &stripes);
	}

	C_TextureList& t= C_Locator::getTextureList();
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


void C_Window::gameLoop()
{
	C_Time& time = C_Locator::getTime();
	C_Menu& menu = C_Locator::getMenu();

	//load the first level
	loadLevel(m_levelNbr);
	while(!m_quit) {

		listenSDL_Events();

		if (time.testNewFrame()) {
			m_forceRefresh = true;
			time.updateFrameTime();
			//play all units

			m_level->play();
			//render image
			if (m_forceRefresh) {

				//display game content from bottom to top
				m_level->render();
				m_level->renderSelected();
				listenButtons();
				menu.updateInfos();
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

void C_Window::listenSDL_Events()
{

	SDL_Event event;
	unsigned int windowID = SDL_GetWindowID(m_window);
	while (SDL_PollEvent(&event)) {
		S_Coord button = {event.button.x, event.button.y};
		switch (event.type) {
		case SDL_QUIT:
			m_quit = true;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.windowID == windowID && event.window.event == SDL_WINDOWEVENT_EXPOSED) {
				m_forceRefresh = true;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			listenMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			listenMouseButtonDown(event);
			break;
		case SDL_MOUSEBUTTONUP:
			listenMouseButtonUP(event);
			break;
		case SDL_KEYDOWN:
			listenKeyboard(event);
		}
		navigateOverTheMap(button);
	}
}
bool C_Window::testIfButton(S_Coord screen){
	C_Menu& menu=C_Locator::getMenu();
    bool ret = false;
	C_MenuItem* menuButton;
	vector <string> list;
	list = menu.getMenuItemsList();
	for (size_t i = 0; i < list.size(); i++) {
		menuButton = menu.getMenuItem(list[i]);
		if(menuButton != nullptr) {
			int type = menuButton->getType();
			if(type != STATUS) {
				int xl = menuButton->getXScreen();
				int xr = xl + menuButton->getWidth();
				int yt= menuButton->getYScreen();
				int yb = yt + menuButton->getHeight();
				string name = menuButton->getName();
				if (screen.x > xl && screen.x < xr && screen.y > yt && screen.y < yb) {
				    ret = true;
				}
			}
        }
    }
    return ret;
}

void C_Window::listenButtons()
{
	C_Menu& menu=C_Locator::getMenu();

	C_MenuItem* menuButton;
	vector <string> list;
	list = menu.getMenuItemsList();
	for (size_t i = 0; i < list.size(); i++) {
		menuButton = menu.getMenuItem(list[i]);
		if(menuButton != nullptr) {
			int type = menuButton->getType();
			if(type != STATUS) {
				int xl = menuButton->getXScreen();
				int xr = xl + menuButton->getWidth();
				int yt= menuButton->getYScreen();
				int yb = yt + menuButton->getHeight();
				string name = menuButton->getName();
				if (m_clic.x > xl && m_clic.x < xr && m_clic.y > yt && m_clic.y < yb) {
					if(menuButton->getEnable()== true) {
						if( type == DRAGUNIT) {
							menuButton->drag(m_cursor);
							m_addingAnewTower = true;
						}
						if( type == ACTION) {
							menuButton->action();
							m_clic.x = m_clic.y = 0;
						}
						m_buttonType = name;
					} else {
						if(menuButton->getType() == DRAGUNIT) {
							m_addingAnewTower = false;
						}
						m_buttonType = "";
						m_clic.x = m_clic.y = 0;
					}
				}
				//mouse Over
				if (m_cursor.x > xl && m_cursor.x < xr && m_cursor.y > yt && m_cursor.y < yb) {
					menuButton->actionHover(true);
				} else {
					menuButton->actionHover(false);
				}
			}
		}
	}
}

void C_Window::listenKeyboard(SDL_Event &event)
{
	C_Settings& settings=C_Settings::Instances();
	int r = 0;
	int l = 0;
	int u = 0;
	int d = 0;
	switch(event.key.keysym.sym) {
	case SDLK_ESCAPE:{
		C_OpenMenu menu;
		if(menu.getBool()){//if menu open ?
			menu.action(); //close it
			}

		m_level->unselectedAll();
		}
		break;
	case SDLK_d:
		settings.setDebugMode();
		settings.displayDebugMode();
		break;
	case SDLK_l:
		loadLevel(m_levelNbr +1);
		break;
	case SDLK_m:{
		C_OpenMenu openMenu;
		openMenu.action();
		}
		break;

	case SDLK_n:
		m_level->sendNextWave();
		break;
	case SDLK_p:
		if(settings.getDebugMode() == true) {
			settings.setDebugPathMode();
			settings.displayDebugMode();
		}
		break;
	case SDLK_q:
		m_quit = true;
		break;
	case SDLK_r:
		loadLevel(m_levelNbr);
		break;
	case SDLK_RIGHT:
		r = 10;
		break;
	case SDLK_LEFT:
		l = 10;
		break;
	case SDLK_DOWN:
		d = 10;
		break;
	case SDLK_UP:
		u = 10;
		break;
	}
	settings.moveCameraPosition(r,l,d,u);
	m_clic.x = m_clic.y = 0;
}

void C_Window::loadLevel(int levelNbr)
{
	C_Settings& settings=C_Settings::Instances();
	m_levelNbr = settings.setCurrentLevelNbr(levelNbr);

	if(m_level != nullptr) {
		delete m_level;
	}
	m_level = m_levelFactory->create(levelNbr);
	if(m_level != nullptr) {
		m_level->load(m_levelNbr);
		settings.cameraOnAPoint(m_level->getGridTown());
	} else {
		C_Message m;
		m.printM("Can not create level" + to_string(m_levelNbr));
	}
}


void C_Window::listenMouseMotion(SDL_Event &event)
{
	C_Settings& settings=C_Settings::Instances();
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

}

void C_Window::listenMouseButtonUP(SDL_Event &event)
{
	if (event.button.button ==  SDL_BUTTON_LEFT) {
		m_clic.x = event.button.x;
		m_clic.y = event.button.y;

	    if(!testIfButton(m_clic)){
		    //Select or add a new Tower
		    if(m_addingAnewTower == true) {
			    m_level->addUnit(m_buttonType, m_clic);
			    m_addingAnewTower = false;
		    }

		    m_aTowerIsSelected = m_level->selectATower(m_clic);

		}
		m_dragAndDropTower = false;
		m_mouseDragWindow = false;
	}
}


void C_Window::listenMouseButtonDown(SDL_Event &event)
{
	if (event.button.button ==  SDL_BUTTON_LEFT) {
		m_mouseButtonDown = true;
		if (m_addingAnewTower || m_aTowerIsSelected) {
			m_dragAndDropTower = true;
			m_mouseDragWindow = false;
		} else {
			m_dragAndDropTower = false;
			C_OpenMenu openMenu;
			if (openMenu.getBool()) {
				m_mouseDragWindow = false;
			} else {
				m_mouseDragWindow = true;
			}
			m_dragLeft.x = event.button.x;
			m_dragLeft.y = event.button.y;
		}
	}
}

void C_Window::navigateOverTheMap(S_Coord const &button)
{
	if(m_mouseDragWindow) {
		C_Settings& settings=C_Settings::Instances();
		if(button.x > 0 && button.x < settings.getWindowWidth() &&
				button.y > 0 && button.y < settings.getWindowHeight()) {
			S_Coord drag;
			drag.x = button.x - m_dragLeft.x;
			drag.y = m_dragLeft.y - button.y; //reverse
			//cout << "button: "<< button.x <<":"<< button.y << " drag: "<< drag.x <<":"<< drag.y << endl;
			settings.moveCameraPosition(drag.x, drag.y);
			m_dragLeft.x = button.x;
			m_dragLeft.y = button.y;
		}
	}
}

S_LevelModel C_Window::getLevelModel(int levelNbr)
{
	return m_levelFactory->getModel(levelNbr);
}
