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


#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "time.h"
#include "settings.h"

#include "pathfinding.h"
#include <string>
class C_GameUnits
{
	public:
	struct S_layer{
		C_GameUnits * main;
		C_GameUnits * dead;
		bool path;
		std::string str_ground;
		bool plot;
		bool water;
    bool town;

	};
	//methods
	C_GameUnits(std::string name,
		     int x_grid,
		     int y_grid,
		     int rank);

	virtual ~C_GameUnits();
	virtual void play() = 0;
	virtual void move() = 0;

	virtual std::string getName() const;
	virtual void displayStatus() const;
	virtual void render(S_Coord screen);
	virtual void shoot(std::string type[MAX_TARGETS], int nbrofTargets) =0;
	virtual void receiveDamage(S_Weapon weapon);

	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual int getXScreen() const;
	virtual int getYScreen() const;

	virtual int getLife() const {return m_life;}

	virtual bool getSelectedStatus() const;
	virtual void setSelectedStatus(bool status);
	virtual void reverseSelectedStatus();

	virtual C_GameUnits * getUnit();
  std::string getDeadImageName() const;

	protected:
	virtual bool alive() const;
	virtual void kill();
	virtual int getDistance(int x, int y) const;

	//attibuts
	std::string m_name;
	int m_life;
	int m_rank;

	C_Coord* m_coord;
	C_Coord* m_old_coord;
	int m_y_center_offset;
	std::string m_strDirection;
	//Store time for animation
	C_AnimTime* m_animation[MAX_ANIM];

	int m_direction;
	int m_lastDirection;
	bool m_selected;
  std::string m_deadImageName;
};

#endif
