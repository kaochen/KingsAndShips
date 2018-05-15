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


#ifndef TOWERS_H
#define TOWERS_H

#include <iostream>
#include <string>
#include "weapons.h"
#include "shooter.h"
#include "surfaces.h"


class C_Towers: public C_Shooter
{
	public:
	//methods
	C_Towers();
	C_Towers(std::string name, int x_grid, int y_grid, int rank);

	virtual void play();
	void drag(S_Coord screen);

	protected:
	virtual void render(S_Coord screen);
	virtual void renderSelected();
	void renderSmoke();
	virtual void drawEllipse(int x,int y, int width, bool ok);
	void drawRhombus(int x, int y,int width, int alpha, bool ok);

	long m_lastSmokeTime;
	int m_smokeNbr;
	bool m_justAdded;
};


class C_ArcherTower: public C_Towers
{
	public:
		C_ArcherTower(int x_grid, int y_grid, int rank);
		virtual void render(S_Coord screen);
	protected:

};

class C_Turbine: public C_Towers
{
	public:
		C_Turbine(int x_grid, int y_grid, int rank);
		virtual void render(S_Coord screen);
	protected:

};




#endif
