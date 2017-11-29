#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define ASPECT_RATIO 9/16
#define FRAMERATE 30
#define TILE_HALF_HEIGHT 25
#define TILE_HALF_WIDTH 50
#define GRID_SIZE 32
#define BOAT_LIST_SIZE 4
#define MAX_LIFE 100
#define LAYER 2
#define SPRITE_SIZE 128

enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
enum GroundType {GROUND_01,GROUND_02};

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
};

#endif
