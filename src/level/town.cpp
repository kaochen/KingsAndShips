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


#include "town.h"

using namespace std;


C_Town::C_Town(S_UnitModel model):C_Shooter(model)
{
	m_targetsTypes.push_back("boat");
	m_canRotate = false;
	m_isBottomAnimated = false;
	m_renderDead = false;
	m_renderLifeBar = false;
}

std::string C_Town::imageName(int status,int direction,int imageNbr){
    (void)status;
    (void)direction;
    (void)imageNbr;
    return m_model.tileSource;
}

C_CastleTower::C_CastleTower(S_UnitModel model):C_Town(model)
{
	m_targetsTypes.push_back("boat");
	m_canRotate = false;
	m_isBottomAnimated = false;
	m_renderDead = false;
	m_renderLifeBar = false;
}

