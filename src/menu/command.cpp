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


#include "command.h"
#include "../message.h"
#include <SDL2/SDL.h>
#include "../window.h"
#include "menu.h"

using namespace std;

void C_OpenMenu::action(){
    C_Message m;
    m.printM("Send an openMenu action\n");
    C_Menu& menu=C_Menu::Instances();
    menu.openBottomMenu();
}

bool C_OpenMenu::getBool(){
    C_Menu& menu=C_Menu::Instances();
    return menu.isOpen();
}

void C_ChangeTab::action(){
    C_Message m;
    m.printM("Send an ChangeTab action\n");
    C_Menu& menu=C_Menu::Instances();
    menu.setTabNbr(m_nbr);
}

void C_LoadALevel::action(){
    C_Window& win=C_Window::Instances();
    win.loadLevel(m_nbr);
}

void C_CenterCamera::action(){
    C_Settings& settings=C_Settings::Instances();
    settings.centerCameraPosition();
}
