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


#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <iostream>
#include <string>
#include "gameUnits.h"
#include "surfaces.h"
#include "grid.h"

class C_Landscape
{
	public:
    C_Landscape();
    ~C_Landscape();
    void display();
	protected:
	private:
  //methods
  void renderWater(int direction);
  int waterDirection();

  //attributes
  S_Coord m_waterDrift;
  int m_waterDirection;
  C_AnimTime* m_animWater;
};

class C_Decors : public C_GameUnits
{
	public:
	  C_Decors(std::string name, int x_grid, int y_grid);
  	virtual void play();
	  virtual void move();
  	virtual void shoot(std::string type[MAX_TARGETS], int nbrofTargets);
    virtual void render(S_Coord screen);
  protected:
  private:
};
#endif
