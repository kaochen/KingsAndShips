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

//describe boat class

#ifndef BOAT_H
#define BOAT_H

#include <iostream>
#include <string>
#include "shooter.h"

#include "../coord.h"
#include "../texture.h"

class C_Boat: public C_Shooter {
public:
	//methods
	C_Boat(S_UnitModel model);
	~C_Boat();

	virtual void play();
protected:
	virtual void render(S_Coord screen);
	virtual void recalcPath(S_Coord dest);
	virtual void move();
	virtual float calcAngle(C_Coord nextStep);
	virtual void kill();

	//attributs
	C_Path* m_C_Path;
	int m_speed;
	int m_countRegenPath;
	S_Coord m_step;
};

#endif
