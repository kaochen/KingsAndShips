#include "window.h"
#include "settings.h"

#include "surfaces.h"

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

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) == -1){
			logSDLerror("SDL_Init() failed");
			exit (EXIT_FAILURE);
		}

	if(TTF_Init() == -1){
			cout << "TTF_init() failed: " << TTF_GetError() << endl;
			exit (EXIT_FAILURE);
		}

	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLerror("IMG_Init");
		//SDL_Quit;
		}
	SDL_ClearError();
}

void C_Window::createWindow(){
 	initSDL();

	C_Set& settings=C_Set::Instances();
 	m_window = SDL_CreateWindow("TOWER",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      settings.getWindowWidth(),
			      settings.getWindowHeight(),
			      SDL_WINDOW_MOUSE_FOCUS);

	if (m_window == nullptr)
		{
			logSDLerror("SDL_CreateWindow() failed");
			SDL_Quit();
		}
	else
		{
			// Create a renderer from the window
			m_renderer = SDL_CreateRenderer(m_window, -1,SDL_RENDERER_ACCELERATED);
			if (m_renderer == nullptr)
				{
					SDL_DestroyWindow(m_window);
					logSDLerror("SDL_GetWindowSurface() failed");
					SDL_Quit();
				}
		}
cout << "The main window has been created successfully" << endl;
}



SDL_Window* C_Window::getWindow(){
	return m_window;
};

SDL_Renderer* C_Window::getRenderer(){
	return m_renderer;
};

void C_Window::loadGame(){
	C_TextureList& t=C_TextureList::Instances();
	//C_Texture text;
	int size = 7;
	string tsxList[size];
	tsxList[0] = "data/levels/boat_01.tsx";
	tsxList[1] = "data/levels/Ground_01.tsx";
	tsxList[2] = "data/levels/smoke_01.tsx";
	tsxList[3] = "data/levels/turbine_00.tsx";
	tsxList[4] = "data/levels/archerTower_00.tsx";
	tsxList[5] = "data/levels/archerTower_01.tsx";
	tsxList[6] = "data/levels/buttons.tsx";

	t.loadTexturesIntoMap();
    int progress = 100/(size+1);
    loadingPage(progress);
    for(int i = 0; i < size; i++){
	    t.extractTSXfile(tsxList[i]);
	    progress += 100/(size+1);
        loadingPage(progress);
    }
	t.displayTexturesList();
}

void C_Window::loadingPage(int progress){
    SDL_SetRenderDrawColor(m_renderer, 64, 64, 64, 255);
	SDL_RenderClear(m_renderer);

	renderProgressBar(progress);
	SDL_RenderPresent(m_renderer);
	}



void C_Window::renderProgressBar(int progress)
	{
		C_Set& settings=C_Set::Instances();
		int height = 40, width = settings.getWindowWidth()/2;
		int x_screen = 	(settings.getWindowWidth()- width) /2;
		int y_screen =  (settings.getWindowHeight() - height)/2;
		//add a life status above the boat

		SDL_Rect back, fill;
		    back.x = x_screen;
		    back.y = y_screen;
		    back.w = width;
		    back.h = height;

		    fill.x = back.x + 1;
		    fill.y = back.y + 1;
		    fill.w = width*progress/100;
		    fill.h = back.h - 2;

		    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255 );
		    SDL_RenderFillRect(m_renderer, &back);
		    SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255 );
		    SDL_RenderFillRect(m_renderer, &fill);
	}



void quitProgram(SDL_Window* window, SDL_Renderer* renderer)
{
	//Cleanup before leaving
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	cout << "Bye" << endl;
}


void logSDLerror(const string &msg)
{
	cout << msg << " error: " << SDL_GetError() << endl;
}

