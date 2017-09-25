#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


void initSDL();

void createWindow(SDL_Window* window, SDL_Renderer* renderer);
void quitProgram(SDL_Window* window, SDL_Renderer* renderer);
void logSDLerror(const std::string &msg);
#endif
