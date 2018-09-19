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
#include "tab.h"
#include "../message.h"
#include "../window.h"
#include "../settings.h"
#include "../wallet.h"
#include "../level/grid.h"
#include <SDL2_gfxPrimitives.h>

using namespace std;

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_current_wave(1),
    m_total_waves(1),
    m_bottomMenuOpen(false),
    m_nbrOfTabs(3)
{
		C_Settings& settings=C_Settings::Instances();
		int size = 64 + 10;
		int x_button = settings.getWindowWidth() - size;
		int y_button = settings.getWindowHeight()/2 - size;
		//left buttons
		m_menuItemsList["AddTower"] = new C_ButtonAddUnit("AddTower","AddTower",x_button,y_button);
		y_button +=size;
	    m_menuItemsList["AddTurbine"] = new C_ButtonAddUnit("AddTurbine","AddTurbine",x_button,y_button);
		y_button +=size;
		m_menuItemsList["AddBarricade"] = new C_ButtonAddUnit("AddBarricade","AddBarricade",x_button,y_button);

        updateInfos();
        popOutMenu();
        menuBanner();
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
    displayBottomMenu();
    vector<string>  list = getListOfButtonVisible();
	//draw all buttons, layer by layer;
	for(int j = BACK; j <= FRONT; j++){
	    for(size_t i = 0; i < list.size(); i++){
                C_MenuItem * b = getMenuItem(list[i]);
	            if( b != nullptr){
	                if(b->getLayer() == j){
		                b->render();
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
    string text = "Life: " + nbrToString(playerLife);
    if(m_menuItemsList["playerlife"] != nullptr){
        m_menuItemsList["playerlife"]->setPercentage(playerLife);
        m_menuItemsList["playerlife"]->setText(text, 18);
    }
    else{
        C_Message m;
        m.printM("the progess bar playerlife does not exist");
    }
}


void C_Menu::updateAttackerStatus(){
        if (m_menuItemsList["Characters_lion"] == nullptr){
        		m_menuItemsList["Characters_lion"] = new C_MenuItem("Characters_lion",20,30);
        }
        if(m_menuItemsList["boatLife"] == nullptr){
		    m_menuItemsList["boatLife"] = new C_ProgressBar("boatLife",50,40);
		}
		if(m_menuItemsList["boatLife"] != nullptr){
    		string text = "Wave " + to_string(m_total_waves - m_current_wave +1) + "/" + to_string(m_total_waves);
		    m_menuItemsList["boatLife"]->setPercentage(m_current_wave,m_total_waves);
            m_menuItemsList["boatLife"]->setText(text, 18);
		}
}

void C_Menu::updateWalletStatus(){
    C_Settings& settings=C_Settings::Instances();
	int x = settings.getWindowWidth();
	C_Wallet& wallet=C_Wallet::Instances();
    if(m_menuItemsList["gold_pile"] == nullptr){
	    m_menuItemsList["gold_pile"] = new C_MenuItem("gold_pile",x - 236,58);
	}
	//progress bar
    if(m_menuItemsList["walletBar"]== nullptr){
		m_menuItemsList["walletBar"] = new C_ProgressBar("walletBar",x - 192,100);
		}
	 if(m_menuItemsList["walletBar"] != nullptr){
    	string text = nbrToString(wallet.getBalance());
        m_menuItemsList["walletBar"]->setPercentage(wallet.getBalance(),wallet.getWalletMax());
        m_menuItemsList["walletBar"]->setText(text, 18);
    }
}


string C_Menu::nbrToString(int nbr){
    string space = "";
    string nbrStr = to_string(nbr);

    if (nbrStr.size()==2){space = "  ";}
    else if (nbrStr.size()==1){space = "    ";}
    else{space = "";}
    return space + nbrStr;
}


void C_Menu::popOutMenu(){
        C_Settings& settings=C_Settings::Instances();
        int y = settings.getWindowHeight() - 64 - 20;
        if(m_menuItemsList["popOutMenu"] == nullptr){
		    m_menuItemsList["popOutMenu"] = new C_Button("popOutMenu","popOutMenu",20,y);
		    C_OpenMenu *om = new C_OpenMenu();
            m_menuItemsList["popOutMenu"]->setCommand(om);
		}

}

void C_Menu::openBottomMenu(){
    if(m_bottomMenuOpen)
        m_bottomMenuOpen = false;
    else
        m_bottomMenuOpen = true;
}


void C_Menu::displayBottomMenu(){
    m_tabs->displayTab(m_bottomMenuOpen, m_currentTab);
}

vector<string> C_Menu::getListOfButtonToListen(){
    vector<string> list;
    //Always Visible
    list.push_back("popOutMenu");
    if(m_bottomMenuOpen){
        for (int i = 0; i < m_nbrOfTabs ; i++){
            list.push_back(tabName(i));
        }
    }
    else{
        list.push_back("AddTower");
        list.push_back("AddTurbine");
        list.push_back("AddBarricade");
    }
    return list;
}
vector<string> C_Menu::getListOfButtonVisible(){
    vector<string> list;
    //Always Visible
    list.push_back("Characters_lion");
    list.push_back("boatLife");
    list.push_back("Characters_fox");
    list.push_back("playerlife");
    list.push_back("gold_pile");
    list.push_back("walletBar");
    list.push_back("popOutMenu");
    list.push_back("AddTower");
    list.push_back("AddTurbine");
    list.push_back("AddBarricade");

    if(m_bottomMenuOpen){
        for (int i = 0; i < m_nbrOfTabs ; i++){
            list.push_back(tabName(i));
        }
    }

    return list;
}


void C_Menu::menuBanner(){
    C_Settings& settings=C_Settings::Instances();
    int width = settings.getWindowWidth();
    int height = settings.getWindowHeight()/3;
    int x_screen = 0;
    int y_screen = settings.getWindowHeight() - height;
    if(m_tabs != nullptr){delete m_tabs;};
    m_tabs = new C_Tab(x_screen,y_screen,width,height);
    int flagWidth = 128;
    x_screen += 32;
    y_screen += 4;
    for (int i = 0; i < m_nbrOfTabs ; i++){
    string name = tabName(i);
        if(m_menuItemsList[name] == nullptr){
		    m_menuItemsList[name] = new C_MenuButton(name,name,18,x_screen + i*flagWidth ,y_screen);
		    if(m_menuItemsList[name] != nullptr){
		        m_menuItemsList[name]->setCommand(new C_ChangeTab);
		        if( m_menuItemsList[name]->getCommand() != nullptr)
		            m_menuItemsList[name]->getCommand()->setNbr(i);
		        }
		    }
	}
    setTabNbr(0); //set the focus on the first tab
}

void C_Menu::setTabNbr(int nbr){
    //first reset all
    for (int i = 0; i < m_nbrOfTabs ; i++){
        string name = tabName(i);
        if(m_menuItemsList[name] != nullptr){
            m_menuItemsList[name]->setImage("FlagGrey");
        }
    }
    //then set the choosen one
        string name = tabName(nbr);
        if(m_menuItemsList[name] != nullptr){
            m_menuItemsList[name]->setImage("FlagRed");
        }
    m_currentTab = nbr;

}

