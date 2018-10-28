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
    m_bottomMenuOpen(false)
{
		C_Settings& settings=C_Settings::Instances();
		int size = 64 + 10;
		int x_button = settings.getWindowWidth() - size;
		int y_button = settings.getWindowHeight()/2 - size;
		//left buttons
		m_menuItemsList["AddTower"] = new C_GB_AddUnit("AddTower","AddTower",x_button,y_button);
		y_button +=size;
	    m_menuItemsList["AddTurbine"] = new C_GB_AddUnit("AddTurbine","AddTurbine",x_button,y_button);
		y_button +=size;
		m_menuItemsList["AddBarricade"] = new C_GB_AddUnit("AddBarricade","AddBarricade",x_button,y_button);

        updateInfos();
        //add the bottom buttons line
        S_Coord line;
        line.x = 20;
        line.y = settings.getWindowHeight() - 20;
        bottomButtonsLine(line);
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
    vector<string>  list = getMenuItemsList();
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
        m_menuItemsList["playerlife"] = new C_GP_Status("playerlife",x - 192,40);
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
		    m_menuItemsList["boatLife"] = new C_GP_Status("boatLife",50,40);
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
		m_menuItemsList["walletBar"] = new C_GP_Status("walletBar",x - 192,100);
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




void C_Menu::openBottomMenu(){
    if(m_bottomMenuOpen)
        m_bottomMenuOpen = false;
    else
        m_bottomMenuOpen = true;
}


void C_Menu::displayBottomMenu(){
    m_tabs[0]->displayTab(m_bottomMenuOpen, m_currentTab);
}


vector<string> C_Menu::getMenuItemsList(){
    vector<string> list;
    //Always Visible
    list.push_back("Characters_lion");
    list.push_back("boatLife");
    list.push_back("Characters_fox");
    list.push_back("playerlife");
    list.push_back("gold_pile");
    list.push_back("walletBar");
    list.push_back("popOutMenu");
    list.push_back("recenterButton");


    if(m_bottomMenuOpen){
        //get tab selector buttons
        for (size_t i = 0; i < m_tabs.size() ; i++){
            list.push_back(m_tabs[i]->getName());
        }
        //get the list of active button from the selected tab
        vector<string> tmp = m_tabs[m_currentTab]->getListOfVisibleItems();
        list.insert(list.end(), tmp.begin(), tmp.end());

    }
    else{
        list.push_back("AddTower");
        list.push_back("AddTurbine");
        list.push_back("AddBarricade");
    }

    return list;
}


void C_Menu::menuBanner(){
    m_tabs.push_back( new C_Tab_Levels());
    m_tabs.push_back( new C_Tab_Settings());
    m_tabs.push_back( new C_Tab("About"));

    //declare buttons from tabs into the mainItemList
    for (size_t i = 0; i < m_tabs.size() ; i++){
            std::map<std::string, C_MenuItem*> tabItems;
            tabItems = m_tabs[i]->getItemList();
            m_menuItemsList.insert(tabItems.begin(),tabItems.end());
        }
    setTabNbr(0); //set the focus on the first tab
}

void C_Menu::setTabNbr(int nbr){
    m_currentTab = nbr;

}


void C_Menu::bottomButton(const string &name,S_Coord screen){
        if(m_menuItemsList[name] == nullptr){
            C_Command *command = nullptr;
            if(name ==  "popOutMenu"){
            	m_menuItemsList[name] = new C_Button(name,name,screen.x,screen.y);
		        command = new C_OpenMenu();
		    }
            else if(name ==  "recenterButton"){
		        m_menuItemsList[name] = new C_Button(name,"MenuSpare",screen.x,screen.y);
		        command = new C_CenterCamera();
		    }
            m_menuItemsList[name]->setCommand(command);
		}
}

void C_Menu::bottomButtonsLine(S_Coord screen){
    S_Coord pos;
    int buttonSize = 64;
    int space = 10;
    pos.x = screen.x;
    pos.y = screen.y - buttonSize;
    bottomButton("popOutMenu", pos);
    pos.x += buttonSize + space;
    bottomButton("recenterButton", pos);
}