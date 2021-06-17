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
	m_total_waves(1),
	m_bottomMenuOpen(false)
{
	C_Message::printM("Constructor C_Menu() : start\n");
	C_Settings& settings=C_Locator::getSettings();
	int size = 64 + 30;
	int x_button = settings.getWindowWidth() - size;
	int y_button = settings.getWindowHeight()/2 - size;
	//left buttons
	m_menuItemsList["AddTower"] = new C_GB_AddUnit("AddTower","AddTower",x_button,y_button);
	y_button +=size;
	m_menuItemsList["AddCatapult"] = new C_GB_AddUnit("AddCatapult","AddCatapult",x_button,y_button);
	y_button +=size;
	m_menuItemsList["AddBarricade"] = new C_GB_AddUnit("AddBarricade","AddBarricade",x_button,y_button);
	S_Coord upgradeCoord = {10,10};
	m_menuItemsList["upgradeTower"] = new C_GU_Upgrade("upgradeTower",upgradeCoord);

	updateDefenderStatus();
	updateAttackerStatus();
	updateWalletStatus();

	//add the bottom buttons line
	S_Coord line;
	line.x = 20;
	line.y = settings.getWindowHeight() - 20;
	bottomButtonsLine(line);
	m_endGameMenu =  new C_Tab_endGame("endGame");

	C_Message::printM("Constructor C_Menu() : done\n");
}

C_Menu::~C_Menu()
{
	for(auto const& x : m_menuItemsList) {
		if(x.second != nullptr)
			delete  x.second;
	}

	for(auto const& tab : m_tabs) {
		if(tab != nullptr)
			delete  tab;
	}
	delete m_endGameMenu;
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
	displayBottomMenu();
	m_endGameMenu->refresh();
	m_endGameMenu->render();
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
	if(m_endGameMenu != nullptr)
		m_endGameMenu->setOpen(false);
}


void C_Menu::updateDefenderStatus()
{
	C_Grid& grid= C_Locator::getGrid();
	int playerLife = grid.getAllTownsLifeLevel();

	C_Settings& settings=C_Locator::getSettings();
	int x = settings.getWindowWidth();

	if(m_menuItemsList["Characters_fox"] == nullptr) {
		m_menuItemsList["Characters_fox"] = new C_MenuItem("Characters_fox",x - 92,20);
	}

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
	if (m_menuItemsList["Characters_lion"] == nullptr) {
		m_menuItemsList["Characters_lion"] = new C_MenuItem("Characters_lion",20,30);
	}
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



void C_Menu::openBottomMenu()
{
	C_Settings& settings=C_Locator::getSettings();
	if(m_bottomMenuOpen) {
		m_bottomMenuOpen = false;
		settings.setPlaying(PLAY);
	} else {
		m_bottomMenuOpen = true;
		settings.setPlaying(PAUSE);
	}
}
void C_Menu::openEndLevelMenu(int status)
{
	C_Settings& settings=C_Locator::getSettings();
	if(m_endGameMenu->getOpen()) {
		m_endGameMenu->setOpen(false);
		settings.setPlaying(PLAY);
	} else {
		m_endGameMenu->setOpen(true);
		settings.setPlaying(PAUSE);
	}
	m_endGameMenu->setWin(status);
}
void C_Menu::resetEndLevelMenu(){
	if(m_endGameMenu != nullptr){
		delete m_endGameMenu;
	}
	m_endGameMenu = new C_Tab_endGame("endGame");
	C_Settings& settings=C_Locator::getSettings();
	settings.setPlaying(PLAY);
}


void C_Menu::displayBottomMenu()
{
	m_tabs[0]->displayTab(m_bottomMenuOpen);
}


vector<string> C_Menu::getMenuItemsList()
{
	vector<string> list;
	//Always Visible
	list.push_back("Characters_lion");
	list.push_back("boatLife");
	list.push_back("Characters_fox");
	list.push_back("playerlife");
	list.push_back("gold_pile");
	list.push_back("walletBar");
	list.push_back("popOutMenu");
	list.push_back("home");
	list.push_back("play");

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

	if(m_bottomMenuOpen) {
		//get tab selector buttons
		for (size_t i = 0; i < m_tabs.size() ; i++) {
			list.push_back(m_tabs[i]->getName());
		}
		//get the list of active button from the selected tab
		vector<string> tmp = m_tabs[m_currentTab]->getListOfVisibleItems();
		list.insert(list.end(), tmp.begin(), tmp.end());

	} else {
		list.push_back("AddTower");
		list.push_back("AddBarricade");
		list.push_back("AddCatapult");
	}

	if(m_endGameMenu != nullptr){
	    std::vector<std::string> tmp = m_endGameMenu->getListOfVisibleItems();
		list.insert(list.end(), tmp.begin(), tmp.end());
	}
	return list;
}


void C_Menu::menuBanner()
{
	m_tabs.push_back( new C_Tab_Levels());
	m_tabs.push_back( new C_Tab_Settings());
	m_tabs.push_back( new C_Tab("About"));

    //
    if(m_endGameMenu != nullptr){
        std::map<std::string, C_MenuItem*> items;
		items = m_endGameMenu->getItemList();
		m_menuItemsList.insert(items.begin(),items.end());
    }


	//declare buttons from tabs into the mainItemList
	for (size_t i = 0; i < m_tabs.size() ; i++) {
		std::map<std::string, C_MenuItem*> tabItems;
		tabItems = m_tabs[i]->getItemList();
		m_menuItemsList.insert(tabItems.begin(),tabItems.end());
	}
	setTabNbr(0); //set the focus on the first tab
}

void C_Menu::setTabNbr(int nbr)
{
	m_currentTab = nbr;

}

void C_Menu::go(int direction){
    m_tabs[m_currentTab]->go(direction);
}

void C_Menu::bottomButton(const string &name,S_Coord screen)
{
	if(m_menuItemsList[name] == nullptr) {
		m_menuItemsList[name] = new C_Button(name,name,screen.x,screen.y);
		C_Command *command = nullptr;
		if(name ==  "popOutMenu") {
			command = new C_OpenMenu();
		} else if(name ==  "home") {
			command = new C_CenterCamera();
		} else if(name ==  "play") {
			command = new C_Play();
		}
		m_menuItemsList[name]->setCommand(command);
		command = nullptr;
	}
}

void C_Menu::bottomButtonsLine(S_Coord screen)
{
	S_Coord pos;
	int buttonSize = 64;
	int space = 10;
	pos.x = screen.x;
	pos.y = screen.y - buttonSize;
	bottomButton("popOutMenu", pos);
	pos.x += buttonSize + space;
	bottomButton("home", pos);
	pos.x += buttonSize + space;
	bottomButton("play", pos);
}


