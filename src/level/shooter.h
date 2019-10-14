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
#include "../wallet.h"
#include <vector>


class C_Shooter : public C_GameUnits {
public:
	C_Shooter(S_UnitModel model);
	virtual ~C_Shooter();

	virtual	void displayStatus();
	virtual void upgrade(S_UnitModel model);
	virtual void drag(S_Coord screen);
	virtual int getCost(){return m_cost;};
	virtual S_UnitModel getInfo();
	virtual void upgrade(bool upgrade){m_upgrade = upgrade;};
	virtual void renderWeapon();
protected:

	virtual bool shoot();
	virtual bool shoot(C_GameUnits* target);
	C_GameUnits*  searchNextTarget();

	virtual void kill();

	virtual void renderLifeBar(int x_screen, int y_screen);
	virtual void render(S_Coord screen);
	virtual	C_GameUnits* searchNextTarget(std::string type);
	virtual void shootTarget(C_GameUnits &target);
	virtual void drawEllipse(int x,int y, int size, bool ok);

	void changeState(std::string state);
	std::string m_state;

	C_Weapon *m_weapon;
	long m_lastShootTime;
	bool m_justAdded;

	int m_cost;
  	bool m_upgrade;
	std::vector <std::string> m_targetsTypes;

};

#endif
