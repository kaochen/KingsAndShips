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
#include "tools.h"
#include "settings.h"
#include "locator.h"
#include "textureList.h"
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

C_Window::C_Window()
{
	initSDL();
	C_Settings& settings= C_Locator::getSettings();
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
	C_Message::printM("Constructor C_Window() : done\n");
}

C_Window::~C_Window()
{
	delete m_level;
	delete m_levelFactory;
}


void C_Window::initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0) {
		C_Message::printSDLerror("SDL_Init() failed");
		exit (EXIT_FAILURE);
	} else {
		C_Message::printM("SDL_Init() succeed\n");
	}

	if(TTF_Init() < 0) {
		C_Message::printTTFerror("TTF_init() failed");
		exit (EXIT_FAILURE);
	} else {
		C_Message::printM("TTF_Init() succeed\n");
	}


	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		C_Message::printSDLerror("IMG_Init");
		//SDL_Quit;
	} else {
		C_Message::printM("IMG_Init succeed\n");
	}
}

void C_Window::createWindow()
{
	C_Settings& settings= C_Locator::getSettings();
	m_window = SDL_CreateWindow("KingsAndShips",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								settings.getWindowWidth(),
								settings.getWindowHeight(),
								SDL_WINDOW_MOUSE_FOCUS);

	if (m_window == nullptr) {
		C_Message::printSDLerror("SDL_CreateWindow() failed");
		SDL_Quit();
	} else {
		// Create a renderer from the window
		m_renderer = SDL_CreateRenderer(m_window, -1,SDL_RENDERER_ACCELERATED);
		if (m_renderer == nullptr) {
			SDL_DestroyWindow(m_window);
			C_Message::printSDLerror("SDL_GetWindowSurface() failed");
			SDL_Quit();
		}
	}
	C_Message::printM("The main window has been created successfully\n");
}

void C_Window::loadGame()
{
	SDL_Color color = {0,0,0,255};
	C_TextureList& t= C_Locator::getTextureList();
	string title = "Kings and Ships";
	t.loadTextAsTexturesIntoMap("title", title, 100, color);

	C_Settings& settings= C_Locator::getSettings();
	queue<string> *list = settings.getTSXfileList();

	//create texture from the path
	int all = list->size();
	while(list->size()>0) {
		string imgPath = list->front();
		string label = C_Tools::extractFilename(imgPath);
		//render
		t.loadTextAsTexturesIntoMap(label, label, FONT_BIG, color);
		loadingPage(all-list->size(), label,all+1);
		t.extractTSXfile(imgPath); //extract;
		list->pop(); //go next
	}
	t.displayTexturesList();
}

void C_Window::listLevels()
{
	C_Message::printM("Start listing levels\n");
	if(m_levelFactory !=nullptr) {
		delete m_levelFactory;
	}
	m_levelFactory = new C_LevelFactory;
	C_Message::printM("Listing levels completed\n");
	//Now level factory is init, it is possible to generate the menuBanner
	C_Menu& menu=C_Locator::getMenu();
	menu.menuBanner();
}

void C_Window::loadingPage(int progress, string label, int stepsNbr)
{
	SDL_SetRenderDrawColor(m_renderer, 64, 64, 64, 255);
	SDL_RenderClear(m_renderer);
	renderProgressBar(progress, label, stepsNbr);
	C_Settings& settings= C_Locator::getSettings();
	C_TextureList& t= C_Locator::getTextureList();
	t.renderText("title",settings.getWindowWidth()/2, settings.getWindowHeight()/3,CENTER);
	SDL_RenderPresent(m_renderer);
}



void C_Window::renderProgressBar(int progress, string label, int stepsNbr)
{
	C_Settings& settings= C_Locator::getSettings();
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
	t.renderText(label,settings.getWindowWidth()/2, settings.getWindowHeight()/2 + 2*height,CENTER);
}
void C_Window::quit(){
	m_quit = true;
}

void C_Window::quitProgram()
{
	//Cleanup before leaving
	C_Grid& grid= C_Locator::getGrid();
	grid.deleteGrid();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	TTF_Quit();
	SDL_Quit();
	C_Message::printM("Bye\n");
}


void C_Window::gameLoop()
{
	C_Time& time = C_Locator::getTime();

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
                if(m_level->endOfALevel()){
                    openMenu();
    		    C_Message::printM("End of Level Open the menu --------------------------------\n");
                }
            	C_Menu& menu = C_Locator::getMenu();
				menu.refresh();
				//display game content from bottom to top
				m_level->render();
				m_level->renderSelected();
				listenButtons();
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
			break;
		case SDL_MOUSEWHEEL:
			listenMouseWheel(event);
			break;
		}
		navigateOverTheMap(button);
	}
}
bool C_Window::testIfButton(S_Coord screen){
	C_Menu& menu=C_Locator::getMenu();
    bool ret = false;
    std::vector <C_MenuItem*> buttonList = menu.getMenuItems();
    if(!buttonList.empty()){
        for(auto i : buttonList){
            if(i != nullptr){
            	int type = i->getType();
			        if(type != STATUS) {
				        int xl = i->getXScreen();
				        int xr = xl + i->getWidth();
				        int yt= i->getYScreen();
				        int yb = yt + i->getHeight();
				        string name = i->getName();
				        if (screen.x > xl && screen.x < xr && screen.y > yt && screen.y < yb) {
				            ret = true;
				        }
			    }

            }

        }
    }
    return ret;
}

void C_Window::listenButtons()
{
	C_Menu& menu=C_Locator::getMenu();
    std::vector <C_MenuItem*> buttonList = menu.getMenuItems();
    if(!buttonList.empty()){
        for(auto i : buttonList){
            if(i != nullptr){
            	int type = i->getType();
			    if(type != STATUS) {
				    int xl = i->getXScreen();
				    int xr = xl + i->getWidth();
				    int yt= i->getYScreen();
				    int yb = yt + i->getHeight();
				    string name = i->getName();
				    if (m_clic.x > xl && m_clic.x < xr && m_clic.y > yt && m_clic.y < yb) {
					    if(i->getEnable()== true) {
						    if( type == DRAGUNIT) {
							    m_level->unselectedAll();
							    m_aTowerIsSelected = false;

							    i->drag(m_cursor);
							    m_addingAnewTower = true;
						    }
						    if( type == ACTION) {
							    i->action();
							    m_clic.x = m_clic.y = 0;
							    if(i->getName() == "popOutMenu" ){
								    m_level->unselectedAll();
								    m_aTowerIsSelected = false;
							    }
						    }
						    m_buttonType = name;
					    } else {
						    if(i->getType() == DRAGUNIT) {
							    m_addingAnewTower = false;
						    }
						    m_buttonType = "";
						    m_clic.x = m_clic.y = 0;
					    }
				    }
				    //mouse Over
				    if (m_cursor.x > xl && m_cursor.x < xr && m_cursor.y > yt && m_cursor.y < yb) {
					    i->actionHover(true);
				    } else {
					   i->actionHover(false);
				    }
			    }
            }
        }
    }
}

void C_Window::listenKeyboard(SDL_Event &event)
{
	C_Settings& settings= C_Locator::getSettings();
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
		}
		break;
	case SDLK_d:
		settings.setDebugMode();
		settings.displayDebugMode();
		break;
	case SDLK_l:
		loadLevel(m_levelNbr +1);
		break;
	case SDLK_m:
        openMenu();
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
	case SDLK_s:
		settings.zoomDown();
		break;
	case SDLK_z:
		settings.zoomUp();
		break;
	case SDLK_w:{
	    if(m_level != nullptr){
            m_level->setStatus(WIN);
            }
		}
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
	C_Settings& settings= C_Locator::getSettings();
	m_levelNbr = settings.setCurrentLevelNbr(levelNbr);

	if(m_level != nullptr) {
		delete m_level;
	}
	m_level = m_levelFactory->create(levelNbr);
	if(m_level != nullptr) {
		if(m_level->load(m_levelNbr)){
		    settings.setPlaying();
    		settings.cameraOnAPoint(m_level->getGridTown());
		} else {
    		C_Message::printM("Can not create level" + to_string(m_levelNbr));
    		quitProgram();
		}
	} else {
		C_Message::printM("Can not create level" + to_string(m_levelNbr));
		quitProgram();
	}
}


void C_Window::listenMouseMotion(SDL_Event &event)
{
	C_Settings& settings= C_Locator::getSettings();
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
		    m_aTowerIsSelected = m_level->selectATower(m_clic);
		    if(m_addingAnewTower == true) {
			    m_level->addUnit(m_buttonType, m_clic);
			    m_addingAnewTower = false;
    			m_level->unselectedAll();
		    }


		}
		m_dragAndDropTower = false;
		m_mouseDragWindow = false;
	} else if (event.button.button ==  SDL_BUTTON_RIGHT) {
		C_OpenMenu openMenu;
		if (!openMenu.getBool()) {
			m_level->unselectedAll();
			m_aTowerIsSelected = false;
		}

		m_addingAnewTower = false;
		m_buttonType = "";
		m_clic.x = m_clic.y = 0;
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
		C_Settings& settings= C_Locator::getSettings();
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

S_LevelData C_Window::getLevelModel(int levelNbr)
{
	return m_levelFactory->getModel(levelNbr);
}

int C_Window::getLastLevelNbr()
{
	return m_levelFactory->getLastLevelNbr();
}

void C_Window::listenMouseWheel(SDL_Event &event){
	if(event.wheel.y != 0){
		C_Settings& settings= C_Locator::getSettings();
		if(event.wheel.y > 0){
			settings.zoomUp();
		} else if(event.wheel.y < 0){
			settings.zoomDown();
		}
    }
}

void C_Window::openMenu(){
    C_Menu& menu = C_Locator::getMenu();
    if(!menu.isOpen()){
	    C_OpenMenu openMenu;
	    openMenu.action();
	    if(m_level != nullptr){
	        m_level->unselectedAll();
	    }
	    m_aTowerIsSelected = false;
	}
}


bool C_Window::currentLevelIsOver(){
    bool ret = false;
    if(m_level != nullptr){
	    S_LevelData data = m_level->getData();
	    if(data.status == WIN || data.status == LOSE ){
	        return true;
	    }
	}
    return ret;
}
