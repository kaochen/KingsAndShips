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

//Display all the menuItems in one menu on top of the game

#include "menu.h"
#include "window.h"
#include "settings.h"
#include "texture.h"
#include "level/grid.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_y_screen(0),
	m_height(72),
	m_current_wave(1),
    m_total_waves(1)
{
		C_Settings& settings=C_Settings::Instances();
		m_width = (settings.getWindowWidth()*50)/100;
		m_x_screen = (settings.getWindowWidth() - m_width)/2;
		int size = 64 + 10;
		int x_button = settings.getWindowWidth() - size;
		int y_button = settings.getWindowHeight()/2 - size;
		//left buttons
		m_map_menuItems[ADDNEWTOWER] = new C_Button("AddTower","Buttons_AddTower",x_button,y_button);
		y_button +=size;
		m_map_menuItems[ADDNEWTURBINE] = new C_Button("AddTurbine","Buttons_AddTurbine",x_button,y_button);
		y_button +=size;
		m_map_menuItems[ADDNEWBARRICADE] = new C_Button("AddBarricade","Buttons_AddBarricade",x_button,y_button);
        //fox
		m_map_menuItems[PLAYERLIFE] = new C_ProgressBar("playerlife",x_button - size-50,40);
		m_map_menuItems[FOX_ICON] = new C_MenuItem("Characters_fox",x_button-10,20);
		//Lion
		m_map_menuItems[BOAT_LIFE] = new C_ProgressBar("boatLife",50,40);

		m_map_menuItems[LION_ICON] = new C_MenuItem("Characters_lion",20,30);
		m_map_menuItems[WAVES_STATUS] = new C_MenuText("wavestatus","0/0", 20,128,100);

        m_button_count += 7;
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
    m_map_menuItems[BOAT_LIFE]->setPercentage(m_current_wave,m_total_waves);

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

void C_Menu::resetValues(){
    m_current_wave = 1;
    m_total_waves = 1;
	m_map_menuItems[WAVES_STATUS] = new C_MenuText("wavestatus","0/0", 20,128,100);
}
