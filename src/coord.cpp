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
	m_screenDelta = {0,0};
}

C_Coord::C_Coord(S_Coord coord)
{
	if (coord.x < 0) //to quite gcc
		cout << "";
}


C_Coord::C_Coord(const C_Coord &a)
{
	m_grid.x = a.m_grid.x;
	m_grid.y = a.m_grid.y;

	m_screenDelta.x = a.m_screenDelta.x;
	m_screenDelta.y = a.m_screenDelta.y;
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


bool C_Coord::isEqual(C_Coord const &b) const
{
	return (m_grid.x == b.m_grid.x && m_grid.y == b.m_grid.y);
}

void C_Coord::applyOffset(S_Coord offset)
{
	m_screenDelta.x += offset.x;
	m_screenDelta.y += offset.y;
}


bool C_Coord::onScreen()
{
	C_Settings& settings=C_Locator::getSettings();
	bool visible = false;
	S_Coord screen = gridToScreen(m_grid);
	if(screen.x > - settings.getTileWidth()/2 && screen.x < settings.getWindowWidth() + settings.getTileWidth()/2
			&& screen.y > - settings.getTileHeight()/2 && screen.y < settings.getWindowHeight() + settings.getTileWidth()) {
		visible = true;
	}
	return visible;
}

S_Coord C_Coord::screenToGrid(S_Coord screen)
{
	C_Settings& settings=C_Locator::getSettings();
	S_Coord cameraPos = settings.getCameraPosition();
	float xOffset = cameraPos.x + settings.getTileWidth()/2;
	float yOffset = cameraPos.y + settings.getTileHeight()/2;
	float tempX = 0.0, tempY = 0.0;
	S_Coord coord;
	tempX = ((screen.x - xOffset ) / (settings.getTileWidth()/2) + (screen.y + yOffset)/(settings.getTileHeight()/2) )/2;
	tempY = (screen.y + yOffset )/(settings.getTileHeight()) - (screen.x - xOffset)/(settings.getTileWidth());

	coord.x = tempX + 1;
	coord.y = tempY;
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

	cout << "\tdelta: "<< m_screenDelta.x << ":"<< m_screenDelta.y << endl;
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
	S_Coord ret;
	ret = gridToScreen(m_grid);
	ret.x +=  m_screenDelta.x;
	ret.y +=  m_screenDelta.y;
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
	m_screenDelta = {0,0};
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


void C_Coord::move(float const &angle, int const &speed)
{
	C_Time& time = C_Locator::getTime();
	//One pixel can't be split, on even frame number use an offset of 0 instead of 1.
	int halfPixel = 1;
	if(time.getFrameNbr ()%2 == 0) {
		halfPixel =0;
	}

	//cout << "Angle : " << angle << endl;

	if(angle > 45.0 && angle <= 76.7) {
		//cout << "NORTH" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.x +=1;
			m_screenDelta.y -=halfPixel;
			break;
		case SLOW:
			m_screenDelta.x +=2;
			m_screenDelta.y -=1;
			break;
		case NORMAL:
			m_screenDelta.x +=3;
			m_screenDelta.y -=(1 + halfPixel);
			break;
		case FAST:
			m_screenDelta.x +=4;
			m_screenDelta.y -=2;
			break;
		case VERY_FAST:
			m_screenDelta.x +=5;
			m_screenDelta.y -=(2 + halfPixel);
			break;
		}
	} else if(angle > 76.7 && angle <=103.3) {
		//cout << "NORTH_EAST" << endl;
		m_screenDelta.x +=speed;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.x +=1;
			break;
		case SLOW:
			m_screenDelta.x +=2;
			break;
		case NORMAL:
			m_screenDelta.x +=3;
			break;
		case FAST:
			m_screenDelta.x +=4;
			break;
		case VERY_FAST:
			m_screenDelta.x -=5;
			break;
		}
	} else if (angle > 103.3 && angle <=135.0) {
		//cout << "EAST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.x +=1;
			m_screenDelta.y +=halfPixel;
			break;
		case SLOW:
			m_screenDelta.x +=2;
			m_screenDelta.y +=1;
			break;
		case NORMAL:
			m_screenDelta.x +=3;
			m_screenDelta.y +=(1 + halfPixel);
			break;
		case FAST:
			m_screenDelta.x +=4;
			m_screenDelta.y +=2;
			break;
		case VERY_FAST:
			m_screenDelta.x +=5;
			m_screenDelta.y +=(2 + halfPixel);
			break;
		}
	}

	else if ((angle > 135.0 && angle <= 225.0)) {
		//cout << "SOUTH_EAST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.y +=halfPixel;
			break;
		case SLOW:
			m_screenDelta.y +=1;
			break;
		case NORMAL:
			m_screenDelta.y +=(1 + halfPixel);
			break;
		case FAST:
			m_screenDelta.y +=2;
			break;
		case VERY_FAST:
			m_screenDelta.y +=(2 + halfPixel);
			break;
		}
	} else if(angle > 225.0 && angle <= 256.7) {

		//cout << "SOUTH" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.x -=1;
			m_screenDelta.y +=halfPixel;
			break;
		case SLOW:
			m_screenDelta.x -=2;
			m_screenDelta.y +=1;
			break;
		case NORMAL:
			m_screenDelta.x -=3;
			m_screenDelta.y +=(1 + halfPixel);
			break;
		case FAST:
			m_screenDelta.x -=4;
			m_screenDelta.y +=2;
			break;
		case VERY_FAST:
			m_screenDelta.x -=5;
			m_screenDelta.y +=(2 + halfPixel);
			break;
		}
	} else if(angle > 256.7 && angle <= 283.3) {
		//cout << "SOUTH_WEST" << endl;
		m_screenDelta.x -=speed;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.x -=1;
			break;
		case SLOW:
			m_screenDelta.x -=2;
			break;
		case NORMAL:
			m_screenDelta.x -=3;
			break;
		case FAST:
			m_screenDelta.x -=4;
			break;
		case VERY_FAST:
			m_screenDelta.x -=5;
			break;
		}
	} else if(angle > 283.3 && angle <= 315.0) {
		//cout << "WEST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.x -=1;
			m_screenDelta.y -=halfPixel;
			break;
		case SLOW:
			m_screenDelta.x -=2;
			m_screenDelta.y -=1;
			break;
		case NORMAL:
			m_screenDelta.x -=3;
			m_screenDelta.y -=(1 + halfPixel);
			break;
		case FAST:
			m_screenDelta.x -=4;
			m_screenDelta.y -=2;
			break;
		case VERY_FAST:
			m_screenDelta.x -=5;
			m_screenDelta.y -=(2 + halfPixel);
			break;
		}
	} else if((angle > 315.0 && angle <= 405.0) || (angle > -45.0 && angle <= 45.0)) {
		//cout << "NORTH_WEST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_screenDelta.y -=halfPixel;
			break;
		case SLOW:
			m_screenDelta.y -=1;
			break;
		case NORMAL:
			m_screenDelta.y -=2;
			break;
		case FAST:
			m_screenDelta.y -=3;
			break;
		case VERY_FAST:
			m_screenDelta.y -=4;
			break;
		}
	}

	else {
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





bool C_Coord::closeToCenter(S_Coord grid, int px_length)
{
	C_CoordGrid tmp(grid);
	tmp.centerOnTile();
	S_Coord center = tmp.getScreen();
	int l = gridToScreen(m_grid).x + m_screenDelta.x - center.x;
	int h = gridToScreen(m_grid).y + m_screenDelta.y - center.y;
	if ( l < 0)
		l *=-1;
	if (h < 0)
		h *=-1;
	if(l < 2*px_length && h < px_length) {
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
	m_screenDelta = {0,0};
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
	m_screenDelta = {0,0};

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
	S_Coord screen = {x_screen, y_screen};
	m_grid = screenToGrid(screen);

	m_screenDelta.x = screen.x - getScreen().x;
	m_screenDelta.y = screen.y - getScreen().y;
}




