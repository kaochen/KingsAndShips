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
#include "message.h"
#include "window.h"
#include "settings.h"
#include "texture.h"
#include "wallet.h"
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
		m_menuItemsList["AddTower"] = new C_Button("AddTower","Buttons_AddTower",x_button,y_button);
		y_button +=size;
	    m_menuItemsList["AddTurbine"] = new C_Button("AddTurbine","Buttons_AddTurbine",x_button,y_button);
		y_button +=size;
		m_menuItemsList["AddBarricade"] = new C_Button("AddBarricade","Buttons_AddBarricade",x_button,y_button);

        updateInfos();
}

C_Menu::~C_Menu(){
	for(auto const& x : m_menuItemsList){
		if(x.second != nullptr)
	        delete  x.second;
	}
}

C_Menu& C_Menu::Instances()
{
	return m_instance;
}

void C_Menu::updateInfos(){
    updatePlayerLife();
    updateLionStatus();
    updateWalletStatus();
}

void C_Menu::render(){
	//draw all buttons, layer by layer;
	for(int i = BACK; i <= FRONT; i++){
	    for(auto& x : m_menuItemsList){
	        if(x.second != nullptr){
	            if(x.second->getLayer() == i){
		            x.second->render();
		        }
		    }
	    }
	}
}
C_MenuItem * C_Menu::getMenuItem(string name){
	return m_menuItemsList[name];
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
}

void C_Menu::resetValues(){
    m_current_wave = 1;
    m_total_waves = 1;
    updatePlayerLife();
    updateLionStatus();
}


void C_Menu::updatePlayerLife(){
    C_Grid& grid=C_Grid::Instances();
    int playerLife = grid.getAllTownsLifeLevel();

    C_Settings& settings=C_Settings::Instances();
	int x = settings.getWindowWidth();

    if(m_menuItemsList["Characters_fox"] == nullptr){
	    m_menuItemsList["Characters_fox"] = new C_MenuItem("Characters_fox",x - 92,20);
	}

    //progress bar value
    if(m_menuItemsList["playerlife"] == nullptr){
        m_menuItemsList["playerlife"] = new C_ProgressBar("playerlife",x - 192,40);
    }

    if(m_menuItemsList["playerlife"] != nullptr){
        m_menuItemsList["playerlife"]->setPercentage(playerLife);
    }
    else{
        C_Message m;
        m.printM("the progess bar playerlife does not exist");
    }

    //text over the progress bar
    string space;
    if (playerLife < 100  &&  playerLife >= 10){space = " ";}
    else if (playerLife < 10){space = "  ";}
    else{space = "";}

    string text = "Life :"+ space + to_string(playerLife);
	if(m_menuItemsList["lifestatus"] != nullptr){
		    delete m_menuItemsList["lifestatus"];
	}
	m_menuItemsList["lifestatus"] = new C_MenuText("lifestatus",text, 18,x - 128,100);
}


void C_Menu::updateLionStatus(){
        if (m_menuItemsList["Characters_lion"] == nullptr){
        		m_menuItemsList["Characters_lion"] = new C_MenuItem("Characters_lion",20,30);
        }
        if(m_menuItemsList["boatLife"] == nullptr){
		    m_menuItemsList["boatLife"] = new C_ProgressBar("boatLife",50,40);
		}
		if(m_menuItemsList["boatLife"] != nullptr){
		    m_menuItemsList["boatLife"]->setPercentage(m_current_wave,m_total_waves);
		}

		string m = "Wave " + to_string(m_total_waves - m_current_wave +1) + "/" + to_string(m_total_waves);
		if(m_menuItemsList["wavestatus"] != nullptr){
            delete m_menuItemsList["wavestatus"];
        }
		m_menuItemsList["wavestatus"] = new C_MenuText("wavestatus",m, 18,128,100);
}

void C_Menu::updateWalletStatus(){
    C_Settings& settings=C_Settings::Instances();
	int x = settings.getWindowWidth();
	C_Wallet& wallet=C_Wallet::Instances();
	//progress bar
    if(m_menuItemsList["walletBar"]== nullptr){
		m_menuItemsList["walletBar"] = new C_ProgressBar("walletBar",x - 192,100);
		}
	 if(m_menuItemsList["walletBar"] != nullptr){
        m_menuItemsList["walletBar"]->setPercentage(wallet.getBalance(),wallet.getWalletMax());
    }
    //text over the progress bar
    string m = "Wallet " + to_string(wallet.getBalance());
	if(m_menuItemsList["walletStatus"] != nullptr){
        delete m_menuItemsList["walletStatus"];
    }
	m_menuItemsList["walletStatus"] = new C_MenuText("walletStatus",m, 18,x-128,160);
}
