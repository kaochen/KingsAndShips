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


#include "barricade.h"
#include "../texture.h"

using namespace std;


C_Barricade::C_Barricade( int x_grid,
                 int y_grid,int rank):C_Shooter("barricade", x_grid, y_grid, rank)
{
	m_weapon = new C_Weapon("NONE",0,0,0,0);
}

void C_Barricade::play(){
	if(!this->alive())
		this->kill();
}

void C_Barricade::render(S_Coord screen){
    if(alive()){
	    int	imageNbr = m_animation[MAIN_ANIM]->getLoopAnimNbr(0,10,100);
	    string fileName = m_name+"_"+to_string(m_rank)+"_A_" + to_string(imageNbr);
	    //cout << "image name is "<< fileName << endl;
	    C_TextureList& t=C_TextureList::Instances();
	    t.renderTexture(fileName, screen.x,screen.y + m_y_center_offset);
	    renderLifeBar(screen.x, screen.y);
	}
}
