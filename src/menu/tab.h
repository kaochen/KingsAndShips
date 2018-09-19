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


#ifndef TAB_H
#define TAB_H
#include "../settings.h"
#include "menuItems.h"
#include <string>
#include <map>
#include <SDL2_gfxPrimitives.h>

class C_Tab
{
	public:
  C_Tab(std::string title);
  virtual ~C_Tab(){};
  virtual void displayTab(bool open, size_t nbr);
  virtual std::string getName(){return m_name;};
  virtual std::string getTitle(){return m_title;};
  virtual std::map<std::string, C_MenuItem*> getItemList(){return m_itemsList;};
  virtual std::vector<std::string> getListOfVisibleItems();

  protected:
  virtual void focusTab(size_t nbr);
  virtual void drawAseparator(Sint16 x, Sint16 y, Sint16 width);

	protected:
  static int m_id;
  std::string m_name;
  std::string m_title;
  S_Coord m_screen /*!< top left corner of the tab*/;
  int m_height;
  int m_width;
  Sint16 m_tabSize;
  std::map<std::string, C_MenuItem*> m_itemsList;
};

class C_Tab_Settings : public C_Tab
{
public:
  C_Tab_Settings(std::string title);
};

#endif
