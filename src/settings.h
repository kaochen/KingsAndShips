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
#define GRID_SIZE 30
#define BOAT_LIST_SIZE 4
#define MAX_LIFE 100
#define LAYER 2

enum Direction {NORTH,SOUTH,EAST,WEST,UNKNOWN};

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

private:
	C_Set& operator= (const C_Set&){return *this;}
	C_Set (const C_Set&){}

	static C_Set m_instance;
	C_Set();
	~C_Set();

	int m_gridSize;
	int m_windowWidth;
	int m_windowHeight;
};

#endif
