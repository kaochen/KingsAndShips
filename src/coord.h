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


#ifndef COORD_H
#define COORD_H

#include <iostream>
#include <string>
#include "settings.h"


enum coordinates {SCREEN,GRID};

class C_Coord {
public:
	C_Coord(int x_grid, int y_grid);
	C_Coord(S_Coord coord);
	C_Coord(const C_Coord &a);
	virtual ~C_Coord(){};
	virtual bool isEqual(C_Coord const &b) const;
    void copy(C_Coord const &a);
    void operator=(C_Coord const &a);
	virtual bool onScreen(); /*!< To know if an object is visible on screen and need render*/
	virtual void displayStatus();
	virtual S_Coord getGrid();
	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual S_Coord getScreen();
	virtual int getXScreen();
	virtual int getYScreen();
	virtual void centerOnTile();
	virtual int angleToDirection(float const &angle);
	virtual float directionToAngle(int const direction);

	virtual void reverseDelta(){m_delta.x *=(-1); m_delta.y *=(-1);};
	virtual void refreshGrid();
	virtual void move(float const &angle, int speed);

	virtual float atan2_360(int const &ab, int const &bc);
	virtual bool closeToCenter(S_Coord grid, int step_nbr);
	virtual bool atCenter(S_Coord destGrid);
	virtual int guessADirection(S_Coord start,S_Coord end);

protected:
	void createCoordFromScreen(int x_screen, int y_screen);
	S_Coord screenToGrid(S_Coord screen);
	S_Coord gridToScreen(S_Coord grid);
	S_Coord m_grid;
	S_Coord m_delta;
};

bool operator==(C_Coord const &a,C_Coord const &b);
bool operator!=(C_Coord const &a,C_Coord const &b);

class C_CoordGrid: public C_Coord {
public:
	C_CoordGrid(S_Coord coord);
	C_CoordGrid(int x_grid, int y_grid);
};

class C_CoordScreen: public C_Coord {
public:
	C_CoordScreen(S_Coord coord);
	C_CoordScreen(int x_screen, int y_screen);
};
#endif
