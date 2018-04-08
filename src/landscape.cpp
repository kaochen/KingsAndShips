#include "landscape.h"

using namespace std;



C_Landscape::C_Landscape()
{
}

C_Landscape::~C_Landscape()
{
}


void C_Landscape::display(){
	    C_Grid& grid=C_Grid::Instances();
        renderWater();
		grid.renderLayer (GROUND);
};

void C_Landscape::renderWater(){
	    C_Window& win=C_Window::Instances();
	    SDL_Renderer* renderer = win.getRenderer ();

		C_TextureList& t=C_TextureList::Instances();

		SDL_SetRenderDrawColor(renderer, 26, 60, 108, 255);	//fill with background color
		SDL_RenderClear(renderer);

		for(int j = 0; j < 17; j++){
		    int h = 96;
		    if (j%2 ==0)
		        h = 0;

		    int y = j*48;
		    for(int i = 0; i < 8; i++){
		        int x = i*192 + h;
        	    t.renderTexture("Water_00_EE_0", x,y);
        	}
        }
}
