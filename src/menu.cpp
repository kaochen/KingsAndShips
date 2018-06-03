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


#include "menu.h"
#include "window.h"
#include "settings.h"
#include "surfaces.h"
#include "grid.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;
C_MenuItem::C_MenuItem(string name, int x_screen, int y_screen):
	m_name(name),
	m_x_screen(x_screen),
	m_y_screen(y_screen),
	m_width(64),
	m_height(64)
{
}
C_MenuItem::~C_MenuItem()
{
}

int C_MenuItem::getXScreen() const{
	return m_x_screen;
}
int C_MenuItem::getYScreen() const{
	return m_y_screen;
}
int C_MenuItem::getWidth() const{
	return m_width;
}
int C_MenuItem::getHeight() const{
	return m_height;
}
string C_MenuItem::getName(){
	return m_name;
	}

 void C_MenuItem::render(){
        C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(m_name, m_x_screen + m_width/2,m_y_screen + m_height + 18);
  }

//-------------------------------------------------------------

C_Button::C_Button(string name,string image_out,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen),
	m_image_out(image_out)
{
}

C_Button::~C_Button()
{
}

void C_Button::render(){
        drawBackground();
		C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(m_image_out, m_x_screen + m_width/2,m_y_screen + m_height + 18);
}

void C_Button::drawBackground(){

		C_Window& win=C_Window::Instances();
		int crop = 4;
        Sint16 x_tr = m_x_screen + crop; //x top right
		Sint16 y_tr = m_y_screen + crop;
		Sint16 x_bl = x_tr + m_width - 2*crop; //x bottom left
		Sint16 y_bl = y_tr + m_height - crop*2;
		Uint8 R = 0, G = 0, B = 0, A = 100;
		roundedBoxRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,10,R,G,B,A);
		roundedBoxRGBA(win.getRenderer(),x_tr+4,y_tr,x_bl-4,y_tr+4,2,255,255,255,10);
		roundedBoxRGBA(win.getRenderer(),x_tr+2,y_bl-4,x_bl-2,y_bl,2,0,0,0,70);
		//border
		roundedRectangleRGBA(win.getRenderer(),x_tr+1,y_tr+1,x_bl-1,y_bl-1,8,R,G,B,255);
		roundedRectangleRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,10,R,G,B,255);
}

void C_Button::setPercentage(int percentage)
{
    cout << percentage; //to calm down gcc
}
void C_Button::setPercentage(int a, int b)
{
    cout << a << b; //to calm down gcc
}

//-------------------------------------------------------------

C_ProgressBar::C_ProgressBar(string name,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_width = 128;
    m_height = 24;
    m_percentage = 100;
}

void C_ProgressBar::setPercentage(int percentage)
{
    m_percentage = percentage;
}

void C_ProgressBar::setPercentage(int a, int b){
    if(a != 0 && b !=0)
        m_percentage = ((100*a)/b);
    else
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


C_MenuText::C_MenuText(string name, string text, int fontSize, int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_name = name;
    m_text = text;
    m_fontSize = fontSize;
    C_TextureList& t=C_TextureList::Instances();
    t.loadTextAsTexturesIntoMap(name, text, fontSize);
}

void C_MenuText::render(){
    C_TextureList& t=C_TextureList::Instances();
    t.renderTexture(m_name, m_x_screen, m_y_screen);
}


//-------------------------------------------------------------

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_y_screen(0),
	m_height(72),
	m_current_wave(1),
    m_total_waves(1)
{
		C_Set& settings=C_Set::Instances();
		m_width = (settings.getWindowWidth()*50)/100;
		m_x_screen = (settings.getWindowWidth() - m_width)/2;
		int size = 64 + 10;
		int x_button = settings.getWindowWidth() - size;
		int y_button = settings.getWindowHeight()/2 - size;
		//left buttons
		m_map_menuItems[ADDNEWTOWER] = new C_Button("addNewTower","Buttons_AddTowerOut",x_button,y_button);
		y_button = settings.getWindowHeight()/2;
		m_map_menuItems[ADDNEWTURBINE] = new C_Button("addNewTurbine","Buttons_AddTurbineOut",x_button,y_button);

        //fox
		m_map_menuItems[PLAYERLIFE] = new C_ProgressBar("playerlife",x_button - size-50,40);
		m_map_menuItems[FOX_ICON] = new C_MenuItem("fox_sneaky",x_button-10,20);
		//Lion
		m_map_menuItems[INVADER_LIFE] = new C_ProgressBar("invaderlife",50,40);

		m_map_menuItems[WAVES_STATUS] = new C_MenuText("wavestatus","0/0", 20,50,60);
        m_button_count += 6;
}

C_Menu::~C_Menu(){
    for (int i = 0; i < 3; i++){
        delete m_map_menuItems[i];
    }
}

C_Menu& C_Menu::Instances()
{
	return m_instance;
}

void C_Menu::render(){
    C_Grid& grid=C_Grid::Instances();
    int playerLife = grid.getAllTownsLifeLevel();
    m_map_menuItems[PLAYERLIFE]->setPercentage(playerLife);
    m_map_menuItems[INVADER_LIFE]->setPercentage(m_current_wave,m_total_waves);

	//drawBackground();
	for (int i = 0; i < m_button_count; i++){
	    if(m_map_menuItems[i]!= nullptr)
		    m_map_menuItems[i]->render();
	}
}
C_MenuItem * C_Menu::getMenuItem(int menuItem){
	return m_map_menuItems[menuItem];
}

int C_Menu::getXScreen(){
	return m_x_screen;
}


void C_Menu::drawBackground(){
		C_Window& win=C_Window::Instances();
		Sint16 x_tr = m_width + m_x_screen; //x top right
		Sint16 y_tr = m_y_screen -5;
		Sint16 x_bl = m_x_screen; //x bottom left
		Sint16 y_bl = m_height;
		Uint8 R = 50, G = 50, B = 50, A = 230;
		roundedBoxRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,5,R,G,B,A);
		roundedRectangleRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,5,R,G,B,255);


	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture("Buttons_lys", m_x_screen + m_width/2, 80);


}

void C_Menu::updateLevelInfos(int current_wave, int total_waves){
    m_current_wave = current_wave;
    m_total_waves = total_waves;
    string m = to_string(m_total_waves - m_current_wave +1) + "/" + to_string(m_total_waves);
    delete m_map_menuItems[WAVES_STATUS];
    m_map_menuItems[WAVES_STATUS] = new C_MenuText("wavestatus",m, 20,128,100);
}


