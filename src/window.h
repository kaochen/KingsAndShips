#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class C_Window
{
	public:
	static	C_Window& Instances();

	void createWindow();
	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
	protected:
	void initSDL();

	private:
	C_Window& operator= (const C_Window&){return *this;}
	C_Window (const C_Window&){}

	static C_Window m_instance;
	C_Window();
	~C_Window();



	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

};


void quitProgram(SDL_Window* window, SDL_Renderer* renderer);
void logSDLerror(const std::string &msg);
#endif
