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
#include "../window.h"
#include "../texture.h"
#include "../message.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;
C_MenuItem::C_MenuItem(string name, int x_screen, int y_screen):
    m_type(ACTION),
	m_name(name),
	m_image(name),
	m_x_screen(x_screen),
	m_y_screen(y_screen),
	m_width(64),
	m_height(64),
	m_layer(FRONT),
	m_enable(true),
	m_state(ACTIVE)
{
    m_textName = name + "Text";
    m_text = "";
    m_color = {200,200,200,255};
    m_x_text = 0;
    m_y_text = 0;
    m_command = nullptr;
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
        t.renderTexture(m_textName, m_x_screen + m_width/2, m_y_screen +  m_height/2,CENTER);
        }
}

 void C_MenuItem::render(){
        C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(m_image, m_x_screen + m_width/2,m_y_screen+m_height/2,CENTER);
		renderText();
  }

  void C_MenuItem::action(){
    C_Message m;
    m.printM("push button " + m_name + "\n");
    if(m_command != nullptr){
        m_command->action();
        }
  }



void C_MenuItem::stripes(int x_screen, int y_screen, int width, int height){
	    C_TextureList& t=C_TextureList::Instances();
	    int size = 12;
        int x = width/size + 1;
        int y = height/size + 1;
        for(int j = 0; j < y; j++){
            for(int i = 0; i < x; i++){
                t.renderTexture("icons_12px_stripes", x_screen +i*size,y_screen +j*size, LEFT);
            }
        }
}



//-------------------------------------------------------------

C_Button::C_Button(string name,string image,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_image = image;
}


void C_Button::render(){
        string name = "Buttons_"+m_image;
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
		t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height/2,CENTER);
}

//-------------------------------------------------------------

C_MB_TabSelect::C_MB_TabSelect(string name,string text, int fontSize,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen){
	setText(text, fontSize);
	m_width = 100;
	m_height = 30;
}


void C_MB_TabSelect::render(){
        C_Window& win=C_Window::Instances();
        Sint16 x1 = m_x_screen; //x top right
		Sint16 y1 = m_y_screen;
		Sint16 x2 = x1 + m_width; //x bottom left
		Sint16 y2 = y1 + 5;
		Uint8 R = 0, G = 0, B = 0;

        //top
		if(m_state == ACTIVE){R = 8; G = 63; B = 127;}
		else if(m_state == HOVER){R = 16; G = 126; B = 255;}
		boxRGBA(win.getRenderer(),x1,y1,x2,y2,R,G,B,255);

        //bottom
        y1 = y2 + 5;
        y2 = y1 + 20;
		boxRGBA(win.getRenderer(),x1,y1,x2,y2,200,200,200,50);

        C_TextureList& t=C_TextureList::Instances();
		if(m_text !=""){
		    if(t.searchTexture(m_textName)== nullptr || m_text != m_oldText){
                t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
                m_oldText = m_text;
            }
        t.renderTexture(m_textName, m_x_screen + m_width/2 , m_y_screen + m_height*2/3,CENTER);
        }
}

//-------------------------------------------------------------

C_MB_1Line::C_MB_1Line(string name,string text,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen){
	m_fontSize = 18;
	m_title = name;
	m_titleName = "Settings_Title_" + name;
    m_text = text;
	m_textName = "Settings_Text_" + name;
	m_width = 400;
	m_height = 20;
}


void C_MB_1Line::render(){
        C_Window& win=C_Window::Instances();
        Sint16 x1 = m_x_screen; //x top right
		Sint16 y1 = m_y_screen;
		Sint16 x2 = x1 + m_width; //x bottom left
		Sint16 y2 = y1 + m_height;
		Uint8 R = 0, G = 0, B = 0;

		if(m_state == ACTIVE){
		    R = 50; G = 50; B = 50;
    		boxRGBA(win.getRenderer(),x1,y1,x2,y2,R,G,B,255);
		}
		else if(m_state == HOVER){
		    R = 8; G = 63; B = 127;
    		boxRGBA(win.getRenderer(),x1-2,y1-2,x2+2,y2+2,R,G,B,255);
		    }


        C_TextureList& t=C_TextureList::Instances();
        if(m_title !=""){
		    if(t.searchTexture(m_titleName)== nullptr || m_title != m_oldTitle){
                t.loadTextAsTexturesIntoMap(m_titleName, m_title, m_fontSize, m_color);
                m_oldTitle = m_title;
            }
        t.renderTexture(m_titleName, x1+2, y1 + m_height/2, LEFT);
        }

		if(m_text !=""){
		    if(t.searchTexture(m_textName)== nullptr || m_text != m_oldText){
                t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
                m_oldText = m_text;
            }
        t.renderTexture(m_textName, x2-2, y1 + m_height/2, RIGHT);
        }
}

//-------------------------------------------------------------

C_MB_LevelCard::C_MB_LevelCard(string name,string text,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen){
	m_fontSize = 18;
	m_title = name;
	m_titleName = "Card_Title_" + name;
    m_text = text;
	m_textName = "Card_Text_" + name;
	m_width = 204;
	m_height = 300;
}

void C_MB_LevelCard::render(){
    C_Window& win=C_Window::Instances();
		Uint8 R = 0, G = 0, B = 0;
        int zoom = 0;
		if(m_state == ACTIVE){
		    R = 50; G = 50; B = 50;
		}
		else if(m_state == HOVER){
		    R = 8; G = 63; B = 127;
		    zoom = 6;
		    }

        Sint16 x1 = m_x_screen -zoom; //x top right
		Sint16 y1 = m_y_screen -zoom;
		Sint16 x2 = x1 + m_width + 2*zoom; //x bottom left
		Sint16 y2 = y1 + m_height + 2*zoom;

    	boxRGBA(win.getRenderer(),x1,y1,x2,y2,50,50,50,100);
    	boxRGBA(win.getRenderer(),x1,y1,x2,y1+50,R,G,B,255);
    	boxRGBA(win.getRenderer(),x1,y1+250,x2,y2,R,G,B,255);

        int width = x2 - x1;
        int height = y2 - y1;
        stripes(x1, y1, width, height);

    	C_TextureList& t=C_TextureList::Instances();
        if(t.searchTexture(m_textName)== nullptr){
             t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
        }
        t.renderTexture(m_textName, x1 + width/2, y1 + 25,CENTER);

}

//-------------------------------------------------------------

C_GB_AddUnit::C_GB_AddUnit(string name,string image,int x_screen, int y_screen)
	:C_Button(name,image,x_screen,y_screen)
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

C_GB_AddUnit::~C_GB_AddUnit()
{
    if(m_unit !=nullptr)
        delete m_unit;
}

void C_GB_AddUnit::drag(S_Coord screen){
    bool water = false;
    if (m_unit != nullptr){
        if(m_unit->getName() == "barricade"){
            water = true;
        }
        m_unit->drag(screen,water);
    }
}

void C_GB_AddUnit::render(){
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
    t.renderTexture(m_textName, m_x_screen + 19, m_y_screen + 16,CENTER);
}
//-------------------------------------------------------------

C_GP_Status::C_GP_Status(string name,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_type = STATUS;
    m_width = 128;
    m_height = 24;
    m_percentage = 100;
    m_layer = BACK;
    m_y_text = -4;
}


void C_GP_Status::setPercentage(int a, int b){
    if(a != 0 && b !=0)
        m_percentage = ((100*a)/b);
    else
        m_percentage = 0;

    if (m_percentage > 100)
        m_percentage = 100;
    if (m_percentage < 0)
        m_percentage = 0;
}

void C_GP_Status::render(){
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

void C_GP_Status::littledots(int x_screen, int y_screen, int width, int height){
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

