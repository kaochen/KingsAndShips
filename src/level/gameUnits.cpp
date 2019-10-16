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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gameUnits.h"
#include "grid.h"

#include "../locator.h"
#include "../texture.h"
#include "../message.h"

using namespace std;

//constructor

C_GameUnits::C_GameUnits(string name, int x_grid, int y_grid, int rank):
	m_name(name),
	m_type("unit"),
	m_rank(rank),
	m_health(100),
	m_max_health(100),
	m_coord(C_CoordGrid(x_grid,y_grid)),
	m_old_coord(C_CoordGrid(x_grid,y_grid)),
	m_direction(UNKNOWN),
	m_selected(false)
{
}

C_GameUnits::C_GameUnits(S_UnitModel model):
	m_name(model.name),
	m_type(model.type),
	m_rank(model.rank),
	m_health(model.health),
	m_max_health(model.health),
	m_coord(C_CoordGrid(model.coord)),
	m_old_coord(C_CoordGrid(model.coord)),
	m_direction(UNKNOWN),
	m_selected(false)
{

}



void C_GameUnits::displayStatus()
{
	C_Message::printM( "Name: " + m_name + " Life: " + to_string(m_health)  + " Rank : " + to_string(m_rank));
	m_coord.displayStatus();
}

void C_GameUnits::render(S_Coord screen)
{
	C_TextureList& t= C_Locator::getTextureList();
	string fileName = imageName(ALIVE,m_direction,0);
	t.renderTexture(fileName, screen.x,screen.y);
}

void C_GameUnits::receiveDamage(S_Weapon weapon)
{
	m_health -=weapon.damage;
	if (m_health < 0) {
		m_health = 0;
	}
}


int C_GameUnits::getDistance(int x, int y)
{
	int sideX = m_coord.getXScreen () - x;
	int sideY = m_coord.getYScreen () - y;
	int dist = sqrt(sideX*sideX + sideY*sideY);
	return dist;
}




void C_GameUnits::reverseSelectedStatus()
{
	if (m_selected == false) {
		m_selected =  true;
	} else
		m_selected =  false;
}

string C_GameUnits::imageName(int status,int direction,int imageNbr)
{
	string statusStr = "A";
	switch (status) {
	case ALIVE:
		statusStr = "A";
		break;
	case DEAD:
		statusStr = "D";
		break;
	}
	return m_type + "_" + to_string(m_rank) + "_"+ statusStr + "_"
		   + directionToStr(direction) + "_" + to_string(imageNbr) ;
}


string C_GameUnits::directionToStr(int intDirection)
{
	string direction = "EE";
	switch(intDirection) {
	case SOUTH:
		direction = "SS";
		break;
	case SOUTH_EAST:
		direction = "SE";
		break;
	case EAST:
		direction = "EE";
		break;
	case NORTH_EAST:
		direction = "NE";
		break;
	case NORTH:
		direction = "NN";
		break;
	case NORTH_WEST:
		direction = "NW";
		break;
	case WEST:
		direction = "WW";
		break;
	case SOUTH_WEST:
		direction = "SW";
		break;
	}
	return direction;
}

void C_GameUnits::changeState(std::string state){
	m_anim.reset(m_state);
	m_state = state;
	m_anim.start(m_state);
};


S_UnitModel C_GameUnits::getInfo(){
	S_UnitModel unit;
	unit.name = m_name;
	unit.type = m_type;
	unit.rank = m_rank;
	unit.health = m_health;
	unit.coord = m_coord.getGrid();
	unit.cost = 0;
	unit.speed = 0;
	unit.alive = true;
	unit.weapon.type = "NO_WEAPON";
	unit.weapon.damage = 0;
	unit.weapon.fireRate = 0;
	unit.weapon.fireRange = 0;
	unit.weapon.direction = WEST;
	return unit;
}

C_EmptyUnit::C_EmptyUnit():
	C_GameUnits("empty", 0,0,0)
{

}
