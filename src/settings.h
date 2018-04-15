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
	bool getDebugModeStatus();
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
};

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
	virtual void move(double angle, int speed);
	virtual void regenGridCoord();
	virtual bool closeToCenter(S_Coord grid);

protected:
	S_Coord screenToGrid(S_Coord screen);
	S_Coord gridToScreen(S_Coord grid);
	S_NodeCoord m_this;
};

	bool operator==(C_Coord const &a,C_Coord const &b);

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
