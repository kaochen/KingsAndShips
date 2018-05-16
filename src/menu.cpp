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

//-------------------------------------------------------------

C_ProgressBar::C_ProgressBar(string name,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_width = 128;
    m_height = 24;
    m_percentage = 70;
}

void C_ProgressBar::setPercentage(int percentage)
{
    m_percentage = percentage;
}

void C_ProgressBar::render(){
        C_Window& win=C_Window::Instances();
        Sint16 x1 = m_x_screen; //x top right
		Sint16 y1 = m_y_screen;
		Sint16 x2 = x1 + m_width; //x bottom left
		Sint16 y2 = y1 + m_height;
		Uint8 R = 0, G = 0, B = 0, A = 100;
		//background
		roundedBoxRGBA(win.getRenderer(),x1,y1,x2,y2,10,R,G,B,A);
        //life status
        Sint16 x_life = x1 + (m_percentage*m_width/100);

        //cout << m_percentage << "/" << angle << endl;
        G = 255;
        if (m_percentage < 50){
            G = 0;
            R = 255;
            }
        int angle = 10;

        if(m_percentage < 20)
            angle = m_percentage/2;

        if(m_percentage > 1)
		        roundedBoxRGBA(win.getRenderer(),x1,y1,x_life,y2,angle,R,G,B,A);

        //reflect
		roundedBoxRGBA(win.getRenderer(),x1+4,y1,x2-4,y1+4,2,255,255,255,10);
		//shadow
		roundedBoxRGBA(win.getRenderer(),x1+2,y2-4,x2-2,y2,2,0,0,0,70);
		//double border
		roundedRectangleRGBA(win.getRenderer(),x1+1,y1+1,x2-1,y2-1,8,0,0,0,255);
		roundedRectangleRGBA(win.getRenderer(),x1,y1,x2,y2,10,0,0,0,255);
}

//-------------------------------------------------------------

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_y_screen(0),
	m_height(72)
{
		C_Set& settings=C_Set::Instances();
		m_width = (settings.getWindowWidth()*50)/100;
		m_x_screen = (settings.getWindowWidth() - m_width)/2;
		int size = 64 + 10;
		int x_button = settings.getWindowWidth() - size;
		int y_button = settings.getWindowHeight()/2 - size;
		m_map_menuItems[ADDNEWTOWER] = new C_Button("addNewTower","Buttons_AddTowerOut",x_button,y_button);
		y_button = settings.getWindowHeight()/2;
		m_map_menuItems[ADDNEWTURBINE] = new C_Button("addNewTurbine","Buttons_AddTurbineOut",x_button,y_button);

		m_map_menuItems[PLAYERLIFE] = new C_ProgressBar("playerlife",x_button - size,20);
		m_button_count += 3;
}

C_Menu::~C_Menu(){
}

C_Menu& C_Menu::Instances()
{
	return m_instance;
}

void C_Menu::render(){
    C_Grid& grid=C_Grid::Instances();
    int playerLife = grid.getAllTownsLifeLevel();
    m_map_menuItems[PLAYERLIFE]->setPercentage(playerLife);
	//drawBackground();
	for (int i = 0; i < m_button_count; i++){
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





