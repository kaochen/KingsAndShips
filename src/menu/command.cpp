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
#include "../level/grid.h"
#include "../locator.h"

using namespace std;


void C_OpenCloseMenu::action()
{
	C_Message::printM("Send an openCloseMenu action\n");
	C_Menu& menu=C_Locator::getMenu();
	if(!menu.isOpen()){
		menu.openMainMenu();
	} else {
		menu.closeMainMenu();
	}
}

bool C_OpenCloseMenu::getBool()
{
	C_Menu& menu=C_Locator::getMenu();
	return menu.isOpen();
}

void C_OpenMenu::action()
{
	C_Message::printM("Send an openMenu action\n");
	C_Menu& menu=C_Locator::getMenu();
	menu.openMainMenu();
}

void C_CloseMenu::action()
{
	C_Message::printM("Send an closeMenu action\n");
	C_Menu& menu=C_Locator::getMenu();
	menu.closeMainMenu();
}

bool C_OpenMenu::getBool()
{
	C_Menu& menu=C_Locator::getMenu();
	return menu.isOpen();
}

void C_ChangeTab::action()
{
    std::string msg = "Send an ChangeTab action for "+ m_text +"\n";
	C_Message::printM(msg);
	C_Menu& menu=C_Locator::getMenu();
	menu.openTab(m_text);
}

void C_ChangeLevelLeft::action()
{
	C_Message::printM("Send an C_ChangeLevelLeft action\n");
	C_Menu& menu=C_Locator::getMenu();
	menu.go(GO_LEFT);
}

void C_ChangeLevelRight::action()
{
	C_Message::printM("Send an C_ChangeLevelRigth action\n");
	C_Menu& menu=C_Locator::getMenu();
	menu.go(GO_RIGHT);
}


void C_LoadALevel::action()
{
	C_Window& win=C_Locator::getWindow();
	win.loadLevel(m_nbr);
}

void C_CenterCamera::action()
{
	C_Settings& settings= C_Locator::getSettings();
	C_Grid& grid= C_Locator::getGrid();
	settings.cameraOnAPoint(grid.foundTown());
}

void C_Play::action()
{
	C_Settings& settings= C_Locator::getSettings();
	settings.setPlaying();

}

void C_UpgradeUnit::action()
{
	C_Grid& grid= C_Locator::getGrid();
	C_GameUnits * unit = grid.getSelected();
	if(unit != nullptr){
		grid.upgradeUnit(unit);
	} else {
		C_Message::printM("No unit is selected");
	}

}


void C_UpgradeUnit::actionHover(bool state)
{
	C_Grid& grid= C_Locator::getGrid();
	C_GameUnits * unit = grid.getSelected();
	if(unit != nullptr){
		if(state){
			unit->upgrade(true);
		} else {
			unit->upgrade(false);
		}
	}
}


void C_QuitProgram::action()
{
    C_Window& win=C_Locator::getWindow();
    win.quit();
}
