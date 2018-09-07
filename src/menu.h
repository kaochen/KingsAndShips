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
#include "menuItems.h"
#include "level/level.h"


class C_Menu
{
	public:
	static	C_Menu& Instances(){return m_instance;};
  void updateInfos(); /*!Create or update informations before render them on screen*/
	void render(); /*!Render the menu on screen*/
	C_MenuItem * getMenuItem(std::string name){	return m_menuItemsList[name];};
  void updateLevelInfos(int current_wave, int total_waves);
  void resetValues(); /*!reset values when change or reset the level*/

  protected:
  void updateDefenderStatus(); /*!Create or update defender informations*/
  void updateAttackerStatus(); /*!Create or update attacker informations*/
  void updateWalletStatus(); /*!Create or update wallet informations of the attacker*/
  std::string nbrToString(int nbr);/*!fill empty space when convert int to string, "10" become "  10"*/

	private:
	C_Menu& operator= (const C_Menu&){return *this;}
	C_Menu (const C_Menu&){}
	static C_Menu m_instance;
	C_Menu();
	~C_Menu();
  void popOutMenu();
  //information to display
  int m_current_wave;
  int m_total_waves;

  std::map<std::string, C_MenuItem*> m_menuItemsList;
};

#endif
