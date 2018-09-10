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
#include <SDL2_gfxPrimitives.h>

class C_Tab
{
	public:
  C_Tab();
  ~C_Tab(){};
  void displayTab(bool open, size_t nbr);
  protected:
  Sint16 *getVertex_X(Sint16 x,size_t nbr);
  Sint16 *getVertex_Y(Sint16 y);

	private:
  S_Coord m_screen /*!< top left corner of the tab*/;
  int m_height;
  int m_width;
  Sint16 m_tabSize;
};

#endif
