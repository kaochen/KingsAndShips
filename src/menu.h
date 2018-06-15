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


class C_Menu
{
	public:
	static	C_Menu& Instances();
	void render();
	C_MenuItem * getMenuItem(int button);
	int getXScreen();
  void updateLevelInfos(int current_wave, int total_waves);
  void resetValues();

  	protected:
	void drawBackground();

	private:
	C_Menu& operator= (const C_Menu&){return *this;}
	C_Menu (const C_Menu&){}
	static C_Menu m_instance;
	C_Menu();
	~C_Menu();

	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
	int m_button_count;

  //information to display
  int m_current_wave;
  int m_total_waves;

	std::map<int,C_MenuItem*> m_map_menuItems;
};

#endif
