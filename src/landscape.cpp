#include "landscape.h"

using namespace std;



C_Landscape::C_Landscape()
{
    m_waterDrift = 0;
    m_animWater= new C_AnimTime();
}

C_Landscape::~C_Landscape()
{
}


void C_Landscape::display(){
	    C_Grid& grid=C_Grid::Instances();
        renderWater(WEST);
		grid.renderLayer (GROUND);
};

void C_Landscape::renderWater(int direction){
	    C_Window& win=C_Window::Instances();
	    SDL_Renderer* renderer = win.getRenderer ();

		C_TextureList& t=C_TextureList::Instances();

		SDL_SetRenderDrawColor(renderer, 26, 60, 108, 255);	//fill with background color
		SDL_RenderClear(renderer);
		for(int j = -1; j < 18; j++){
		    int h = 96;
		    if (j%2 ==0)
		        h = 0;

		    int y = j*48;
		    for(int i = -1; i < 9; i++){
		        int x = i*192 + h;
        	    t.renderTexture("Water_00_EE_0", x + 2*m_waterDrift,y + m_waterDrift);
        	}
        }
		    int pause = m_animWater->getAnimNbr(1,2,FRAMERATE);

		    if(pause == 2){
		        if(direction == WEST){
                    m_waterDrift--;
                    }
                else if (direction == EAST){
                    m_waterDrift++;
                }
                if(m_waterDrift > 47 || m_waterDrift < -47){
                    m_waterDrift = 0;
                    }
            }
}
