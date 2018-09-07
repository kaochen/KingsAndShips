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


#include "menuItems.h"
#include "window.h"
#include "texture.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;
C_MenuItem::C_MenuItem(string name, int x_screen, int y_screen):
    m_type(ACTION),
	m_name(name),
	m_x_screen(x_screen),
	m_y_screen(y_screen),
	m_width(64),
	m_height(64),
	m_layer(FRONT),
	m_enable(true)
{
    m_textName = name + "Text";
    m_text = "";
    m_color = {200,200,200,255};
    m_x_text = 0;
    m_y_text = 0;
}

 void C_MenuItem::setText(string text, int fontSize){
    m_text = text;
    m_fontSize = fontSize;
 }
 void C_MenuItem::setTextPosition(int x_text, int y_text){
    m_x_text = x_text;
    m_y_text = y_text;
 }


 void C_MenuItem::renderText(){
         C_TextureList& t=C_TextureList::Instances();
		if(m_text !=""){
		    if(t.searchTexture(m_textName)== nullptr || m_text != m_oldText){
                t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
                m_oldText = m_text;
            }
        t.renderTexture(m_textName, m_x_screen + m_width/2 + m_x_text, m_y_screen +  m_width/2 + m_y_text);
        }
}

 void C_MenuItem::render(){
        C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(m_name, m_x_screen + m_width/2,m_y_screen + m_height + 18);
		renderText();
  }

//-------------------------------------------------------------

C_Button::C_Button(string name,string image_out,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen),
	m_image_out(image_out),
	m_state(ACTIVE)
{
}


void C_Button::render(){
        string name = "Buttons_"+m_name;
        string prefix;
        if(m_state == ACTIVE){
            prefix ="_Active";
            }
        else if(m_state == HOVER){
            prefix ="_Hover";
            }
        else{
            prefix ="_Disabled";
            }

        if(m_enable == false){
            prefix ="_Disabled";
            }
        name += prefix;
		C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height + 18);
}


//-------------------------------------------------------------

C_ButtonAddUnit::C_ButtonAddUnit(string name,string image_out,int x_screen, int y_screen)
	:C_Button(name,image_out,x_screen,y_screen)
{
    m_type = DRAGUNIT;
    if(name == "AddTower"){
         m_unit = new C_ArcherTower(0,0,0);
         }
    else if(name == "AddTurbine"){
         m_unit = new C_Turbine(0,0,0);
         }
    else if(name == "AddBarricade"){
        m_unit = new C_Barricade(0,0,1);
        }
    m_text = to_string(m_unit->getCost());
    m_fontSize = 9;
}

C_ButtonAddUnit::~C_ButtonAddUnit()
{
    if(m_unit !=nullptr)
        delete m_unit;
}

void C_ButtonAddUnit::drag(S_Coord screen){
    bool water = false;
    if (m_unit != nullptr){
        if(m_unit->getName() == "barricade"){
            water = true;
        }
        m_unit->drag(screen,water);
    }
}

void C_ButtonAddUnit::render(){
    C_Wallet& wallet=C_Wallet::Instances();
    if(wallet.getBalance() < m_unit->getCost()){
        m_enable = false;
    }
    else{
        m_enable = true;
    }

    C_Button::render();
    C_TextureList& t=C_TextureList::Instances();
    if(t.searchTexture(m_textName)== nullptr){
        t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
    }
    t.renderTexture(m_textName, m_x_screen + 19, m_y_screen + 64);
}
//-------------------------------------------------------------

C_ProgressBar::C_ProgressBar(string name,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_type = STATUS;
    m_width = 128;
    m_height = 24;
    m_percentage = 100;
    m_layer = BACK;
    m_y_text = -4;
}


void C_ProgressBar::setPercentage(int a, int b){
    if(a != 0 && b !=0)
        m_percentage = ((100*a)/b);
    else
        m_percentage = 0;

    if (m_percentage > 100)
        m_percentage = 100;
    if (m_percentage < 0)
        m_percentage = 0;
}

void C_ProgressBar::render(){
        C_Window& win=C_Window::Instances();
        Sint16 x1 = m_x_screen; //x top right
		Sint16 y1 = m_y_screen;
		Sint16 x2 = x1 + m_width; //x bottom left
		Sint16 y2 = y1 + m_height;
		Uint8 R = 0, G = 0, B = 0, A = 100;

        int angle = 4;
		//background
		roundedBoxRGBA(win.getRenderer(),x1,y1,x2,y2,angle,R,G,B,A);
        //life status
        Sint16 x_life = x1 + (m_percentage*m_width/100);

        //cout << m_percentage << "/" << angle << endl;
        G = 255;
        if (m_percentage < 50){
            G = 0;
            R = 255;
            }
        int offset = 0;
        int lifeAngle = angle;
        if(m_percentage < (angle*2)){
            lifeAngle = 2;
            offset = 1;
            }

        if(m_percentage > 2)
		        roundedBoxRGBA(win.getRenderer(),x1,y1+offset,x_life,y2-offset,lifeAngle,R,G,B,A);

        //reflect
		roundedBoxRGBA(win.getRenderer(),x1+4,y1,x2-4,y1+4,angle/2,255,255,255,30);
		//shadow
		roundedBoxRGBA(win.getRenderer(),x1+2,y2-4,x2-2,y2,angle/2,0,0,0,70);
		//double border
		roundedRectangleRGBA(win.getRenderer(),x1+1,y1+1,x2-1,y2-1,angle-1,0,0,0,255);
		roundedRectangleRGBA(win.getRenderer(),x1,y1,x2,y2,angle,0,0,0,255);

		littledots(m_x_screen +2 , m_y_screen +6, m_width-4, m_height-4);

		renderText();
}

void C_ProgressBar::littledots(int x_screen, int y_screen, int width, int height){
        C_Window& win=C_Window::Instances();
        SDL_Rect dot;
            int size = 1;
            int steps = 3;
		    dot.w = size;
		    dot.h = size;
		    dot.x = x_screen;
		    dot.y = y_screen;
    		SDL_SetRenderDrawColor(win.getRenderer(), 60, 60, 60, 20 );
    		int w = width/(size*steps);
    		int h = height/(size*steps);
		    for(int j = 1; j < h;j++){
		        for(int i = 1; i < w;i++){
		            dot.x = x_screen + i*steps*size;
		            SDL_RenderFillRect(win.getRenderer(), &dot);
		        }
		        dot.y = y_screen + j*steps*size;
		    }
}

