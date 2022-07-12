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


#include "locator.h"

using namespace std;

C_Time* C_Locator::m_time = nullptr;
C_Menu* C_Locator::m_menu = nullptr;
C_TextureList* C_Locator::m_texturelist = nullptr;
C_Window* C_Locator::m_window = nullptr;
C_Wallet* C_Locator::m_wallet = nullptr;
C_Grid* C_Locator::m_grid = nullptr;
C_Settings* C_Locator::m_settings = nullptr;

void C_Locator::deleteAllService(){
    if(m_window){delete m_window;};
    if(m_menu){delete m_menu;};
    if(m_wallet){delete m_wallet;};
    if(m_grid){delete m_grid;};
    if(m_texturelist){delete m_texturelist;};
    //if(m_time){delete m_time;};
    if(m_settings){delete m_settings;};
}

