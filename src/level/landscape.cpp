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


#include "landscape.h"
#include "grid.h"
#include "../settings.h"
#include "../locator.h"
#include "../textureList.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

using namespace std;



C_Landscape::C_Landscape(S_Coord town)
{
	m_waterDrift.x = 0;
	m_waterDrift.y = 0;
	m_animWater= new C_Anim("Water",1,4,FRAMERATE);
	m_waterDirection = waterDirection(town);
}

C_Landscape::~C_Landscape()
{
	delete m_animWater;
}


void C_Landscape::render(int gridSize)
{
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer* renderer = win.getRenderer ();
	C_Settings& settings=C_Locator::getSettings();
	S_Coord camera = settings.getCameraPosition();
	C_TextureList& t= C_Locator::getTextureList();
	//cout << "direction: " << direction << endl;
	SDL_SetRenderDrawColor(renderer, 26, 60, 108, 255);	//fill with background color
	SDL_RenderClear(renderer);
	gridSize +=2;

	for(int j = -2; j < (gridSize+4); j++) {
		int tilewidth = 192 ;
	    int tileheight = 96;

        int h = tilewidth;
            if (j%2 ==0){
            h = 0;
        }
		int y = j*tileheight - camera.y;
		for(int i = -(gridSize/4); i < (gridSize/4); i++) {
			int x =  i*2*tilewidth + h + camera.x;
			t.renderTexture("Water_00_Blue_EE_0", x + m_waterDrift.x,y + m_waterDrift.y, true);
			t.renderTexture("Water_00_White_EE_0", x + m_waterDrift.x*(-1),y + m_waterDrift.y*(-1), true);
		}
	}
}

void C_Landscape::play(){
	C_Settings& settings=C_Locator::getSettings();
	int pause = m_animWater->getImageNbr();
	m_animWater->play();
	if(m_animWater->end()){
		m_animWater->reset();
	}

	int limit = 2*settings.getTileWidth() - 1;

	if(pause%2 == 0) {
		switch (m_waterDirection) {
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
			limit = 2*settings.getTileWidth() -1;
			if(m_waterDrift.x > limit || m_waterDrift.x < -limit) {
				m_waterDrift.x = 0;
				m_waterDrift.y = 0;
			}
			break;
		case SOUTH_EAST:
			m_waterDrift.y -= 1;
			limit = settings.getTileWidth() -1;
			break;
		case NORTH_WEST:
			m_waterDrift.y += 1;
			limit = settings.getTileWidth() -1;
			break;
		case NORTH_EAST:
			m_waterDrift.x -= 1;
			limit = 2*settings.getTileWidth() -1;
			if(m_waterDrift.x > limit || m_waterDrift.x < -limit) {
				m_waterDrift.x = 0;
				m_waterDrift.y = 0;
			}
			break;

		}

		if(m_waterDrift.y > limit || m_waterDrift.y < -limit) {
			m_waterDrift.x = 0;
			m_waterDrift.y = 0;
		}
	}
}



int C_Landscape::waterDirection(S_Coord town)
{

	C_Settings& settings=C_Locator::getSettings();

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


void C_Landscape::renderBottomMask(int gridSize)
{
	C_Settings& settings=C_Locator::getSettings();
	//get coordinates from the tile that are on corners
	C_CoordScreen bottomLeft(0,settings.getWindowHeight());
	S_Coord b_left = bottomLeft.getGrid();
	C_CoordScreen bottomRight(settings.getWindowWidth(),settings.getWindowHeight());
	S_Coord b_right = bottomRight.getGrid();

	S_Coord corner = {0,gridSize}; //corner of the map.
	//cover the bottom left corner with tiles
	C_OutsideTile tile;
	for(int x = corner.x; x < b_right.x + 1; x++) {
		for(int y = corner.y ; y < b_left.y + 1; y++) {
			S_Coord c = {x,y};
			tile.render(c);
		}
	}
	//cover the bottom right corner with tiles
	C_CoordScreen topRight(settings.getWindowWidth(),0);
	S_Coord t_right = topRight.getGrid();
	corner.x = gridSize;

	//int count = 0;
	for(int x = corner.x; x < b_right.x + 1; x++) {
		for(int y = corner.y ; y > t_right.y - 1; y--) {
			S_Coord c = {x,y};
			tile.render(c);
		}
	}
	//cout << "border render: " << count << endl;
}


void C_Landscape::renderTopMask(int gridSize)
{
	C_Settings& settings=C_Locator::getSettings();
	//get coordinates from the tile that are on corners
	C_CoordScreen topLeft(0,0);
	S_Coord t_left = topLeft.getGrid();
	C_CoordScreen bottomLeft(0,settings.getWindowHeight());
	S_Coord b_left = bottomLeft.getGrid();
	C_CoordScreen topRight(settings.getWindowWidth(),0);
	S_Coord t_right = topRight.getGrid();

	S_Coord corner = {gridSize,0}; //corner of the map.
	//cover the top right corner with tiles
	C_OutsideTile tile;
	for(int x = corner.x; x > t_left.x - 2; x--) {
		for(int y = corner.y - 1 ; y > t_right.y - 2; y--) {
			S_Coord c = {x,y};
			tile.render(c);
		}
	}
	//cover the top left corner with tiles

	corner.x = -1;
	//int count = 0;
	for(int x = corner.x; x > t_left.x - 2; x--) {
		for(int y = corner.y ; y < b_left.y + 2; y++) {
			S_Coord c = {x,y};
			tile.render(c);
		}
	}

	//cout << "border render: " << count << endl;
}

//---------------------------Decors-------------------------

void C_Decors::render(S_Coord screen)
{
	string fileName = m_model.name;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE, true);
}

//---------------------------Clouds-------------------------

C_Clouds::C_Clouds(int x_grid, int y_grid):
	C_GameUnits("clouds_01", x_grid, y_grid, 0)
{
	m_type = rand() %8;
	m_typeOnTop = rand() %8;
	m_cloudName = "clouds_Cloud_0"+to_string(m_type);
	m_cloudOnTopName = "clouds_Cloud_0"+to_string(m_typeOnTop);
	m_shadowName = "clouds_Shadow_0"+to_string(m_type);
	m_shadowOnTopName = "clouds_Shadow_0"+to_string(m_typeOnTop);

	m_randStart = rand() %800;
	m_anim.add(new C_AnimRewind("Moving",-20,20,60,true));
	m_anim.add(new C_Anim("Waiting",0,0,m_randStart));

	m_state = "Moving";
}

void C_Clouds::play()
{
		if(m_state == "Waiting"){
			m_anim.get(m_state)->play();
			if(m_anim.get(m_state)->end()){
				changeState("Moving");
			}
		}
		if(m_state == "Moving"){
			m_anim.get(m_state)->play();
			if(m_anim.get(m_state)->end()){
				changeState("Waiting");
			}
		}
}

void C_Clouds::render()
{
	//cout << "image name is "<< fileName << endl;
	int x = m_anim.get("Moving")->getImageNbr();

	C_TextureList& t= C_Locator::getTextureList();
	C_Settings& settings=C_Locator::getSettings();
	int h = settings.getTileHeight()/2;
	//t.renderTexture(m_shadowName, m_coord.getXScreen()-h,m_coord.getYScreen()+3*h,CENTER_TILE);
	t.renderTexture(m_shadowOnTopName, m_coord.getXScreen()-h + x ,m_coord.getYScreen()+3*h,CENTER_TILE, true);

	t.renderTexture(m_cloudName, m_coord.getXScreen(),m_coord.getYScreen() + 5,CENTER_TILE, true);
	t.renderTexture(m_cloudOnTopName, m_coord.getXScreen()+x,m_coord.getYScreen()+ 5,CENTER_TILE, true);
}




//---------------------------C_Ground -------------------------

void C_Ground::render()
{
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(m_model.name, m_coord.getXScreen(),m_coord.getYScreen(),CENTER_TILE, true);

	size_t found = m_model.name.find("Water");
	if(found == string::npos) {
		if ((m_coord.getXGrid()+m_coord.getYGrid())%2 == 0) {
			t.renderTexture("Ground_01_darken", m_coord.getXScreen(),m_coord.getYScreen() -2, CENTER_TILE, true);
		}
	}
}

//---------------------------C_Trees-------------------------
C_Trees::C_Trees(S_Unit unit):
	C_Decors(unit)
{
	int size =  unit.name.size() - 3;  //cut the last tree letters Trees_01_00 -> Trees_01
	m_model.name = m_model.name.substr(0,size);
	m_state = "Wind";
	m_anim.add(new C_AnimRewind("Wind",1,10,90,true));
}

void C_Trees::play()
{
	m_anim.get(m_state)->play();
}

void C_Trees::render(S_Coord screen)
{
	int imageNbr = m_anim.getImageNbr(m_state);
	string fileName = m_model.name + "_0" + to_string(imageNbr);
	if(imageNbr>9) {
		fileName = m_model.name + "_" + to_string(imageNbr);
	}
	//cout << "image name is "<< fileName << endl;
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE, true);
}

//---------------------------C_Trees-------------------------
C_OutsideTile::C_OutsideTile():
	C_GameUnits("outsideTile", 0,0,0)
{

}

void C_OutsideTile::render(S_Coord grid)
{
	C_Settings& settings=C_Locator::getSettings();
	int x_min = 0 - (settings.getTileWidth()/2);
	int y_min = 0 - (settings.getTileHeight()/2);
	int x_max = settings.getWindowWidth() + (settings.getTileWidth()/2);
	int y_max = settings.getWindowHeight() + (settings.getTileHeight()/2);
	C_CoordGrid coord(grid);
	S_Coord screen = coord.getScreen();
	if(screen.x > x_min && screen.x < x_max && screen.y > y_min && screen.y < y_max) {
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Ground_01_paper", screen.x,screen.y, true);
	}
}


