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


enum coordinates{SCREEN,GRID};

class C_Coord
{
public:
	C_Coord(int x_grid, int y_grid);
	C_Coord(S_Coord coord);
	C_Coord(const C_Coord &a);
	virtual ~C_Coord();
	virtual bool isEqual(C_Coord const &b) const;
	virtual void applyOffset(S_Coord offset);
  virtual bool onScreen(); /*!< To know if an object is visible on screen and need render*/
	virtual void displayStatus();
	virtual S_Coord getGrid();
	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual S_Coord getScreen();
	virtual void updateScreen(S_Coord new_screen);
	virtual int getXScreen();
	virtual int getYScreen();
	virtual void centerOnTile();
	virtual int angleToDirection(double angle);
  virtual double directionToAngle(int direction);
	virtual void move(double angle, int speed);
	virtual void regenGridCoord();
  virtual void regenScreenCoord();
  virtual double atan2_360(int ab, int bc);
	virtual bool closeToCenter(S_Coord grid, int px_length);
  virtual int guessADirection(S_Coord start,S_Coord end);

protected:
	S_Coord screenToGrid(S_Coord screen);
	S_Coord gridToScreen(S_Coord grid);
	S_NodeCoord m_this;
};

	bool operator==(C_Coord const &a,C_Coord const &b);
	bool operator!=(C_Coord const &a,C_Coord const &b);

class C_CoordGrid: public C_Coord
{
public:
	C_CoordGrid(S_Coord coord);
	C_CoordGrid(int x_grid, int y_grid);
	virtual ~C_CoordGrid();
};

class C_CoordScreen: public C_Coord
{
public:
	C_CoordScreen(S_Coord coord);
	C_CoordScreen(int x_screen, int y_screen);
	virtual ~C_CoordScreen();
};
#endif
