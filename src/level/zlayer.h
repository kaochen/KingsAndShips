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


#ifndef ZLAYER_H
#define ZLAYER_H

#include <iostream>

#include "gameUnits.h"

class C_ZLayer
{
public:
  C_ZLayer();
  ~C_ZLayer();
private:
  C_GameUnits * m_field; //main arena.
  C_GameUnits * m_grave; //when unit is dead
  C_GameUnits * m_ground; //land and water
};

#endif
