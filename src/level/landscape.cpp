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
#include "../texture.h"
#include "../locator.h"
#include <stdlib.h>

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


void C_Landscape::render(int gridSize)
{
	renderWater(m_waterDirection,gridSize);
};

void C_Landscape::renderWater(int direction, int gridSize)
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
		int h = 2*TILE_HALF_WIDTH;
		if (j%2 ==0)
			h = 0;

		int y = j*TILE_HALF_WIDTH - camera.y;
		for(int i = -(gridSize/4); i < (gridSize/4); i++) {
			int x =  i*4*TILE_HALF_WIDTH + h + camera.x;
			t.renderTexture("Water_00_Blue_EE_0", x + m_waterDrift.x,y + m_waterDrift.y);
			t.renderTexture("Water_00_White_EE_0", x + m_waterDrift.x*(-1),y + m_waterDrift.y*(-1));
		}
	}
	int pause = m_animWater->getAnimNbr(1,4,FRAMERATE);

	int limit = 4*TILE_HALF_WIDTH - 1;

	if(pause%2 == 0) {
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
			if(m_waterDrift.x > limit || m_waterDrift.x < -limit) {
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

C_Decors::C_Decors(string name, int x_grid, int y_grid):
	C_GameUnits(name, x_grid, y_grid, 0)
{
}

void C_Decors::render(S_Coord screen)
{
	string fileName = m_name;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE);
}

//---------------------------Decors-------------------------

C_Clouds::C_Clouds(int x_grid, int y_grid):
	C_GameUnits("clouds_01", x_grid, y_grid, 0)
{
	m_type = rand() %3 + 1;
}

void C_Clouds::render()
{
	string fileName = "clouds_0"+to_string(m_type);
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("clouds_shadow", m_coord.getXScreen()-10,m_coord.getYScreen()+2*TILE_HALF_HEIGHT,CENTER_TILE);
	t.renderTexture(fileName, m_coord.getXScreen(),m_coord.getYScreen(),CENTER_TILE);
}




//---------------------------C_Ground -------------------------
C_Ground::C_Ground(string name, int x_grid, int y_grid):
	C_GameUnits(name, x_grid, y_grid,0)
{
}

void C_Ground::render()
{
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(m_name, m_coord.getXScreen(),m_coord.getYScreen(),CENTER_TILE);

	size_t found = m_name.find("Water");
	if(found == string::npos) {
		if ((m_coord.getXGrid()+m_coord.getYGrid())%2 == 0) {
			t.renderTexture("Ground_01_darken", m_coord.getXScreen(),m_coord.getYScreen() -2, CENTER_TILE);
		}
	}
}


//---------------------------C_Trees-------------------------
C_Trees::C_Trees(string name, int x_grid, int y_grid):
	C_Decors(name, x_grid, y_grid)
{
	int size =  name.size() - 3;  //cut the last tree letters Trees_01_00 -> Trees_01
	m_name = m_name.substr(0,size);
	m_imageNbr =  0;
	int random = rand() %10;
	m_animation[MAIN_ANIM]->setAnimNbr(random);
}

void C_Trees::play()
{
	m_imageNbr = m_animation[MAIN_ANIM]->getLoopAnimNbr(0,10,90);
}

void C_Trees::render(S_Coord screen)
{
	string fileName = m_name + "_0" + to_string(m_imageNbr);
	if(m_imageNbr>9) {
		fileName = m_name + "_" + to_string(m_imageNbr);
	}
	//cout << "image name is "<< fileName << endl;
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE);
}

//---------------------------C_Trees-------------------------
C_OutsideTile::C_OutsideTile():
	C_GameUnits("outsideTile", 0,0,0)
{

}

void C_OutsideTile::render(S_Coord grid)
{
	C_Settings& settings=C_Locator::getSettings();
	int x_min = 0 - TILE_HALF_WIDTH;
	int y_min = 0 - TILE_HALF_HEIGHT;
	int x_max = settings.getWindowWidth() + TILE_HALF_WIDTH;
	int y_max = settings.getWindowHeight() + TILE_HALF_HEIGHT;
	C_CoordGrid coord(grid);
	S_Coord screen = coord.getScreen();
	if(screen.x > x_min && screen.x < x_max && screen.y > y_min && screen.y < y_max) {
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Ground_01_paper", screen.x,screen.y);
	}
}


