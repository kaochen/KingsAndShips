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

int C_Tab::m_id = 0;

C_Tab::C_Tab(string title,int x_screen,int y_screen, int width, int height)
{
    m_id++;
    m_name = "tab_" + m_id;
    m_title = title;
    m_screen.x = x_screen;
    m_screen.y = y_screen;
    m_width = width;
    m_height = height;
    m_tabSize = 100;
}


void C_Tab::displayTab(bool open, size_t nbr){
    if(open){
                focusTab(nbr);
    }
}

void C_Tab::focusTab(size_t nbr){

        C_Settings& settings=C_Settings::Instances();
        Uint8 R = 0, G = 0, B = 0, A = 150;
        Sint16 x1 = m_screen.x - 5; //top left
 		Sint16 y1 = m_screen.y; //top left
		Sint16 x2 = m_screen.x + m_width; //top right
		Sint16 y2 = y1;
        Sint16 x3 = x2; // bottom right
        Sint16 y3 = settings.getWindowHeight();
	    Sint16 x4 = x1; //bottom left;
	    Sint16 y4 = y3;

        Sint16 vx[] = {x1,x2,x3,x4};
        Sint16 vy[] = {y1,y2,y3,y4};
		//draw
        C_Window& win=C_Window::Instances();
    	SDL_Renderer * renderer = win.getRenderer();
		filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);

        //border

		for(Sint16 j = 0; j < 2; j++){
		    for(Sint16 i = 0; i < 2; i++){
                Sint16 vx1[] = {Sint16(x1+i),Sint16(x2+i),Sint16(x3+i),Sint16(x4-i)};
                Sint16 vy1[] = {Sint16(y1+i),Sint16(y2+i),Sint16(y3+i),Sint16(y4+i)};
		        polygonRGBA(renderer,vx1,vy1,4,R,G,B,A);
		        }
		    x1 += 1;
		    x2 += 1;
		    x3 += 1;
		    x4 += 1;
		}

}

