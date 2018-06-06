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


#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define ASPECT_RATIO 9/16
#define FRAMERATE 30
#define TILE_HALF_HEIGHT 24
#define TILE_HALF_WIDTH 48
#define GRID_SIZE 32
#define BOAT_LIST_SIZE 4
#define MAX_LIFE 100
#define LAYER 2
#define SPRITE_SIZE 128
#define MAX_TARGETS 3

struct S_Coord{
	int x;
	int y;
};
struct S_NodeCoord{
	S_Coord screen;
	S_Coord grid;
};

struct S_Weapon{
	int damage;
	int speedImpact;
	int fireRate;
	int fireRange;
  std::string direction;
};

enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
enum GroundType {GROUND_01,GROUND_02};
enum Speed {VERY_SLOW,SLOW, NORMAL,FAST,VERY_FAST};

//singleton
class C_Set
{
public:
	static	C_Set& Instances();
	int getWindowWidth();
	int getWindowHeight();
	int getGridSize();
	int getGridWidth();
	int getGridHeight();
	int getGridNbrOfLine();
	int getGridNbrOfRow();
	int getGridFirstTileX();
	int getGridFirstTileY();

	void setDebugMode();
	bool getDebugMode();
  void setDebugPathMode();
  bool getDebugPathMode();

	void displayDebugMode();

private:
	C_Set& operator= (const C_Set&){return *this;}
	C_Set (const C_Set&){}

	static C_Set m_instance;
	C_Set();
	~C_Set();

	int m_gridSize;
	int m_windowWidth;
	int m_windowHeight;
	int m_gridNbrOfLine; //nbr of tiles in a line
	int m_gridNbrOfRow; //nbr of tiles in a row
	int m_first_tile_x; //first tile to display
	int m_first_tile_y;
	bool m_debugMode;
  bool m_debugPath;
};

#endif
