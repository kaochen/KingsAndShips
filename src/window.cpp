#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;


void initSDL()
{

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) == -1)
		{
			cout << "SDL_init() failed: " << SDL_GetError() << endl;
			exit (EXIT_FAILURE);
		}

	if(TTF_Init() == -1)
		{
			cout << "TTF_init() failed: " << TTF_GetError() << endl;
			exit (EXIT_FAILURE);
		}

}

void createWindow(SDL_Window* window, SDL_Renderer* renderer)
{
 	initSDL();
 	window = SDL_CreateWindow("TOWER",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      800,
			      600,
			      SDL_WINDOW_MOUSE_FOCUS);

	if (window == nullptr)
		{
			cout << "SDL_CreateWindow() failed " << SDL_GetError() << endl;
			SDL_Quit();
		}
	else
		{
			// Create a renderer from the window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr)
				{
					SDL_DestroyWindow(window);
					cout << "SDL_GetWindowSurface() failed " << SDL_GetError() << endl;
					SDL_Quit();
				}

		}
cout << "The main window has been created successfully" << endl;
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
