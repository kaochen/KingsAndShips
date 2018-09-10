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

        Uint8 R = 0, G = 0, B = 0, A = 150;
        //draw points clockwise
        Sint16 x1 = m_screen.x; //top left
		Sint16 y1 = m_screen.y;

		Sint16 x2 = x1 + 5; //tab beginning
		Sint16 y2 = y1;

		Sint16 x3 = x2 + 20;
		Sint16 y3 = y2 - 20;

		Sint16 tabSize = 100;
		Sint16 x4 = x3 + tabSize;
		Sint16 y4 = y3;

		Sint16 x5 = x4 + 20;
		Sint16 y5 = y1;

		Sint16 x6 = x1 + m_width; //top right
		Sint16 y6 = y1;

        Sint16 x7 = x6; // bottom right
        Sint16 y7 = settings.getWindowHeight();

	    Sint16 x8 = x1;
	    Sint16 y8 = y7;

	    Sint16 vx[] = {x1,x2,x3,x4,x5,x6,x7,x8};
	    Sint16 vy[] = {y1,y2,y3,y4,y5,y6,y7,y8};

		//draw
        C_Window& win=C_Window::Instances();
    	SDL_Renderer * renderer = win.getRenderer();
		filledPolygonRGBA(renderer,vx,vy,8,R,G,B,A);
    }
}

