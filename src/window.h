#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void initSDL();

void createWindow(SDL_Window* window, SDL_Renderer* renderer);
void quitProgram(SDL_Window* window, SDL_Renderer* renderer);
#endif
