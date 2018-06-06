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
    C_Message m;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) == -1){
		m.printSDLerror("SDL_Init() failed");
			exit (EXIT_FAILURE);
		}

	if(TTF_Init() == -1){
			m.printTTFerror("TTF_init() failed");
			exit (EXIT_FAILURE);
		}

	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		m.printSDLerror("IMG_Init");
		//SDL_Quit;
		}
}

void C_Window::createWindow(){
 	initSDL();
    C_Message m;
	C_Set& settings=C_Set::Instances();
 	m_window = SDL_CreateWindow("TOWER",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      settings.getWindowWidth(),
			      settings.getWindowHeight(),
			      SDL_WINDOW_MOUSE_FOCUS);

	if (m_window == nullptr)
		{
    		m.printSDLerror("SDL_CreateWindow() failed");
			SDL_Quit();
		}
	else
		{
			// Create a renderer from the window
			m_renderer = SDL_CreateRenderer(m_window, -1,SDL_RENDERER_ACCELERATED);
			if (m_renderer == nullptr)
				{
					SDL_DestroyWindow(m_window);
            		m.printSDLerror("SDL_GetWindowSurface() failed");
					SDL_Quit();
				}
		}
        m.printM("The main window has been created successfully\n");
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
	int size = 12;
	string tsxList[size];
	tsxList[0] = "data/levels/Ground_01.tsx";
	tsxList[1] = "data/levels/boat_01.tsx";
	tsxList[2] = "data/levels/town_01.tsx";
	tsxList[3] = "data/levels/Rocks_00.tsx";
	tsxList[4] = "data/levels/Trees_00.tsx";
	tsxList[5] = "data/levels/turbine_00.tsx";
	tsxList[6] = "data/levels/archerTower_00.tsx";
	tsxList[7] = "data/levels/archerTower_01.tsx";
	tsxList[8] = "data/levels/buttons.tsx";
	tsxList[9] = "data/levels/Water_00.tsx";
	tsxList[10] = "data/levels/smoke_01.tsx";
	tsxList[11] = "data/levels/fox.tsx";
	//create texture from the path
	SDL_Color color = {0,0,0,255};
	string firstImages = "First Images";
	t.loadTextAsTexturesIntoMap(firstImages, firstImages, 20, color);
    for(int i = 0; i < size; i++){
        t.loadTextAsTexturesIntoMap(tsxList[i], tsxList[i], 20, color);
        }

	t.loadTexturesIntoMap();
    loadingPage(1, firstImages, size+1);
    for(int i = 0; i < size; i++){
        loadingPage(i+1, tsxList[i],size+1);
	    t.extractTSXfile(tsxList[i]);
    }
	t.displayTexturesList();
}

void C_Window::loadingPage(int progress, string label, int stepsNbr){
	    SDL_SetRenderDrawColor(m_renderer, 64, 64, 64, 255);
	    SDL_RenderClear(m_renderer);
        renderProgressBar(progress, label, stepsNbr);
	    SDL_RenderPresent(m_renderer);
	}



void C_Window::renderProgressBar(int progress, string label, int stepsNbr)
	{
		C_Set& settings=C_Set::Instances();
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
		    for(int i = 1; i < stepsNbr;i++){
		        stripes.x = x_screen + i*step;
		        SDL_RenderFillRect(m_renderer, &stripes);
		    }

		    C_TextureList& t=C_TextureList::Instances();
		    t.renderTexture(label,settings.getWindowWidth()/2, settings.getWindowHeight()/2 + 2*height);
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



