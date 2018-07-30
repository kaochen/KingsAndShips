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



#include "zlayer.h"



using namespace std;


C_ZLayer::C_ZLayer(int x_grid, int y_grid):
    m_x_grid(x_grid),
    m_y_grid(y_grid),
    m_field(nullptr),
    m_grave(nullptr),
    m_ground(nullptr)
{
    cliStatus();
}

C_ZLayer::~C_ZLayer()
{
    delete m_field;
    delete m_grave;
    delete m_ground;
}

void C_ZLayer::set(int layer, C_GameUnits * unit){

switch(layer){
	case GROUND :
        m_ground = unit;
	 break;
	 case GRAVEYARD :
        m_grave = unit;
	 break;
	 case  FIELD:
	    m_field = unit;
	 break;
	}
}


C_GameUnits* C_ZLayer::get(int layer){
    C_GameUnits * unit = nullptr;
    switch(layer){
	    case GROUND :
            unit = m_ground;
	     break;
	     case GRAVEYARD :
            unit = m_grave;
	     break;
	     case  FIELD:
	        unit = m_field;
	     break;
	}
	return unit;
 }

  void C_ZLayer::del(int layer){
    switch(layer){
	    case GROUND :
            delete m_ground;
	     break;
	     case GRAVEYARD :
            delete m_grave;
	     break;
	     case  FIELD:
	        delete m_field;
	     break;
	}
  }

void C_ZLayer::delAll(){
        delete m_ground;
        delete m_grave;
        delete m_field;
}



void C_ZLayer::cliStatus(){
    cout << m_x_grid << ":"<< m_y_grid << " ";
}



