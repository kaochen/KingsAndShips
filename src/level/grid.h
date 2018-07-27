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

/*The grid is a 2D array to store and retreive easely each game unit.
The grid store S_Layer structure where you can found C_GameUnits in it.
The grid size is fixed by GRID_SIZE m_grid[GRID_SIZE][GRID_SIZE]
You can use x+1 to find a unit close to another one m_grid[x+1][y]*/


#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>

#include "gameUnits.h"
#include "level.h"
#include "../coord.h"

enum layers{GROUND,GRAVEYARD,UNITS};

//singleton
class C_Grid
{
public:
	static	C_Grid& Instances() {return m_instance;};
  void reset();

	void renderLayer(int layer);

	void addANewBoat(int x, int y, int rank,C_Wave* parent);
	void addANewTower(int type, int x, int y, int rank);
	void moveUnit(int x_from, int y_from, int x_dest, int y_dest);
	void moveToDead(int x_grid, int y_grid);

	C_GameUnits* getUnits(int x_grid, int y_grid) {	return m_grid[x_grid][y_grid].main;};
	void setGround(int x, int y, int id);
	void setDecors(int x, int y, int id);
	bool waterway(int x_grid, int y_grid){return m_grid[x_grid][y_grid].water;};
	std::string getStrGround(int x, int y){return m_grid[x][y].str_ground;};
	bool isThisConstructible(S_Coord grid);
	bool isThisConstructible(int x_grid,int y_grid);

	void displayStatus();
	void playAllUnits();
	void deleteGrid();

	bool selectATower(C_Coord clic);

	C_GameUnits* getSelectedUnit();
  virtual bool mainEmpty(int x_grid, int y_grid, C_GameUnits* current);
  virtual bool mainEmpty(int x_grid, int y_grid);
  virtual bool boatInMain(int x_grid, int y_grid);


  void setTown(int x_grid, int y_grid);
  S_Coord foundTown();
  int getAllTownsLifeLevel();

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
