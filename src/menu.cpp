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
#include "wallet.h"
#include "level/grid.h"

using namespace std;

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_current_wave(1),
    m_total_waves(1)
{
		C_Settings& settings=C_Settings::Instances();
		int size = 64 + 10;
		int x_button = settings.getWindowWidth() - size;
		int y_button = settings.getWindowHeight()/2 - size;
		//left buttons
		m_menuItemsList["AddTower"] = new C_ButtonAddUnit("AddTower","Buttons_AddTower",x_button,y_button);
		y_button +=size;
	    m_menuItemsList["AddTurbine"] = new C_ButtonAddUnit("AddTurbine","Buttons_AddTurbine",x_button,y_button);
		y_button +=size;
		m_menuItemsList["AddBarricade"] = new C_ButtonAddUnit("AddBarricade","Buttons_AddBarricade",x_button,y_button);

        updateInfos();
}

C_Menu::~C_Menu(){
	for(auto const& x : m_menuItemsList){
		if(x.second != nullptr)
	        delete  x.second;
	}
}


void C_Menu::updateInfos(){
    updateDefenderStatus();
    updateAttackerStatus();
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


void C_Menu::updateLevelInfos(int current_wave, int total_waves){
    m_current_wave = current_wave;
    m_total_waves = total_waves;
}

void C_Menu::resetValues(){
    m_current_wave = 1;
    m_total_waves = 1;
    updateAttackerStatus();
    updateDefenderStatus();
}


void C_Menu::updateDefenderStatus(){
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
    string text = "Life: " + nbrToString(playerLife);
	if(m_menuItemsList["lifestatus"] != nullptr){
		    delete m_menuItemsList["lifestatus"];
	}
	m_menuItemsList["lifestatus"] = new C_MenuText("lifestatus",text, 18,x - 128,100);
}


void C_Menu::updateAttackerStatus(){
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
    string m = "Gold: " + nbrToString(wallet.getBalance());
	if(m_menuItemsList["walletStatus"] != nullptr){
        delete m_menuItemsList["walletStatus"];
    }
	m_menuItemsList["walletStatus"] = new C_MenuText("walletStatus",m, 18,x-128,160);
}


string C_Menu::nbrToString(int nbr){
    string space = "";
    string nbrStr = to_string(nbr);

    if (nbrStr.size()==2){space = "  ";}
    else if (nbrStr.size()==1){space = "    ";}
    else{space = "";}
    return space + nbrStr;
}
