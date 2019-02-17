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
#include "../menu/popup.h"

class C_Shooter : public C_GameUnits {
public:
	C_Shooter(S_UnitModel model);
	virtual ~C_Shooter();

	virtual	void displayStatus();
	virtual void shoot(std::string type[MAX_TARGETS], int nbrofTargets);
	virtual void kill();
	virtual void move();
	virtual void upgrade(S_UnitModel model);
	virtual void drag(S_Coord screen);
	virtual int getCost(){return m_cost;};
	virtual S_UnitModel getInfo();
	virtual void sendToPopup(std::string message){m_popup.setMode(message);};
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
	C_Popup m_popup;
};

#endif
