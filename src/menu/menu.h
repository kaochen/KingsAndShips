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


#ifndef MENU_H
#define MENU_H

#include <map>
#include <string>
#include <vector>
#include "../locator.h"
#include "../level/level.h"
#include "menuItems.h"
#include "popup.h"
#include "tab.h"

class C_MenuItem;


class C_Menu {
public:
	C_Menu();
	virtual ~C_Menu();
	void updateInfos(); /*!Create or update informations before render them on screen*/
	void render(); /*!Render the menu on screen*/
	C_MenuItem * getMenuItem(std::string name){return m_menuItemsList[name];};
	void updateLevelInfos(int current_wave, int total_waves);
	void resetValues(); /*!reset values when change or reset the level*/
	void menuBanner();

 	void updateUpgradeButtonsStatus();
	//commands
	void openBottomMenu();
	void openEndLevelMenu(int status);
	bool isOpen(){return m_bottomMenuOpen;};
	void displayBottomMenu();
	std::vector<std::string> getMenuItemsList();
	void setTabNbr(int nbr);
protected:
	void updateDefenderStatus(); /*!Create or update defender informations*/
	void updateAttackerStatus(); /*!Create or update attacker informations*/
	void updateWalletStatus(); /*!Create or update wallet informations of the attacker*/
	std::string nbrToString(int nbr);/*!fill empty space when convert int to string, "10" become "  10"*/

private:

	void bottomButton(const std::string &name,S_Coord screen);/*!setup button for the bottom line of buttons*/
	void bottomButtonsLine(S_Coord screen);
	std::string tabName(int nbr){
		return "tab" + std::to_string(nbr) + "_Flag";};
	//information to display
	int m_current_wave;
	int m_total_waves;
	bool m_bottomMenuOpen;
	std::map<std::string, C_MenuItem*> m_menuItemsList;
	std::vector<C_Tab *> m_tabs;
	int m_currentTab;

	C_EndLevelMenu* m_endLevelMenu;
	bool m_endLevelMenuOpen;
};

#endif
