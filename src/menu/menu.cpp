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

using namespace std;


C_Menu::C_Menu():
	m_current_wave(1),
	m_total_waves(1)
{
	C_Message::printM("Constructor C_Menu() : start\n");
	S_Coord upgradeCoord = {10,10};
	m_menuItemsList["upgradeTower"] = new C_GU_Upgrade("upgradeTower",upgradeCoord);

	updateDefenderStatus();
	updateAttackerStatus();
	updateWalletStatus();


    C_Frame *top = new C_Frame("topMenu");
    top->addPage(new C_Menu_Top("topMenu"));
    m_frames.push_back(top);

    C_Frame *bottom = new C_Frame("bottomMenu");
    bottom->addPage(new C_Menu_Bottom("bottomMenu"));
    m_frames.push_back(bottom);

    C_Frame *end = new C_Frame("endGame");
    end->addPage(new C_Tab_endGame("endGame"));
    m_frames.push_back(end);


	C_Message::printM("Constructor C_Menu() : done\n");
}

C_Menu::~C_Menu()
{
	for(auto const& x : m_menuItemsList) {
		if(x.second != nullptr)
			delete  x.second;
	}

	for(auto const& f : m_frames) {
		if(f != nullptr)
			delete  f;
	}
}

C_Frame* C_Menu::getFrame(std::string name){
	C_Frame* ret = nullptr;
	for(auto const& f : m_frames) {
		if(f != nullptr){
	        if(f->getName() == name){
	            ret = f;
	        }
		}
	}
    return ret;
}


void C_Menu::updateInfos()
{
	updateDefenderStatus();
	updateAttackerStatus();
	updateWalletStatus();
	updateUpgradeButtonsStatus();
}

void C_Menu::render()
{
	for(auto const& f : m_frames) {
		if(f != nullptr){
			f->refresh();
			f->render();
		}
	}

	vector<string>  list = getMenuItemsList();
	//draw all buttons, layer by layer;
	for(int j = BACK; j <= FRONT; j++) {
		for(size_t i = 0; i < list.size(); i++) {
			C_MenuItem * b = getMenuItem(list[i]);
			if( b != nullptr) {
				if(b->getLayer() == j) {
					b->render();
				}
			}
		}
	}
}


void C_Menu::updateLevelInfos(int current_wave, int total_waves)
{
	m_current_wave = current_wave;
	m_total_waves = total_waves;
}

void C_Menu::resetValues()
{
	m_current_wave = 1;
	m_total_waves = 1;
	updateAttackerStatus();
	updateDefenderStatus();
	if(getFrame("mainMenu") !=  nullptr)
		getFrame("mainMenu")->setOpen(false);
	if(getFrame("endGame") !=  nullptr)
		getFrame("endGame")->setOpen(false);
	if(getFrame("bottomMenu") !=  nullptr)
		getFrame("bottomMenu")->setOpen(true);
	if(getFrame("topMenu") !=  nullptr)
		getFrame("topMenu")->setOpen(true);
}


void C_Menu::updateDefenderStatus()
{
	C_Grid& grid= C_Locator::getGrid();
	int playerLife = grid.getAllTownsLifeLevel();

	C_Settings& settings=C_Locator::getSettings();
	int x = settings.getWindowWidth();


	//progress bar value
	if(m_menuItemsList["playerlife"] == nullptr) {
		m_menuItemsList["playerlife"] = new C_GP_Status("playerlife",x - 200,40, GREEN, RED);
	}
	string text = "Life: " + nbrToString(playerLife);
	if(m_menuItemsList["playerlife"] != nullptr) {
		m_menuItemsList["playerlife"]->setPercentage(playerLife);
		m_menuItemsList["playerlife"]->setText(text, 18);
	} else {
		C_Message::printM("the progress bar playerlife does not exist");
	}
}


void C_Menu::updateAttackerStatus()
{
	if(m_menuItemsList["boatLife"] == nullptr) {
		m_menuItemsList["boatLife"] = new C_GP_Status("boatLife",60,40, GREEN,BLUE);
	}
	if(m_menuItemsList["boatLife"] != nullptr) {
		string text = "Wave " + to_string(m_total_waves - m_current_wave +1) + "/" + to_string(m_total_waves);
		m_menuItemsList["boatLife"]->setPercentage(m_current_wave,m_total_waves);
		m_menuItemsList["boatLife"]->setText(text, 18);
	}
}

void C_Menu::updateWalletStatus()
{
	C_Settings& settings=C_Locator::getSettings();
	int x = settings.getWindowWidth();
	C_Wallet& wallet= C_Locator::getWallet();
	if(m_menuItemsList["gold_pile"] == nullptr) {
		m_menuItemsList["gold_pile"] = new C_MenuItem("gold_pile",x - 218,40);
	}
	//progress bar
	if(m_menuItemsList["walletBar"]== nullptr) {
		m_menuItemsList["walletBar"] = new C_GP_Status("walletBar",x - 168,80, GREEN, BLUE);
	}
	if(m_menuItemsList["walletBar"] != nullptr) {
		string text = nbrToString(wallet.getBalance());
		m_menuItemsList["walletBar"]->setPercentage(wallet.getBalance(),wallet.getWalletMax());
		m_menuItemsList["walletBar"]->setText(text, 18);
	}
}


string C_Menu::nbrToString(int nbr)
{
	string space = "";
	string nbrStr = to_string(nbr);

	if (nbrStr.size()==2) {
		space = "  ";
	} else if (nbrStr.size()==1) {
		space = "    ";
	} else {
		space = "";
	}
	return space + nbrStr;
}


void C_Menu::updateUpgradeButtonsStatus(){
	C_Grid& grid= C_Locator::getGrid();
	C_GameUnits * unit = grid.getSelected();
	if(unit != nullptr){
		if(m_menuItemsList["upgradeTower"] != nullptr){
			if(grid.isUnitupgradable(unit)){
				m_menuItemsList["upgradeTower"]->setEnable(true);
			} else {
				m_menuItemsList["upgradeTower"]->setEnable(false);
			}
		}
	}
}



void C_Menu::openMainMenu()
{
	C_Settings& settings=C_Locator::getSettings();
	C_Frame* main = getFrame("mainMenu");
	if(main != nullptr){
	    if(main->getOpen()) {
		    main->setOpen(false);
	        if(getFrame("topMenu") !=  nullptr)
		        getFrame("topMenu")->setOpen(true);
		    if(getFrame("bottomMenu") !=  nullptr)
		        getFrame("bottomMenu")->setOpen(true);
		    settings.setPlaying(PLAY);
	    } else {
		    main->setOpen(true);
	        if(getFrame("topMenu") !=  nullptr)
		        getFrame("topMenu")->setOpen(false);
		    if(getFrame("bottomMenu") !=  nullptr)
		        getFrame("bottomMenu")->setOpen(false);
		    settings.setPlaying(PAUSE);
	    }
	}
}
void C_Menu::openEndLevelMenu(int status)
{
	C_Settings& settings=C_Locator::getSettings();
	C_Frame * tmp = getFrame("endGame");
	if(tmp != nullptr){
	    if(tmp->getOpen()) {
		    tmp->setOpen(false);
		    settings.setPlaying(PLAY);
	    } else {
		    tmp->setOpen(true);
		    settings.setPlaying(PAUSE);
	    }
    //tmp->setWin(status);
	}
}
void C_Menu::resetEndLevelMenu(){
    C_Frame * tmp = getFrame("endGame");
	if(tmp != nullptr){
		delete tmp;
	}

	tmp = new C_Frame("endGame");
    tmp->addPage(new C_Tab_endGame("endGame"));

	C_Settings& settings=C_Locator::getSettings();
	settings.setPlaying(PLAY);
}


vector<string> C_Menu::getMenuItemsList()
{
	vector<string> list;
	//Always Visible
	list.push_back("boatLife");
	list.push_back("playerlife");
	list.push_back("gold_pile");
	list.push_back("walletBar");

	C_Grid& grid= C_Locator::getGrid();
	C_GameUnits * unit = grid.getSelected();
	if(unit != nullptr){
	    list.push_back("upgradeTower");
	    S_Coord coord = unit->getScreen();
	    C_Settings& settings= C_Locator::getSettings();
	    coord.x -=32;
	    coord.y -= settings.getTileHeight()*3;
	    m_menuItemsList["upgradeTower"]->setScreen(coord);
    }

    for(auto const& f : m_frames) {
		if(f != nullptr){
			std::vector<std::string> tmp = f->getListOfVisibleItems();
		    list.insert(list.end(), tmp.begin(), tmp.end());
		}
	}

	return list;
}


void C_Menu::menuBanner()
{
	C_Frame *main = new C_Frame("mainMenu");
	if(main != nullptr){
	    main->setOpen(false);
        main->addPage( new C_Tab_Levels());
        main->addPage( new C_Tab_Settings());
        main->addPage( new C_Tab("About"));
        m_frames.push_back(main);
    }

    //
    for(auto const& f : m_frames) {
		if(f != nullptr){
			std::map<std::string, C_MenuItem*> items = f->getItemList();
		    m_menuItemsList.insert(items.begin(),items.end());
		}
	}
}


void C_Menu::go(int direction){
    if(getFrame("mainMenu") != nullptr){
        if(getFrame("mainMenu")->getCurrent() != nullptr){
            getFrame("mainMenu")->getCurrent()->go(direction);
         }
    }
}




