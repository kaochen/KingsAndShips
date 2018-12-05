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

#include <cmath>

#include "weapons.h"
#include "../texture.h"

using namespace std;

C_Weapon::C_Weapon(S_Weapon model):
    m_weapon(model),
	m_x_screen(0),
	m_y_screen(0),
	m_shooting(false),
	m_lastShootTime(0),
	m_dist(80),
	m_angle(0.0)
{
	m_weapon.direction = EAST;
}

void C_Weapon::displayStatus() const
{
 	cout << "\t\t\tWeapon: " << m_weapon.type << " (Damage: "<< m_weapon.damage << ", firerate: " << m_weapon.fireRate << ")" << endl;
}


void C_Weapon::setShooting(bool status)
{
	m_shooting = status;
}


bool C_Weapon::shoot(C_GameUnits &shooter, C_GameUnits &target){
			int x_s_target = target.getXScreen();
			int y_s_target = target.getYScreen();
			int x_s_shooter = shooter.getXScreen();
			int y_s_shooter = shooter.getYScreen();
			int ab = x_s_target - x_s_shooter;
			int bc = y_s_target - y_s_shooter;
			int hyp = sqrt((ab*ab + bc*bc));
			hyp -= hyp*m_dist/100;
			double angle = atan2(ab,bc);
			int newA = hyp*sin(angle);
			int newB = hyp*cos(angle);
			m_angle = 180 - (angle *180/3.14159265359);
			if(m_angle < 0)
				m_angle +=360;

			C_Coord coord(1,1); //need a random one
			m_weapon.direction = coord.angleToDirection(m_angle);
			//cout << "angle: " << m_angle << ":" << m_weapon.direction << endl;

			m_x_screen = x_s_shooter + newA;
			m_y_screen = y_s_shooter + newB;
			m_dist -= 4;
			if (m_dist < 20){
				m_dist = 80;
				m_lastShootTime = SDL_GetTicks();
				return true;
				}
			return false;
}


void C_Weapon::render(){
		C_TextureList& t=C_TextureList::Instances();
		t.renderTextureEx("Weapons_arrow", m_x_screen,m_y_screen,m_angle, CENTER_TILE);
}


