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

#include "../texture.h"
#include "../message.h"

using namespace std;

//constructor

C_GameUnits::C_GameUnits(string name, int x_grid, int y_grid, int rank):
	m_name(name),
	m_rank(rank),
	m_life(MAX_LIFE),
	m_deadImageName("boat_01_Dead.png"),
	m_y_center_offset(0),
	m_strDirection("EE"),
	m_direction(UNKNOWN),
	m_selected(false)
{

	for (int i = 0; i < MAX_ANIM; i++){
		m_animation[i]= new C_AnimTime();
	}
	C_Message m;
    string message = "Add new unit: " + m_name +" life: "+ to_string(m_life) + " rank: "+ to_string(m_rank);
	m.printM(message);
	S_Coord coord;
	coord.x = x_grid;
	coord.y = y_grid;
	m_coord = new C_CoordGrid(coord);
	m_old_coord = new C_CoordGrid(coord);
	m_coord->displayStatus();
}

//delete
C_GameUnits::~C_GameUnits()
{
    for (int i = 0; i < MAX_ANIM; i++){
		delete m_animation[i];
	}
	delete m_coord;
	delete m_old_coord;
}


void C_GameUnits::displayStatus() const
{
    C_Message m;
    m.printM( "Name: " + m_name + " Life: " + to_string(m_life)  + " Rank : " + to_string(m_rank));
 	m_coord->displayStatus();
}

void C_GameUnits::render(S_Coord screen){
	C_TextureList& t=C_TextureList::Instances();
    string fileName = imageName(ALIVE,m_direction,0);
	t.renderTexture(fileName, screen.x,screen.y + m_y_center_offset);
}

void C_GameUnits::receiveDamage(S_Weapon weapon)
{
	m_life -=weapon.damage;
	if (m_life < 0)
	{
		m_life = 0;
	}
}


int C_GameUnits::getDistance(int x, int y) const
{
	int sideX = m_coord->getXScreen () - x;
	int sideY = m_coord->getYScreen () - y;
	int dist = sqrt(sideX*sideX + sideY*sideY);
	return dist;
}

void C_GameUnits::kill()
{
	C_Grid& grid=C_Grid::Instances();
	C_Message m;
	m.printM("kill " + m_name + " from:"+ to_string(m_coord->getXGrid ())
	            + ":" + to_string(m_coord->getYGrid ()) + "\n");
 	grid.moveToDead(m_coord->getXGrid (), m_coord->getYGrid ());
}



void C_GameUnits::reverseSelectedStatus()
{
	if (m_selected == false){
		m_selected =  true;
	}
	else
		m_selected =  false;
}

string C_GameUnits::imageName(int status,int direction,int imageNbr){
    string statusStr = "A";
    switch (status){
        case ALIVE:
            statusStr = "A";
            break;
        case DEAD:
            statusStr = "D";
            break;
    }
    return m_name + "_" + to_string(m_rank) + "_"+ statusStr + "_"
            + directionToStr(direction) + "_" + to_string(imageNbr) ;
}


string C_GameUnits::directionToStr(int intDirection){
string direction = "EE";
		switch(intDirection){
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
