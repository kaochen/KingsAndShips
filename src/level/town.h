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


#ifndef TOWN_H
#define TOWN_H

#include <iostream>
#include <string>
#include "shooter.h"

class C_Town: public C_Shooter {
public:
	C_Town(S_UnitModel model);
protected:
	virtual void play();
	virtual void render(S_Coord screen);
private:
};

#endif
