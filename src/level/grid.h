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

/*The grid is a 2D vector to store and retreive easely each game unit*.

The grid store C_ZLayer class where you can found C_GameUnits in it.
C_Zlayer is like the z axis for the 2D
You can access to a gameUnit from a specific layer with m_vgrid[x][y].get(layer).
You can use x+1 to find a unit close to another one m_vgrid[x+1][y]*/


#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>

#include "zlayer.h"
#include "gameUnits.h"
#include "level.h"
#include "../coord.h"


//singleton
class C_Grid
{
public:
	static	C_Grid& Instances() {return m_instance;};
  void reset(int size);
	void renderLayer(int layer);
  int getSize(){return m_size;};
	void addANewBoat(int x, int y, int rank,C_Wave* parent);
  int addUnit(std::string &type, int x_grid, int y_grid, int rank);
	void moveUnit(int x_from, int y_from, int x_dest, int y_dest);
	void moveToDead(int x_grid, int y_grid);

	C_GameUnits* getUnits(int x_grid, int y_grid);
	void setGround(int x, int y, int id);
	void setDecors(int x, int y, int id);
	bool waterway(int x_grid, int y_grid);
  bool testBarricade(int x_grid, int y_grid);
	bool isThisConstructible(S_Coord grid);
	bool isThisConstructible(int x_grid,int y_grid);
  int size(){return m_vgrid.size() - 2;};
	void displayStatus();
	void playAllUnits(int layer);
	void deleteGrid();

	bool selectATower(C_Coord clic);

  virtual bool mainEmpty(int x_grid, int y_grid, C_GameUnits* current);
  virtual bool mainEmpty(int x_grid, int y_grid);
  virtual std::string getName(int layer, int x_grid, int y_grid);


  void setTown(int x_grid, int y_grid);
  S_Coord foundTown();
  int getAllTownsLifeLevel();

protected:
	void unselectedAll(int x_grid, int y_grid);
  void createAnEmptyGrid(int size);


private:
	C_Grid& operator= (const C_Grid&){return *this;}
	C_Grid (const C_Grid&){}

	static C_Grid m_instance;
	C_Grid();
	~C_Grid();

  std::vector < std::vector <C_ZLayer> > m_vgrid;
  int m_size;
};

#endif
