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

#include <SDL2_gfxPrimitives.h>

#include "landscape.h"
#include "grid.h"
#include "../settings.h"
#include "../texture.h"

using namespace std;



C_Landscape::C_Landscape(S_Coord town)
{
    m_waterDrift.x = 0;
    m_waterDrift.y = 0;
    m_animWater= new C_AnimTime();
    m_waterDirection = waterDirection(town);
}

C_Landscape::~C_Landscape()
{
    delete m_animWater;
}


void C_Landscape::render(){
        renderWater(m_waterDirection);
};

void C_Landscape::renderWater(int direction){
	    C_Window& win=C_Window::Instances();
	    SDL_Renderer* renderer = win.getRenderer ();

		C_TextureList& t=C_TextureList::Instances();
        //cout << "direction: " << direction << endl;
		SDL_SetRenderDrawColor(renderer, 26, 60, 108, 255);	//fill with background color
		SDL_RenderClear(renderer);
		for(int j = -3; j < 21; j++){
		    int h = 2*TILE_HALF_WIDTH;
		    if (j%2 ==0)
		        h = 0;

		    int y = j*TILE_HALF_WIDTH;
		    for(int i = -3; i < 11; i++){
		        int x = i*4*TILE_HALF_WIDTH + h;
        	    t.renderTexture("Water_00_Blue_EE_0", x + m_waterDrift.x ,y + m_waterDrift.y);
        	    t.renderTexture("Water_00_White_EE_0", x + m_waterDrift.x*(-1) ,y + m_waterDrift.y*(-1));
        	}
        }
		    int pause = m_animWater->getAnimNbr(1,4,FRAMERATE);

            int limit = 4*TILE_HALF_WIDTH - 1;

		    if(pause%2 == 0){
		        switch (direction) {
		            case WEST:
		                 m_waterDrift.x += 1;
                          m_waterDrift.y += 1;
		            break;
		            case EAST:
		                 m_waterDrift.x -= 1;
                         m_waterDrift.y -= 1;
		            break;
		            case NORTH:
		                 m_waterDrift.x -= 1;
                          m_waterDrift.y += 1;
		            break;
		            case SOUTH:
		                 m_waterDrift.x += 1;
                         m_waterDrift.y -= 1;
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



int C_Landscape::waterDirection(S_Coord town){

        	C_Settings& settings=C_Settings::Instances();

		    int ab = town.x - settings.getWindowWidth()/2;
		    int bc = town.y - settings.getWindowHeight()/2;;
		    double angle = atan2(ab,bc);
		    angle = 180 - (angle *180/3.14159265359);
			if(angle < 0)
				angle +=360;

            C_Coord tmp(0,0);
		    int direction = tmp.angleToDirection(angle);
		    return direction;
}

//---------------------------Decors-------------------------

C_Decors::C_Decors(string name, int x_grid, int y_grid):
	C_GameUnits(name, x_grid, y_grid, 0)
{
}

void C_Decors::render(S_Coord screen){
	m_coord->regenScreenCoord();
	string fileName = m_name;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE);
}

//---------------------------C_Ground -------------------------
C_Ground::C_Ground(string name, int x_grid, int y_grid):
	C_GameUnits(name, x_grid, y_grid,0)
{
}

void C_Ground::render(){
    m_coord->regenScreenCoord();
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(m_name, m_coord->getXScreen(),m_coord->getYScreen(),CENTER_TILE);

	    size_t found = m_name.find("Water");
	    if(found == string::npos){
	        if ((m_coord->getXGrid()+m_coord->getYGrid())%2 == 0){
                darkenGround(m_coord->getXScreen(), m_coord->getYScreen() + TILE_HALF_HEIGHT);
            }
	    }

}

void C_Ground::darkenGround(int x_screen, int y_screen){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	Sint16 w =  TILE_HALF_WIDTH;
	Sint16 h =  w/2;
	Sint16 x1 = x_screen - w;

	Sint16 y1 = y_screen - h -2; //center
	Sint16 x2 = x1 + w;
	Sint16 y2 = y1 + h;
	Sint16 x3 = x1 + (w*2);
	Sint16 y3 = y1;
	Sint16 x4 = x2;
	Sint16 y4 = y1 - h;
	Sint16 vx[] = {x1,x2,x3,x4};
	Sint16 vy[] = {y1,y2,y3,y4};
	int R = 30, G = 30, B = 30, A = 50;

	filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);
}

//---------------------------C_Trees-------------------------
C_Trees::C_Trees(string name, int x_grid, int y_grid):
	C_Decors(name, x_grid, y_grid)
{
}

void C_Trees::render(S_Coord screen){
	m_coord->regenScreenCoord();
	int	imageNbr = m_animation[MAIN_ANIM]->getLoopAnimNbr(0,5,200);
	string fileName = "trees_01_0" + to_string(imageNbr);
	//cout << "image name is "<< fileName << endl;
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE);
}




