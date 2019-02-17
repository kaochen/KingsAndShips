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


#ifndef ZLAYER_H
#define ZLAYER_H

#include <iostream>

#include "gameUnits.h"
#include "../coord.h"

enum zlayers {GROUND,GRAVEYARD,FIELD};

class C_ZLayer {
public:
	C_ZLayer(int x_grid, int y_grid);
	~C_ZLayer();
	void set(int layer, C_GameUnits * unit);
	C_GameUnits* get(int layer);
	void del(int layer);
	void delAll();
	bool play(int layer);
	bool render(int layer);
private:
	void cliStatus(); //print basic status in the CLI

	//attributs
	C_Coord m_coord;
	C_GameUnits * m_field; //main arena.
	C_GameUnits * m_grave; //when unit is dead
	C_GameUnits * m_ground; //land and water

};

#endif
