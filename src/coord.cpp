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
	int gridSize = grid.getSize();
	if (x_grid < 0)
		x_grid = 0;
	if (y_grid < 0)
		y_grid = 0;
	if (x_grid > gridSize)
		x_grid = gridSize;
	if (y_grid > gridSize)
		y_grid = gridSize;

	m_this.grid = {x_grid,y_grid};
	m_this.screen = gridToScreen(m_this.grid);
	m_this.delta = {0,0};
}

C_Coord::C_Coord(S_Coord coord)
{
	if (coord.x < 0) //to quite gcc
		cout << "";
}


C_Coord::C_Coord(const C_Coord &a)
{
	m_this.grid.x = a.m_this.grid.x;
	m_this.grid.y = a.m_this.grid.y;
	m_this.screen.x = a.m_this.screen.x;
	m_this.screen.y = a.m_this.screen.y;
	m_this.delta.x = a.m_this.delta.x;
	m_this.delta.y = a.m_this.delta.y;
}

C_Coord::~C_Coord()
{
};


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
	//cout << "test equality " << m_this.grid.x << ":" << m_this.grid.y << " ";
	//cout << b.m_this.grid.x << ":" << b.m_this.grid.y << endl;
	return (m_this.grid.x == b.m_this.grid.x && m_this.grid.y == b.m_this.grid.y);
}

void C_Coord::applyOffset(S_Coord offset)
{
	m_this.delta.x += offset.x;
	m_this.delta.y += offset.y;
}


bool C_Coord::onScreen()
{
	C_Settings& settings=C_Locator::getSettings();
	bool visible = false;
	if(m_this.screen.x > - TILE_HALF_WIDTH && m_this.screen.x < settings.getWindowWidth() + TILE_HALF_WIDTH
			&& m_this.screen.y > - TILE_HALF_HEIGHT && m_this.screen.y < settings.getWindowHeight() + 4*TILE_HALF_HEIGHT) {
		visible = true;
	}
	return visible;
}

S_Coord C_Coord::screenToGrid(S_Coord screen)
{
	C_Settings& settings=C_Locator::getSettings();
	S_Coord cameraPos = settings.getCameraPosition();
	float xOffset = cameraPos.x;
	float yOffset = cameraPos.y + TILE_HALF_HEIGHT/2;
	float tempX = 0.0, tempY = 0.0;
	S_Coord coord;
	tempX = ( ((screen.x - xOffset ) / TILE_HALF_WIDTH + (screen.y + yOffset)/TILE_HALF_HEIGHT )/2);
	tempY = ( (screen.y + yOffset )/(TILE_HALF_HEIGHT*2) - (screen.x - xOffset)/(TILE_HALF_WIDTH*2));
	coord.x = tempX;
	coord.y = tempY;
	return coord;
}

S_Coord C_Coord::gridToScreen(S_Coord grid)
{
	C_Settings& settings=C_Locator::getSettings();
	S_Coord cameraPos = settings.getCameraPosition();
	S_Coord screen;
	screen.x = cameraPos.x + (grid.x - grid.y)* TILE_HALF_WIDTH;
	screen.y = (grid.y + grid.x) * TILE_HALF_HEIGHT - cameraPos.y;
	return screen;
}

void C_Coord::displayStatus()
{
	cout << "\tgrid: "<< m_this.grid.x << ":"<< m_this.grid.y;
	cout << "\tscreen: "<< m_this.screen.x << ":"<< m_this.screen.y;
	cout << "\tdelta: "<< m_this.delta.x << ":"<< m_this.delta.y << endl;
}

S_Coord C_Coord::getGrid()
{
	return m_this.grid;
}

int C_Coord::getXGrid() const
{
	return m_this.grid.x;
}

int C_Coord::getYGrid() const
{
	return m_this.grid.y;
}

S_Coord C_Coord::getScreen()
{
	S_Coord ret;
	ret.x = m_this.screen.x + m_this.delta.x;
	ret.y = m_this.screen.y + m_this.delta.y;
	return ret;
}


int C_Coord::getXScreen()
{
	return m_this.screen.x + m_this.delta.x;
}
int C_Coord::getYScreen()
{
	return m_this.screen.y + m_this.delta.y;
}

void C_Coord::centerOnTile()
{
	m_this.delta = {0,0};
}

int C_Coord::angleToDirection(double angle)
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

double C_Coord::directionToAngle(int direction)
{
	double  angle = 0;
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


void C_Coord::move(double angle, int speed)
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
			m_this.delta.x +=1;
			m_this.delta.y -=halfPixel;
			break;
		case SLOW:
			m_this.delta.x +=2;
			m_this.delta.y -=1;
			break;
		case NORMAL:
			m_this.delta.x +=3;
			m_this.delta.y -=(1 + halfPixel);
			break;
		case FAST:
			m_this.delta.x +=4;
			m_this.delta.y -=2;
			break;
		case VERY_FAST:
			m_this.delta.x +=5;
			m_this.delta.y -=(2 + halfPixel);
			break;
		}
	} else if(angle > 76.7 && angle <=103.3) {
		//cout << "NORTH_EAST" << endl;
		m_this.delta.x +=speed;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.x +=1;
			break;
		case SLOW:
			m_this.delta.x +=2;
			break;
		case NORMAL:
			m_this.delta.x +=3;
			break;
		case FAST:
			m_this.delta.x +=4;
			break;
		case VERY_FAST:
			m_this.delta.x -=5;
			break;
		}
	} else if (angle > 103.3 && angle <=135.0) {
		//cout << "EAST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.x +=1;
			m_this.delta.y +=halfPixel;
			break;
		case SLOW:
			m_this.delta.x +=2;
			m_this.delta.y +=1;
			break;
		case NORMAL:
			m_this.delta.x +=3;
			m_this.delta.y +=(1 + halfPixel);
			break;
		case FAST:
			m_this.delta.x +=4;
			m_this.delta.y +=2;
			break;
		case VERY_FAST:
			m_this.delta.x +=5;
			m_this.delta.y +=(2 + halfPixel);
			break;
		}
	}

	else if ((angle > 135.0 && angle <= 225.0)) {
		//cout << "SOUTH_EAST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.y +=halfPixel;
			break;
		case SLOW:
			m_this.delta.y +=1;
			break;
		case NORMAL:
			m_this.delta.y +=(1 + halfPixel);
			break;
		case FAST:
			m_this.delta.y +=2;
			break;
		case VERY_FAST:
			m_this.delta.y +=(2 + halfPixel);
			break;
		}
	} else if(angle > 225.0 && angle <= 256.7) {

		//cout << "SOUTH" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.x -=1;
			m_this.delta.y +=halfPixel;
			break;
		case SLOW:
			m_this.delta.x -=2;
			m_this.delta.y +=1;
			break;
		case NORMAL:
			m_this.delta.x -=3;
			m_this.delta.y +=(1 + halfPixel);
			break;
		case FAST:
			m_this.delta.x -=4;
			m_this.delta.y +=2;
			break;
		case VERY_FAST:
			m_this.delta.x -=5;
			m_this.delta.y +=(2 + halfPixel);
			break;
		}
	} else if(angle > 256.7 && angle <= 283.3) {
		//cout << "SOUTH_WEST" << endl;
		m_this.delta.x -=speed;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.x -=1;
			break;
		case SLOW:
			m_this.delta.x -=2;
			break;
		case NORMAL:
			m_this.delta.x -=3;
			break;
		case FAST:
			m_this.delta.x -=4;
			break;
		case VERY_FAST:
			m_this.delta.x -=5;
			break;
		}
	} else if(angle > 283.3 && angle <= 315.0) {
		//cout << "WEST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.x -=1;
			m_this.delta.y -=halfPixel;
			break;
		case SLOW:
			m_this.delta.x -=2;
			m_this.delta.y -=1;
			break;
		case NORMAL:
			m_this.delta.x -=3;
			m_this.delta.y -=(1 + halfPixel);
			break;
		case FAST:
			m_this.delta.x -=4;
			m_this.delta.y -=2;
			break;
		case VERY_FAST:
			m_this.delta.x -=5;
			m_this.delta.y -=(2 + halfPixel);
			break;
		}
	} else if((angle > 315.0 && angle <= 405.0) || (angle > -45.0 && angle <= 45.0)) {
		//cout << "NORTH_WEST" << endl;
		switch(speed) {
		case VERY_SLOW:
			m_this.delta.y -=halfPixel;
			break;
		case SLOW:
			m_this.delta.y -=1;
			break;
		case NORMAL:
			m_this.delta.y -=2;
			break;
		case FAST:
			m_this.delta.y -=3;
			break;
		case VERY_FAST:
			m_this.delta.y -=4;
			break;
		}
	}

	else {
		cout << "Angle is not between 0 and 360 : " << angle << endl;
	}

}


double C_Coord::atan2_360(int ab, int bc)
{
	double angle = atan2(ab,bc);
	angle = 180 - (angle *180/3.14159265359);
	if(angle < 0)
		angle +=360;
	else if(angle > 360)
		angle -=360;
	return angle;
}



void C_Coord::regenGridCoord()
{

	createCoordFromScreen(m_this.screen.x + m_this.delta.x, m_this.screen.y + m_this.delta.y);
}

void C_Coord::regenScreenCoord()
{
	m_this.screen = gridToScreen(m_this.grid);
}


bool C_Coord::closeToCenter(S_Coord grid, int px_length)
{
	C_CoordGrid tmp(grid);
	tmp.centerOnTile();
	S_Coord center = tmp.getScreen();
	int l = m_this.screen.x + m_this.delta.x - center.x;
	int h = m_this.screen.y + m_this.delta.y - center.y;
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
	m_this.grid = {coord.x,coord.y};
	m_this.screen = gridToScreen(m_this.grid);
	m_this.delta = {0,0};
	//cout << "grid to screen" << endl;
}

C_CoordGrid::C_CoordGrid(int x_grid, int y_grid ): C_Coord(x_grid, y_grid)
{
	C_Grid& grid= C_Locator::getGrid();
	int size = grid.getSize();
	if (x_grid < 0)
		x_grid = 0;
	if (y_grid < 0)
		y_grid = 0;
	if (x_grid > size)
		x_grid = size;
	if (y_grid > size)
		y_grid = size;

	m_this.grid = {x_grid,y_grid};
	m_this.screen = gridToScreen(m_this.grid);
	m_this.delta = {0,0};

}

C_CoordGrid::~C_CoordGrid()
{
};

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
	m_this.screen = {x_screen, y_screen};
	m_this.grid = screenToGrid(m_this.screen);
	m_this.screen = gridToScreen(m_this.grid);
	m_this.delta.x = x_screen - m_this.screen.x;
	m_this.delta.y = y_screen - m_this.screen.y;
}


C_CoordScreen::~C_CoordScreen()
{
};



