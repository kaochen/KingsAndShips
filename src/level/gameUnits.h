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

/*!
 *  \file gameUnits.h
 *  \author kao_chen
 */

#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <string>

#include "../settings.h"
#include "../coord.h"
#include "../time.h"
#include "pathfinding.h"

struct S_Unit {
	std::string name;
	S_Coord coord;
};

struct S_Weapon {
	std::string type;
	int damage;
	int speedImpact;
	int fireRate;
	int fireRange;
	int direction;
};

struct S_UnitModel {
	std::string name;
	std::string type;
	int rank;
	int health;
	S_Coord coord;
	int cost;
	int speed;
	S_Weapon weapon;
	bool alive;
};

/*! \class C_GameUnits
 * \brief mother class for all isometric elements (units) of the game, like boats or trees
 */

class C_GameUnits {
public:
	C_GameUnits(std::string name,
				int x_grid,
				int y_grid,
				int rank);
	C_GameUnits(S_UnitModel model);

	virtual ~C_GameUnits();
	virtual void play() {};
	virtual void move() {};
	virtual void upgrade(S_UnitModel model) {std::cout << "model " << model.name;};
	virtual void drag(S_Coord screen)
	{
		std::cout << "drag() "<< screen.x;
	};

	virtual C_GameUnits * getUnit()
	{
		return this;
	};
	virtual std::string getName() const
	{
		return m_name;
	};
	virtual std::string getType() const
	{
		return m_type;
	};

	virtual int getRank() const {return m_rank;};
	virtual void displayStatus() const;
	virtual void render(S_Coord screen);
	virtual void render()
	{
		std::cout << ";)";
	};

	//shoot
	virtual void shoot(std::string type[MAX_TARGETS], int nbrofTargets);
	virtual void receiveDamage(S_Weapon weapon);

	//Coord
	virtual int getXGrid() const
	{
		return m_coord->getXGrid ();
	};
	virtual int getYGrid() const
	{
		return m_coord->getYGrid ();
	};
	virtual int getXScreen() const
	{
		return m_coord->getXScreen ();
	};
	virtual int getYScreen() const
	{
		return m_coord->getYScreen ();
	};
	virtual S_Coord getScreen() const
	{
		return m_coord->getScreen ();
	};
	virtual void regendScreenCoord()
	{
		m_coord->regenScreenCoord();
	};
	//selected or not
	virtual bool getSelectedStatus() const
	{
		return m_selected;
	};
	virtual void setSelectedStatus(bool status)
	{
		m_selected = status;
	};
	virtual void reverseSelectedStatus();

	//alive or dead
	virtual int getHealth() const
	{
		return m_health;
	}

	//wallet
	virtual int getCost(){return 0;};

	virtual S_UnitModel getInfo();

protected:
	//alive or dead
	virtual bool alive() const
	{
		return m_health>0;
	};
	virtual void kill() {};

	virtual int getDistance(int x, int y) const;
	virtual std::string imageName(int status,int direction,int imageNbr);
	virtual std::string directionToStr(int intDirection);

	//attibuts
	std::string m_name;
	std::string m_type;
	int m_rank;

	int m_health;
	int m_max_health;

	//Coord
	C_Coord* m_coord;
	C_Coord* m_old_coord;
	int m_direction; /*!< Orientation of the unit, south, north east...*/

	//Store time for animation
	C_AnimTime* m_animation[MAX_ANIM];

	bool m_selected;
};

class C_EmptyUnit: public C_GameUnits {
public:
	C_EmptyUnit();
	virtual void render(S_Coord grid)
	{
		if(grid.x == 0) {}
	};
	virtual void render() {};
};

#endif
