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

//gameUnits that can shoot

#ifndef SHOOTER_H
#define SHOOTER_H

#include <iostream>
#include <string>
#include "gameUnits.h"
#include "weapons.h"
#include "../time.h"

class C_Shooter : public C_GameUnits
{
	public:
	C_Shooter(std::string name, int x_grid, int y_grid, int rank);
	virtual ~C_Shooter();

	virtual	void displayStatus() const;
	virtual void shoot(std::string type[MAX_TARGETS], int nbrofTargets);
	virtual void move();
	virtual void drag(S_Coord screen, bool water);
  virtual int getCost(){return m_cost;};
	protected:

	virtual void renderLifeBar(int x_screen, int y_screen);
	virtual void render(S_Coord screen);
	virtual	C_GameUnits* searchNextTarget(std::string type);
	virtual void shootTarget(C_GameUnits &target);
  virtual void drawEllipse(int x,int y, int width, bool ok);
	virtual void drawRhombus(int x, int y,int width, int alpha, bool ok);

	C_Weapon *m_weapon;
	long m_lastShootTime;
  bool m_justAdded;

  int m_cost;
};

#endif
