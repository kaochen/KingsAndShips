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


#ifndef INVADERS_H
#define INVADERS_H

#include <iostream>
#include <string>
#include "shooter.h"

class C_invaders: public C_Shooter
{
	public:
	//methods
	C_invaders();
	C_invaders(int x_grid, int y_grid,int rank);
	~C_invaders();

	virtual void play();
	virtual void move();

	virtual void receiveDamage(S_Weapon weapon);

	protected:
	virtual void render(S_Coord screen);
  virtual void recalcPath(S_Coord dest);
  virtual int calcSpeed();

	//attributs
	bool m_moving;
	C_Path* m_C_Path;
	int m_speed;
  int m_speedImpact;
  int m_speedImpactLoop;
  int m_countStop;
  C_AnimTime* m_animDirection;
};

#endif
