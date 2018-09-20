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

#include "../texture.h"
#include "../message.h"

using namespace std;

C_Shooter::C_Shooter(std::string name, int x_grid, int y_grid, int rank):
	C_GameUnits(name, x_grid, y_grid, rank)
{
	m_weapon = nullptr;
	m_lastShootTime = 0;
	m_cost = 50;
	C_Message m;
    string message = "Add new shooter: " + m_name +" life: "+ to_string(m_health) + " rank: "+ to_string(m_rank);
	m.printM(message);
	m_coord->displayStatus();

}

C_Shooter::~C_Shooter()
{
	delete m_weapon;
}
C_GameUnits*  C_Shooter::searchNextTarget(string type){
    C_Settings& settings=C_Settings::Instances();
    int gridSize = settings.getGridSize();

	int gridDiag = m_weapon->getFireRange();
	C_Grid& grid=C_Grid::Instances();
	int x_grid = m_coord->getXGrid();
	int y_grid = m_coord->getYGrid();
	C_GameUnits* target = nullptr;

	map<int, C_GameUnits*> list;
	for(int y = (y_grid - gridDiag); y <= (y_grid + gridDiag); y++){
		for(int x = (x_grid - gridDiag); x <= (x_grid + gridDiag); x++){
			if((x != x_grid || y != y_grid)){
			    if(x >= 0 && x <= gridSize && y >= 0 && y <= gridSize){
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

void C_Shooter::kill()
{
	C_Grid& grid=C_Grid::Instances();
	C_Message m;
	m.printM("kill " + m_name + " from:"+ to_string(m_coord->getXGrid ())
	            + ":" + to_string(m_coord->getYGrid ()) + "\n");
 	grid.moveToDead(m_coord->getXGrid (), m_coord->getYGrid ());
 	m_selected = false;
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
		int l = 50;
		int h = 6;

		//background
		int x1_b = x_screen - TILE_HALF_WIDTH/2;
		int y1_b = y_screen - 85;
		int x2_b = x1_b + l;
		int y2_b = y1_b + h;

		//health
        int x2_l = x1_b + (l*m_health/m_max_health);

		int R = 0, G = 200, B = 0;
        if (m_health < m_max_health/2){ R = 200; G = 0 ; }

        int angle = 2;
        if((x2_l - x1_b)<=(angle*2)){angle = 0;}

        //highlight
        int y2_h = y1_b + h/2 + 1;

        //transparent background
        roundedBoxRGBA(renderer,x1_b,y1_b,x2_b,y2_b,angle,0,0,0,60);
        //life
        roundedBoxRGBA(renderer,x1_b,y1_b,x2_l,y2_b,angle,R,G,B,120);
        //highlight
        roundedBoxRGBA(renderer,x1_b+2,y1_b+2,x2_l-2,y2_h,0,255,255,255,80);
        //border
        roundedRectangleRGBA(renderer,x1_b,y1_b,x2_b,y2_b,angle,0,0,0,255);

	}

void C_Shooter::render(S_Coord screen){
	C_GameUnits::render(screen);
	if (m_weapon->getShooting())
		m_weapon->render();
	renderLifeBar(screen.x, screen.y);
}

void C_Shooter::drag(S_Coord screen, bool water)
{
	m_justAdded = false;
	C_Grid& grid=C_Grid::Instances();

	C_CoordScreen coord(screen);

	int width = m_weapon->getFireRange()*2*TILE_HALF_WIDTH;
	int x = coord.getXGrid ();
	int y = coord.getYGrid ();
	//draw ellipse
	bool status = grid.isThisConstructible(x,y);
	drawEllipse(screen.x,screen.y,width, status);
	//draw square
	x -=2;
	y -=2;
	for(int i = 0; i < 3; i++){
		y++;
		for(int j = 0; j < 3; j++){
			x++;
			C_CoordGrid tmp(x,y);
			if(water){
		    status = grid.waterway(x,y);
			}
			else{
			    status = grid.isThisConstructible(tmp.getGrid ());
			}
			int x_s = tmp.getXScreen ();
			int y_s = tmp.getYScreen ();
			drawRhombus(x_s,y_s,70,40,status);
			if (i == 1 && j == 1){
				drawRhombus(x_s,y_s,70,90,status);
				}

		}
		x = coord.getXGrid () - 2;
	}

	screen.y -= (2*TILE_HALF_HEIGHT);
	render(screen);
}


void C_Shooter::drawEllipse(int x,
		int y,
		int width,
		bool ok){
		int animNbr = m_animation[SELECTED]->getAnimNbr(10,20,500);
		C_Window& win=C_Window::Instances();
		width = width*90/100;
		int height = width/2;
		int R = 0, G = 200, B = 0, A = 10;
			if(ok == false)
				R = 120, G = 0, B = 0;

        for(int i = animNbr; i >= 0; i--){
        int w = width-i;
		aaellipseRGBA(win.getRenderer(),x,y,w,w/2,R,G,B,A+4*(animNbr -i));
		}
        filledEllipseRGBA(win.getRenderer(),x,y,width,height,R,G,B,A*4);


}

void C_Shooter::drawRhombus(int x, int y, int width, int alpha, bool ok){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	Sint16 w =  width/2;
	Sint16 h =  w/2;
	Sint16 x1 = x - w;

	Sint16 y1 = y - h + 14; //center
	Sint16 x2 = x1 + w;
	Sint16 y2 = y1 + h;
	Sint16 x3 = x1 + (w*2);
	Sint16 y3 = y1;
	Sint16 x4 = x2;
	Sint16 y4 = y1 - h;
	Sint16 vx[] = {x1,x2,x3,x4};
	Sint16 vy[] = {y1,y2,y3,y4};
	int R = 0, G = 200, B = 0, A = alpha;
			if(ok == false)
				R = 120, G = 0, B = 0;
	filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);

	A = alpha * 2;
	if (alpha > 255)
		alpha = 255;
	aapolygonRGBA(renderer,vx,vy,4,R,G,B,A);

}
