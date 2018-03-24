#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>

#include "gameUnits.h"

enum layers{GROUND,DEAD,UNITS};

//singleton
class C_Grid
{
public:
	static	C_Grid& Instances();
  void reset();

	void renderLayer(int layer);

	void addANewBoat(int x, int y, int rank);
	void addANewTower(int type, int x, int y, int rank);
	void moveUnit(int x_from, int y_from, int x_dest, int y_dest);
	void moveToDead(int x_grid, int y_grid);

	C_GameUnits* getUnits(int x_grid,
	                     int y_grid);
	void setGround(int x, int y, int id);
	bool waterway(int x_grid, int y_grid);
	std::string getStrGround(int x, int y);
	bool isThisConstructible(S_Coord grid);
	bool isThisConstructible(int x_grid,int y_grid);

	void displayStatus();
	void playAllUnits();
	void deleteGrid();

	bool selectATower(C_Coord clic);

	C_GameUnits* getSelectedUnit();
  virtual bool mainEmpty(int x_grid, int y_grid, C_GameUnits* current);

protected:
	void unselectedAll(int x_grid, int y_grid);

  void darkenGround(int x_screen, int y_screen);

private:
	C_Grid& operator= (const C_Grid&){return *this;}
	C_Grid (const C_Grid&){}

	static C_Grid m_instance;
	C_Grid();
	~C_Grid();

	C_GameUnits::S_layer m_grid[GRID_SIZE][GRID_SIZE];
};

#endif
