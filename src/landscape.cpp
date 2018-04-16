#include "landscape.h"
#include "settings.h"

using namespace std;



C_Landscape::C_Landscape()
{
    m_waterDrift.x = 0;
    m_waterDrift.y = 0;
    m_animWater= new C_AnimTime();
    m_waterDirection = waterDirection();
}

C_Landscape::~C_Landscape()
{
}


void C_Landscape::display(){
	    C_Grid& grid=C_Grid::Instances();
        renderWater(waterDirection());
		grid.renderLayer (GROUND);
};

void C_Landscape::renderWater(int direction){
	    C_Window& win=C_Window::Instances();
	    SDL_Renderer* renderer = win.getRenderer ();

		C_TextureList& t=C_TextureList::Instances();
        //cout << "direction: " << direction << endl;
		SDL_SetRenderDrawColor(renderer, 26, 60, 108, 255);	//fill with background color
		SDL_RenderClear(renderer);
		for(int j = -1; j < 20; j++){
		    int h = 2*TILE_HALF_WIDTH;
		    if (j%2 ==0)
		        h = 0;

		    int y = j*TILE_HALF_WIDTH;
		    for(int i = -1; i < 10; i++){
		        int x = i*4*TILE_HALF_WIDTH + h;
        	    t.renderTexture("Water_00_Blue_EE_0", x + m_waterDrift.x ,y + m_waterDrift.y);
        	    //t.renderTexture("Water_00_White_EE_0", x + m_waterDrift.x -10 ,y + m_waterDrift.y -10);
        	}
        }
		    int pause = m_animWater->getAnimNbr(1,4,FRAMERATE);
            int halfPixel = 1;
            int limit = TILE_HALF_WIDTH -1;
	        if(pause == 4){
    			   	 halfPixel =0;
    			   	 }
		    if(pause%2 == 0){
		        switch (direction) {
		            case WEST:
		                 m_waterDrift.x += 1;
                          m_waterDrift.y += halfPixel;
		            break;
		            case EAST:
		                 m_waterDrift.x -= 1;
                         m_waterDrift.y -= halfPixel;
		            break;
		            case NORTH:
		                 m_waterDrift.x -= 1;
                          m_waterDrift.y += halfPixel;
		            break;
		            case SOUTH:
		                 m_waterDrift.x += 1;
                         m_waterDrift.y -= halfPixel;
		            break;
		            case SOUTH_WEST:
		                 m_waterDrift.x += 1;
		                 limit = 4*TILE_HALF_WIDTH -1;
		                 if(m_waterDrift.x > limit || m_waterDrift.x < -limit){
                                m_waterDrift.x = 0;
                                m_waterDrift.y = 0;
                                }
  		            break;
  		            case SOUTH_EAST:
		                 m_waterDrift.y -= 1;
		                 limit = 2*TILE_HALF_WIDTH -1;
  		            break;
  		            case NORTH_WEST:
		                 m_waterDrift.y += 1;
		                 limit = 2*TILE_HALF_WIDTH -1;
  		            break;
  		            case NORTH_EAST:
		                 m_waterDrift.x -= 1;
		                 limit = 4*TILE_HALF_WIDTH -1;
                            if(m_waterDrift.x > limit || m_waterDrift.x < -limit){
                                m_waterDrift.x = 0;
                                m_waterDrift.y = 0;
                                }
  		             break;

		        }

                if(m_waterDrift.y > limit || m_waterDrift.y < -limit){
                    m_waterDrift.x = 0;
                    m_waterDrift.y = 0;
                    }
            }
}



int C_Landscape::waterDirection(){

        	C_Set& settings=C_Set::Instances();
        	C_Grid& grid=C_Grid::Instances();

            S_Coord center;
            center.x = settings.getWindowWidth()/2;
            center.y = settings.getWindowHeight()/2;

		    C_CoordGrid tmp(grid.foundTown());
		    S_Coord town = tmp.getScreen();
		    //cout << "center: " << center.x << ":" << center.y << "Town: " << town.x <<":" << town.y;
		    int ab = town.x - center.x;
		    int bc = town.y - center.y;
		    double angle = atan2(ab,bc);
		    angle = 180 - (angle *180/3.14159265359);
			if(angle < 0)
				angle +=360;

		    int direction = tmp.angleToDirection(angle);
		    //cout << "angle" << angle;
		    return direction;
}

//---------------------------Decors-------------------------

C_Decors::C_Decors(string name, int x_grid, int y_grid):
	C_GameUnits(name, x_grid, y_grid, 1)
{
}
