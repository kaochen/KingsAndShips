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

using namespace std;

C_Tab::C_Tab()

{
    C_Settings& settings=C_Settings::Instances();
    m_width = settings.getWindowWidth();
    m_height = settings.getWindowHeight()/3;
    m_screen.x = 0;
    m_screen.y = settings.getWindowHeight() - m_height;
    m_tabSize = 100;

}


void C_Tab::displayTab(bool open){
    if(open){

        Uint8 R = 0, G = 0, B = 0, A = 150;
        Sint16 *vx = getVertex_X(m_screen.x);
        Sint16 *vy = getVertex_Y(m_screen.y);
		//draw
        C_Window& win=C_Window::Instances();
    	SDL_Renderer * renderer = win.getRenderer();
		filledPolygonRGBA(renderer,vx,vy,8,R,G,B,A);
		for(int i = 0; i < 3; i++){
            Sint16 *vx1 = getVertex_X(m_screen.x + i);
		    polygonRGBA(renderer,vx1,vy,8,R,G,B,A);
		}
		for(int i = 0; i < 3; i++){
		    Sint16 *vy1 = getVertex_Y(m_screen.y + i);
		    polygonRGBA(renderer,vx,vy1,8,R,G,B,A);
		}
    }
}


Sint16  *C_Tab::getVertex_X(Sint16 x){
        //draw points clockwise
        Sint16 x1 = x; //top left
		Sint16 x2 = x1 + 5; //tab beginning
		Sint16 x3 = x2 + 20;
		Sint16 x4 = x3 + m_tabSize;
		Sint16 x5 = x4 + 20;
		Sint16 x6 = x1 + m_width; //top right
        Sint16 x7 = x6; // bottom right
	    Sint16 x8 = x1; //bottom left;
        Sint16 *array = new Sint16[8];
	    array[0] = x1;
	    array[1] = x2;
	    array[2] = x3;
	    array[3] = x4;
	    array[4] = x5;
	    array[5] = x6;
	    array[6] = x7;
	    array[7] = x8;
        return array;
}

Sint16  *C_Tab::getVertex_Y(Sint16 y){
        C_Settings& settings=C_Settings::Instances();
        //draw points clockwise
		Sint16 y1 = y; //top left
        Sint16 y2 = y1; //tab beginning
		Sint16 y3 = y2 - 20;
		Sint16 y4 = y3;
		Sint16 y5 = y1;
		Sint16 y6 = y1;
        Sint16 y7 = settings.getWindowHeight();
	    Sint16 y8 = y7;
        Sint16 *array = new Sint16[8];
	    array[0] = y1;
	    array[1] = y2;
	    array[2] = y3;
	    array[3] = y4;
	    array[4] = y5;
	    array[5] = y6;
	    array[6] = y7;
	    array[7] = y8;
        return array;
}

