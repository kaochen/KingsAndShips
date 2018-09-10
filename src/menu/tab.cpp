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

#include "tab.h"
#include "../message.h"
#include "../window.h"
#include "../settings.h"
#include <SDL2_gfxPrimitives.h>

using namespace std;

C_Tab::C_Tab()

{
    C_Settings& settings=C_Settings::Instances();
    m_width = settings.getWindowWidth();
    m_height = settings.getWindowHeight()/3;
    m_screen.x = 0;
    m_screen.y = settings.getWindowHeight() - m_height;

}


void C_Tab::displayTab(bool open){
    if(open){
        C_Settings& settings=C_Settings::Instances();
        int angle = 10;
        Sint16 x1 = m_screen.x; //x top left
		Sint16 y1 = m_screen.y;
		Sint16 x2 = x1 + m_width; //x bottom right
		Sint16 y2 = settings.getWindowHeight() + angle;
		Uint8 R = 0, G = 0, B = 0, A = 150;

		//background
        C_Window& win=C_Window::Instances();
		roundedBoxRGBA(win.getRenderer(),x1,y1,x2,y2,angle,R,G,B,A);
    }
}

