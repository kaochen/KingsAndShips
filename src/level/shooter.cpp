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

#include "shooter.h"
#include "grid.h"

#include "../surfaces.h"

using namespace std;

C_Shooter::C_Shooter(std::string name, int x_grid, int y_grid, int rank):
	C_GameUnits(name, x_grid, y_grid, rank)
{
	m_weapon = nullptr;
	m_y_center_offset = 36;
	m_lastShootTime = 0;
}

C_Shooter::~C_Shooter()
{
	delete m_weapon;
}
C_GameUnits*  C_Shooter::searchNextTarget(string type){
	int gridDiag = m_weapon->getFireRange();
	C_Grid& grid=C_Grid::Instances();
	int x_grid = m_coord->getXGrid();
	int y_grid = m_coord->getYGrid();
	C_GameUnits* target = nullptr;

	map<int, C_GameUnits*> list;
	for(int y = (y_grid - gridDiag); y <= (y_grid + gridDiag); y++){
		for(int x = (x_grid - gridDiag); x <= (x_grid + gridDiag); x++){
			if((x != x_grid || y != y_grid)){
			    if(x >= 0 && x <= GRID_SIZE && y >= 0 && y <= GRID_SIZE){
                    //cout << "test: "<< x << ":" << y << endl;
				    C_GameUnits* tmp = grid.getUnits(x,y);
				    if(tmp != nullptr){
					    if(tmp->getName() == type){
					     	int dist = getDistance(x,y);
					     	list[dist] = tmp;
				     	}
				    }
				}
			}
		}
	}
	if(!list.empty()){
		target = list.begin()->second;
	}

	return target;
}
void C_Shooter::shoot(std::string type[MAX_TARGETS], int nbrofTargets)
{
    for(int i = 0; i < nbrofTargets; i++){
	    C_GameUnits* target = searchNextTarget(type[i]);
	    if(target != nullptr){
		    long currentTime = SDL_GetTicks();
		    if ((currentTime ) > m_weapon->getLastShootTime() + m_weapon->getFireRate()){
			    m_weapon->setShooting(true);
			    shootTarget(*target);
			    i = MAX_TARGETS + 1;
			    //cout << target.getName() << " has been shot" << endl;
		    }
		    else {
			    m_weapon->setShooting(false);
			    }
		    }
	    else {
		     m_weapon->setShooting(false);
		    }
	}
}

void C_Shooter::displayStatus() const
{
	C_GameUnits::displayStatus();
	if(m_weapon != nullptr)
 		m_weapon->displayStatus();
}


void C_Shooter::move()
{
}


void C_Shooter::shootTarget(C_GameUnits &target){
	if(m_weapon->getShooting()){
		bool test = m_weapon->shoot(*this, target);
		if (test){
			target.receiveDamage(m_weapon->getWeaponInfo());
		}
	}
}

void C_Shooter::renderLifeBar(int x_screen, int y_screen)
	{
		C_Window& win=C_Window::Instances();
		SDL_Renderer* renderer = win.getRenderer();
		//add a life status above the boat
		int l = m_life / 2;
		int h = 6;
		//life
        int x1_l = x_screen - TILE_HALF_WIDTH/2;
        int y1_l = y_screen - 85;
        int x2_l = x1_l + l;
        int y2_l = y1_l + h;

		int R = 0, G = 200, B = 0;
        if (m_life < MAX_LIFE/2){
		    	R = 200;
		    	G = 0 ;
		    	}

        //background
		int x1_b = x1_l;
		int y1_b = y1_l;
		int x2_b = x1_b + MAX_LIFE/2;
		int y2_b = y1_b + h;
        int angle = h/2;

        //highlight
        int y2_h = y1_b + h/2 + 1;

        //transparent background
        roundedBoxRGBA(renderer,x1_b,y1_b,x2_b,y2_b,angle,0,0,0,120);
        //life
        roundedBoxRGBA(renderer,x1_l,y1_l,x2_l,y2_l,angle,R,G,B,120);
        //lightbar
        roundedBoxRGBA(renderer,x1_l+2,y1_l+2,x2_l-2,y2_h,0,255,255,255,80);
        //border
        roundedRectangleRGBA(renderer,x1_b,y1_b,x2_b,y2_b,angle,0,0,0,255);

	}

void C_Shooter::render(S_Coord screen){
	C_GameUnits::render(screen);
	if (m_weapon->getShooting())
		m_weapon->render();
	renderLifeBar(screen.x, screen.y);
}
