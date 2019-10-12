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


#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "gameUnits.h"


class C_Weapon {
public:
	C_Weapon(S_Weapon spec);
	~C_Weapon() {};

	void displayStatus() const;
	S_Weapon getWeaponInfo(){return m_weapon;};
	int getDamage(){return m_weapon.damage;};
	int getFireRate(){return m_weapon.fireRate;};
	int getFireRange(){return m_weapon.fireRange;};
	bool getShooting(){return m_shooting;};

	long getLastShootTime(){return m_lastShootTime;};
	void setShooting(bool status);
	bool shoot(C_GameUnits &shooter, C_GameUnits &target);
	void render();

	void updateDirection(C_GameUnits &shooter, C_GameUnits &target);
private:
	int yOffset(int distFromTarget);
	float angleOffset(int distFromTarget, float angle);
	S_Weapon m_weapon;
	int m_x_screen;
	int m_y_screen;
	bool m_shooting;
	long m_lastShootTime;
	int m_dist;
	double m_angle;
};


#endif
