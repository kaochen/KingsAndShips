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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "level/factory.h"
#include "level/level.h"
#include "coord.h"

class C_Window
{
public:
	static	C_Window& Instances(){return m_instance;};

	void createWindow();
	SDL_Window* getWindow() {return m_window;};
	SDL_Renderer* getRenderer() {return m_renderer;};

  	void loadGame();
	void gameLoop();
	void quitProgram();
	void listenButtons();
	void loadLevel(int levelNbr);

protected:
	void initSDL();
	void loadingPage(int progress, std::string label, int stepsNbr);
	void renderProgressBar(int progress, std::string label, int stepsNbr);
	void listenSDL_Events();
	void listenKeyboard(SDL_Event &event);
	void listenMouseMotion(SDL_Event &event);
	void listenMouseButtonUP(SDL_Event &event);
	void listenMouseButtonDown(SDL_Event &event);
	void navigateOverTheMap(S_Coord const &button);

private:
	C_Window& operator= (const C_Window&){return *this;}
	C_Window (const C_Window&){}

	static C_Window m_instance;
	C_Window();
	~C_Window();

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	bool m_forceRefresh;
	//C_LevelFactory * m_levelFactory;
  	C_Level* m_level;
	int m_levelNbr;

	std::string m_buttonType;
	S_Coord m_cursor;
	S_Coord m_clic;
	S_Coord m_dragLeft;
	bool m_mouseButtonDown;
	bool m_mouseDragWindow;

	//towers are needed when drag & drop from the buttons:
	bool m_dragAndDropTower;
	bool m_addingAnewTower;
	bool m_aTowerIsSelected;

	bool m_quit;
};
#endif
