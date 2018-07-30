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

void C_ZLayer::cliStatus(){
    cout << m_x_grid << ":"<< m_y_grid << " ";
}
