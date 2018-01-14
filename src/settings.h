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

struct S_Coord{
	int x;
	int y;
};
struct S_NodeCoord{
	S_Coord screen;
	S_Coord grid;
};

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

enum coordinates{SCREEN,GRID};

class C_Coord
{
public:
	C_Coord(int x_grid, int y_grid);
	C_Coord(S_Coord coord);
	virtual ~C_Coord();
	virtual void displayStatus();
	virtual S_Coord getGrid();
	virtual int getXGrid();
	virtual int getYGrid();
	virtual S_Coord getScreen();
	virtual int getXScreen();
	virtual int getYScreen();

protected:
	S_Coord screenToGrid(S_Coord screen);
	S_Coord gridToScreen(S_Coord grid);
	S_NodeCoord m_this;
};


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
