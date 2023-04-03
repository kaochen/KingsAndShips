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
#include "tab.h"

class C_MenuItem;


class C_Menu {
public:
	C_Menu();
	virtual ~C_Menu();
	void refresh(); /*!Create or update informations before render them on screen*/
	void render(); /*!Render the menu on screen*/
	std::vector<C_MenuItem *> getMenuItems();
	void menuBanner();
    C_Frame* getFrame(std::string name);
    S_Coord getCoord(std::string name);
    void openTab(std::string name);
	//commands
	void openMainMenu();
	void closeMainMenu();
	bool isOpen(){return getFrame("mainMenu")->getOpen();};
    void go(int direction);
    void Nbr(int nbr);
protected:
private:
	//information to display
    std::vector <C_Frame*> m_frames;
};

#endif
