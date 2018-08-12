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

#include "towers.h"
#include "grid.h"
#include "weapons.h"

#include "../texture.h"
#include "../time.h"


using namespace std;


C_Towers::C_Towers(string name,
		   int x_grid,
		   int y_grid,
		   int rank):C_Shooter(name, x_grid, y_grid, rank)
{
	m_lastSmokeTime = 0;
	m_smokeNbr = 1;
	m_justAdded = true;
}

void C_Towers::play(){
	string list[1] = {"boat"};
	this->shoot(list,1);
	if(!this->alive())
		this->kill();
}

void C_Towers::renderSmoke(){
	int imageNbr = m_animation[JUSTADDED]->getAnimNbr(0,7,100);
	//smoke_01_smoke0
	string fileName = "smoke_01_smoke" + to_string(imageNbr);
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, m_coord->getXScreen (),m_coord->getYScreen () + 36);
	if (imageNbr == 7)
		m_justAdded = false;
}

void C_Towers::render(S_Coord screen){
	renderSelected();
	C_Shooter::render(screen);

	if (m_justAdded)
		renderSmoke();
}

void C_Towers::renderSelected(){
	if (m_selected == true){
		int width = m_weapon->getFireRange()*2*TILE_HALF_WIDTH;
		drawEllipse(m_coord->getXScreen (),m_coord->getYScreen (),width, true);
	}
}

void C_Towers::drag(S_Coord screen)
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
			status = grid.isThisConstructible(tmp.getGrid ());
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
	C_Shooter::render(screen);
}

void C_Towers::drawEllipse(int x,
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

void C_Towers::drawRhombus(int x, int y, int width, int alpha, bool ok){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	Sint16 w =  width/2;
	Sint16 h =  w/2;
	Sint16 x1 = x - w;

	Sint16 y1 = y - h + TILE_HALF_HEIGHT; //center
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
//---------------------------------------------------

C_ArcherTower::C_ArcherTower(int x_grid,
		   int y_grid,
		   int rank):C_Towers("ArcherTower", x_grid, y_grid, rank)
{
	m_weapon = new C_Weapon("ARCHER",10,0,2000,2);
}

void C_ArcherTower::render(S_Coord screen){
	if(alive()){
	    renderSelected();
	    C_GameUnits::render(screen);
	    renderLifeBar(screen.x, screen.y);
	    if (m_weapon->getShooting())
		    m_weapon->render();

	    if (m_justAdded)
		    renderSmoke();
	}
}

C_Turbine::C_Turbine(int x_grid,
		   int y_grid,
		   int rank):C_Towers("Turbine", x_grid, y_grid, rank)
{
	m_weapon = new C_Weapon("WIND",0,1,0,2);
}

void C_Turbine::render(S_Coord screen){
	if(alive()){
	    renderSelected();
	    renderLifeBar(screen.x, screen.y);

	    S_Weapon current = m_weapon->getWeaponInfo();
	    int rotationSpeed = 200;
	    if (m_weapon->getShooting())
		    rotationSpeed = 50;
	    int imageNbr = m_animation[MAIN_ANIM]->getAnimNbr(0,7,rotationSpeed);

	    string fileName = imageName(ALIVE,current.direction,imageNbr);
	    C_TextureList& t=C_TextureList::Instances();
	    t.renderTexture(fileName, screen.x,screen.y + m_y_center_offset);

	    if (m_justAdded)
		    renderSmoke();
	}
}

