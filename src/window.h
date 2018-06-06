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


#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>
#include "coord.h"
#include "level/towers.h"

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
  void loadGame();
  void gameLoop();
  void quitProgram();
  void listenButtons(S_Coord clic, S_Coord cursor,
                      int &buttonType, bool &addingAnewTower,
                     C_Towers* archerTower, C_Towers* turbineTower);

	protected:
	void initSDL();
  void loadingPage(int progress, std::string label, int stepsNbr);
  void renderProgressBar(int progress, std::string label, int stepsNbr);

	private:
	C_Window& operator= (const C_Window&){return *this;}
	C_Window (const C_Window&){}

	static C_Window m_instance;
	C_Window();
	~C_Window();

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};
#endif
