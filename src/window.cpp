#include "window.h"
#include "settings.h"
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
			m_renderer = SDL_CreateRenderer(m_window, -1,SDL_RENDERER_SOFTWARE);
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
