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
#include "../locator.h"

using namespace std;


C_Barricade::C_Barricade(S_UnitModel model):C_Shooter(model)
{
	m_anim.add(C_Anim("Waiting",0,10,100));
	m_isBottomAnimated = true;
	m_canRotate = false;
	m_renderDead = false;
}

void C_Barricade::play()
{
	if(alive()){
		if(m_state == "Waiting"){
			if(m_anim.end(m_state)){
				changeState("Waiting");
			}
		}
		m_anim.get(m_state).play();
	} else {
		kill();
	}
}

