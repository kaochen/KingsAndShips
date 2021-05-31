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

#define STEPSNBR 96 // lower tilewidth possible (128x128 image -> tilewidth = 96)

#include "coord.h"
#include "time.h"
#include "level/grid.h"
#include "locator.h"

using namespace std;


C_Coord::C_Coord(int x_grid, int y_grid)
{

	C_Grid& grid= C_Locator::getGrid();
	int gridSize = grid.getFullSize();
	if (x_grid < 0)
		x_grid = 0;
	if (y_grid < 0)
		y_grid = 0;
	if (x_grid > gridSize)
		x_grid = gridSize;
	if (y_grid > gridSize)
		y_grid = gridSize;

	m_grid = {x_grid,y_grid};
	m_delta = {0,0};
}

C_Coord::C_Coord(S_Coord coord)
{
	if (coord.x < 0) //to quite gcc
		cout << "";
}


C_Coord::C_Coord(const C_Coord &a)
{
    copy(a);
}



bool operator==(C_Coord const &a, C_Coord const &b)
{
	return a.isEqual(b);

}

bool operator!=(C_Coord const &a, C_Coord const &b)
{
	bool test = a.isEqual(b);
	if(test)
		return false;
	else
		return true;
}

void C_Coord::operator=(C_Coord const &a){
    copy(a);
}

void C_Coord::copy(C_Coord const &a){
   	m_grid.x = a.m_grid.x;
	m_grid.y = a.m_grid.y;
	m_delta = a.m_delta;
}

bool C_Coord::isEqual(C_Coord const &b) const
{
	return (m_grid.x == b.m_grid.x && m_grid.y == b.m_grid.y);
}



bool C_Coord::onScreen()
{
	C_Settings& settings=C_Locator::getSettings();
	bool visible = false;
	S_Coord screen = gridToScreen(m_grid);
	if(screen.x > - 2*settings.getTileWidth() && screen.x < settings.getWindowWidth() + settings.getTileWidth()/2
			&& screen.y > - 2*settings.getTileHeight() && screen.y < settings.getWindowHeight() + settings.getTileWidth()) {
		visible = true;
	}
	return visible;
}

S_Coord C_Coord::screenToGrid(S_Coord screen)
{
	C_Settings& settings=C_Locator::getSettings();
	S_Coord cameraPos = settings.getCameraPosition();
	float xOffset = cameraPos.x;
	float yOffset = cameraPos.y + settings.getTileHeight()/2;
	float tempX = 0.0, tempY = 0.0;
	S_Coord coord;
	tempX = ((screen.x - xOffset ) / (settings.getTileWidth()/2) + (screen.y + yOffset)/(settings.getTileHeight()/2) )/2;
	tempY = (screen.y + yOffset )/(settings.getTileHeight()) - (screen.x - xOffset)/(settings.getTileWidth());

	coord.x = tempX ;
	coord.y = tempY ;
	return coord;
}

S_Coord C_Coord::gridToScreen(S_Coord grid)
{
	C_Settings& settings=C_Locator::getSettings();
	S_Coord cameraPos = settings.getCameraPosition();
	S_Coord screen;
	screen.x = cameraPos.x + (grid.x - grid.y)* (settings.getTileWidth()/2);
	screen.y = (grid.y + grid.x) * (settings.getTileHeight()/2) - cameraPos.y;
	return screen;
}

void C_Coord::displayStatus()
{
	cout << "\tgrid: "<< m_grid.x << ":"<< m_grid.y;
	cout << "\tscreen: "<< getXScreen() << ":"<< getYScreen();
	cout << "\tdelta: "<< m_delta.x << ":"<< m_delta.y << endl;
}

S_Coord C_Coord::getGrid()
{
	return m_grid;
}

int C_Coord::getXGrid() const
{
	return m_grid.x;
}

int C_Coord::getYGrid() const
{
	return m_grid.y;
}

S_Coord C_Coord::getScreen()
{
	C_Settings& settings= C_Locator::getSettings();
	int tileWidth = settings.getTileWidth();
	int step = tileWidth/STEPSNBR;
	S_Coord ret = gridToScreen(m_grid);
	ret.x +=  m_delta.x*step*2;
	ret.y +=  m_delta.y*step;
	return ret;
}


int C_Coord::getXScreen()
{
	return getScreen().x;
}
int C_Coord::getYScreen()
{
	return getScreen().y;
}

void C_Coord::centerOnTile()
{
	m_delta = {0,0};
}

int C_Coord::angleToDirection(float const &angle)
{
	int  direction = UNKNOWN;
	if (angle > -45.0 && angle <= 45.0) {
		direction = NORTH_WEST;
	} else if (angle > 45.0 && angle <= 76.7) {
		direction = NORTH;
	} else if(angle > 76.7 && angle <=103.3) {
		direction = NORTH_EAST;
	} else if(angle > 103.3 && angle <=135.0) {
		direction = EAST;
	} else if((angle > 135.0 && angle <= 225.0)) {
		direction = SOUTH_EAST;
	} else if(angle > 225.0 && angle <= 256.7) {
		direction = SOUTH;
	} else if(angle > 256.7 && angle <= 283.3) {
		direction = SOUTH_WEST;
	} else if(angle > 283.3 && angle <= 315.0) {
		direction = WEST;
	} else if(angle > 315.0 && angle <= 405.0) {
		direction = NORTH_WEST;
	} else {
		direction = EAST;
	}
	return direction;
}

float C_Coord::directionToAngle(int const direction)
{
	float  angle = 0;
	switch (direction) {
	case NORTH_WEST :
		angle = 0.0;
		break;
	case NORTH :
		angle = 63.4;
		break;
	case NORTH_EAST :
		angle = 90.0;
		break;
	case EAST :
		angle = 116.6;
		break;

	case SOUTH_EAST:
		angle = 180.0;
		break;
	case SOUTH :
		angle = 243.4;
		break;
	case SOUTH_WEST :
		angle = 270.0;
		break;
	case WEST :
		angle = 283.3;
		break;
	case UNKNOWN :
		angle = 116.6;
		break;

	}
	return angle;
}

void C_Coord::refreshGrid(){
	C_Settings& settings= C_Locator::getSettings();
	S_Coord currentScreen = getScreen();
	S_Coord newGrid = screenToGrid(currentScreen);
	if(newGrid.x != m_grid.x || newGrid.y != m_grid.y){
		m_grid = newGrid;
		m_delta = {0,0};
		int tileWidth = settings.getTileWidth();
		int step = tileWidth/STEPSNBR;
		if(step != 0){
			m_delta.x = (currentScreen.x - getXScreen())/(2*step);
			m_delta.y = (currentScreen.y - getYScreen())/step;
		}
	}

}

void C_Coord::move(float const &angle, int speed)
{

	//cout << "Angle : " << angle << endl;
	int s = speed;
	if(s <= 1){
		s = 1;
	} else if (s >= 5){
		s = 5;
	}

	if(angle > 45.0 && angle <= 76.7) {
		//cout << "NORTH" << endl;
		m_delta.x +=s;
		m_delta.y -=s;

	} else if(angle > 76.7 && angle <=103.3) {
		//cout << "NORTH_EAST" << endl;
		m_delta.x +=s;

	} else if (angle > 103.3 && angle <=135.0) {
		//cout << "EAST" << endl;
		m_delta.x +=s;
		m_delta.y +=s;

	} else if ((angle > 135.0 && angle <= 225.0)) {
		//cout << "SOUTH_EAST" << endl;
		m_delta.y +=s;

	} else if(angle > 225.0 && angle <= 256.7) {
		//cout << "SOUTH" << endl;
		m_delta.x -=s;
		m_delta.y +=s;

	} else if(angle > 256.7 && angle <= 283.3) {
		//cout << "SOUTH_WEST" << endl;
		m_delta.x -=s;

	} else if(angle > 283.3 && angle <= 315.0) {
		//cout << "WEST" << endl;
		m_delta.x -=s;
		m_delta.y -=s;

	} else if((angle > 315.0 && angle <= 405.0) || (angle > -45.0 && angle <= 45.0)) {
		//cout << "NORTH_WEST" << endl;
		m_delta.y -=s;

	} else {
		cout << "Angle is not between 0 and 360 : " << angle << endl;
	}
}


float C_Coord::atan2_360(int const &ab, int const &bc)
{
	float angle = atan2(ab,bc);
	angle = 180 - (angle *180/3.14159265359);
	if(angle < 0)
		angle +=360;
	else if(angle > 360)
		angle -=360;
	return angle;
}

bool C_Coord::atCenter(S_Coord destGrid){
	bool ret = false;
	if(destGrid.x == m_grid.x && destGrid.y == m_grid.y){
		if(m_delta.x < 2 &&  m_delta.x > -2 && m_delta.y < 2 &&  m_delta.y > -2){
			ret = true;
		}
	}
	return ret;
}



bool C_Coord::closeToCenter(S_Coord grid, int step_nbr)
{
	C_CoordGrid tmp(grid);
	S_Coord center = tmp.getScreen();
	int l = getXScreen() - center.x;
	int h = getYScreen() - center.y;
	if ( l < 0)
		l *=-1;
	if (h < 0)
		h *=-1;

	C_Settings& settings= C_Locator::getSettings();
	int tileWidth = settings.getTileWidth();
	int step = (tileWidth/STEPSNBR)*step_nbr;

	if(l < 2*step && h < step) {
		//cout << "center true" << endl;
		return true;
	} else {
		//cout << "center false" << endl;
		return false;
	}

}


int C_Coord::guessADirection(S_Coord start,S_Coord end)
{

	//enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
	int direction = UNKNOWN;

	if(end.x == start.x && end.y == start.y) {
		direction = UNKNOWN;
	} else if(end.x > start.x && end.y == start.y)
		direction = NORTH_EAST;
	else if(end.x > start.x && end.y > start.y)
		direction = EAST;
	else if(end.x == start.x && end.y > start.y)
		direction = SOUTH_EAST;
	else if(end.x > start.x && end.y < start.y)
		direction = NORTH;
	else if(end.x < start.x && end.y == start.y)
		direction = SOUTH_WEST;
	else if(end.x < start.x && end.y < start.y)
		direction = WEST;
	else if(end.x == start.x && end.y < start.y)
		direction = NORTH_WEST;
	else if(end.x < start.x && end.y > start.y)
		direction = SOUTH;
	else
		direction = UNKNOWN;

	return direction;

};




C_CoordGrid::C_CoordGrid(S_Coord coord): C_Coord(coord)
{
	m_grid = {coord.x,coord.y};
	m_delta = {0,0};
	//cout << "grid to screen" << endl;
}

C_CoordGrid::C_CoordGrid(int x_grid, int y_grid ): C_Coord(x_grid, y_grid)
{
	C_Grid& grid= C_Locator::getGrid();
	int size = grid.getFullSize();
	if (x_grid < 0)
		x_grid = 0;
	if (y_grid < 0)
		y_grid = 0;
	if (x_grid > size)
		x_grid = size;
	if (y_grid > size)
		y_grid = size;

	m_grid = {x_grid,y_grid};
	m_delta = {0,0};

}


C_CoordScreen::C_CoordScreen(S_Coord coord): C_Coord(coord)
{
	createCoordFromScreen(coord.x, coord.y);
}

C_CoordScreen::C_CoordScreen(int x_screen, int y_screen ): C_Coord(x_screen, y_screen)
{
	createCoordFromScreen(x_screen, y_screen);
}

void C_Coord::createCoordFromScreen(int x_screen, int y_screen)
{
	C_Settings& settings= C_Locator::getSettings();
	S_Coord screen = {x_screen, y_screen};
	m_grid = screenToGrid(screen);
	int tileWidth = settings.getTileWidth();
	int step = tileWidth/STEPSNBR;
	m_delta.x = (screen.x - getXScreen())/step;
	m_delta.y = (screen.y - getYScreen())/step;
}


