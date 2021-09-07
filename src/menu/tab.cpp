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

#include "tab.h"
#include "../message.h"
#include "../window.h"
#include "../textureList.h"
#include "../locator.h"
#include "../tools.h"

using namespace std;

C_Frame::C_Frame(std::string name):
    m_name(name)
{
}

C_Frame::~C_Frame(){
    for(auto const& i : m_list) {
		if(i != nullptr)
			delete  i;
	}
}



void C_Frame::addPage(C_Page *page){
    if(page != nullptr){
        for(auto i : m_list){
            if(page->getName() == i->getName()){
                std::cout << "Page " << page->getName() << " exist allready." << std::endl;
            }
        }
        m_list.push_back(page);
    }
}

void C_Frame::refresh(){
    for(auto const& i : m_list) {
		if(i != nullptr)
			i->refresh();
	}
}

void C_Frame::render(){
	if(m_open){
        for(auto const& i : m_list) {
		    if(i != nullptr){
			        i->render();
			    }
	    }
	}
}


std::vector<std::string> C_Frame::getListOfVisibleItems(){
    std::vector<std::string> ret;
    if(m_open){
        for(auto const& i : m_list) {
		    if(i != nullptr){
	                ret = i->getListOfVisibleItems();
		    };
		}
	}
	return ret;
}

std::map<std::string, C_MenuItem*> C_Frame::getItemList(){
    std::map<std::string, C_MenuItem*> ret;
	if(m_open){
        for(auto const& i : m_list) {
    		if(i != nullptr){
	            ret = i->getItemList();
		    };
		}
	}
	return ret;
}

C_Page* C_Frame::getCurrent(){
    C_Page* ret = nullptr;
    if(m_list.size()<= m_currentPage){
       ret = m_list[m_currentPage];
    }
	return ret;
}

C_Page::C_Page(std::string name):
    m_name(name)
{
	m_width = 768;
	m_height = 512;
}

std::vector<std::string> C_Page::getListOfVisibleItems()
{
	std::vector <std::string> list;
    if(m_open){
	    for(auto const& x : m_itemsList) {
		    list.push_back(x.first);
	    }
	}
	return list;
}

C_Page::~C_Page()
{
    m_itemsList.clear(); //items are allready delete
}

void C_Page::flagLine(std::vector <std::string> names, S_Coord last)
{
        int size = 48;
	    //left buttons
	    int offset = 0;
	    for(auto i : names){
	        if(!m_itemsList[i]){
	            std::string img = i;
	            C_Command *command = nullptr;
	            if(i ==  "popOutMenu" || i ==  "popOutMenu2" ) {
			        command = new C_OpenMenu();
			        img = "popOutMenu";
		        } else if(i ==  "home") {
			        command = new C_CenterCamera();
		        } else if(i ==  "play") {
			        command = new C_Play();
		        } else if(i ==  "quit") {
			        command = new C_QuitProgram();
		        }
	            m_itemsList[i] = new C_Button(i,img,last.x - offset , last.y);
		        if(m_itemsList[i]!= nullptr){
		            if(command){
		                m_itemsList[i]->setCommand(command);
		            }
	            }
	        }
	        offset += size;
	    }
}


int C_Tab::m_id = -1;

C_Tab::C_Tab(std::string title)
    : C_Page(title)
{
	m_id++;
	m_name = "tab_" + to_string(m_id);
	m_title = title;
	C_Settings& settings=C_Locator::getSettings();
	m_width = 768;
	m_height = 512;
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_flagScreen.x = m_screen.x;
	m_flagScreen.y = m_screen.y - m_height/3 + 20;
	m_flagOffset = 35;


	m_tabSize = 30;
	int x = m_screen.x - ((m_tabSize *3) / 2) + 35;

	m_itemsList[m_name] = new C_MB_TabSelect(m_name,m_title,22, m_screen.x - m_width/3 - 20  ,m_screen.y - m_height/3 + (m_flagOffset*m_id) + 20 );
	if(m_itemsList[m_name] != nullptr) {
		m_itemsList[m_name]->setCommand(new C_ChangeTab);
		if( m_itemsList[m_name]->getCommand() != nullptr)
			m_itemsList[m_name]->getCommand()->setNbr(m_id);
	}


	std::vector <string> names = {"popOutMenu2", "quit"};
    S_Coord first = {m_screen.x + m_width/3 - 30,m_screen.y + m_height/4};
    flagLine(names,first);
}


void C_Tab::displayTab(bool open)
{
	if(open) {
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Menu_01_background", m_screen.x - 10,m_screen.y,CENTER);
	}
}



std::vector<string> C_Tab::getListOfVisibleItems()
{
	std::vector <std::string> list;
	for(auto const& x : m_itemsList) {
		if(x.first != m_name)
			list.push_back(x.first);
	}
	return list;
}

C_Tab_Settings::C_Tab_Settings()
	:C_Tab("Settings")
{
	std::string name = "Resolution";
	C_Settings& settings=C_Locator::getSettings();
	std::string width = std::to_string(settings.getWindowWidth());
	std::string height = std::to_string(settings.getWindowHeight());
	std::string text = width + "x" + height;
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x - 128,m_flagScreen.y);

	name = "Grid Size";
	text =  std::to_string(settings.getGridWidth())+ "x" +std::to_string(settings.getGridHeight());
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x - 128,m_flagScreen.y + m_flagOffset);
}

C_Tab_Levels::C_Tab_Levels()
	:C_Tab("Levels")
{
	C_Settings& settings=C_Locator::getSettings();
	std::string name = "Number of Levels";
	std::string text = std::to_string(settings.getNbrOfLevels());
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x - 128,m_flagScreen.y);

    m_currentCardLevelNbr = settings.getCurrentLevelNbr();
	name = "Card_Level";
	m_itemsList[name] = new C_MB_LevelCard(m_currentCardLevelNbr,"Card_" + to_string(m_currentCardLevelNbr),m_flagScreen.x + 82,m_screen.y -32);

	std::string arrowLeft = "Level_Change_Arrow_Left";
	m_itemsList[arrowLeft]  = new C_MB_Arrows(arrowLeft,GO_LEFT,m_flagScreen.x - 48 ,m_screen.y - 32);
	m_itemsList[arrowLeft]->setCommand(new C_ChangeLevelLeft());

	std::string arrowRight = "Level_Change_Arrow_Right";
	m_itemsList[arrowRight]  = new C_MB_Arrows(arrowRight,GO_RIGHT,m_flagScreen.x + 192,m_screen.y - 32);
	m_itemsList[arrowRight]->setCommand(new C_ChangeLevelRight());

	std::string load = "Level_Load";
	m_itemsList[load]  = new C_MB_CardButton(load, m_flagScreen.x - 10, m_screen.y + 64);
	if(m_itemsList[load]!= nullptr){
	    m_itemsList[load]->setText("Load");
	    C_LoadALevel *command = new C_LoadALevel();
	    m_itemsList[load]->setCommand(command);
	    m_itemsList[load]->getCommand()->setNbr(m_currentCardLevelNbr);
    }

}

void C_Tab_Levels::go(int direction){
    if(direction == GO_LEFT){
        m_currentCardLevelNbr--;
    } else if (direction == GO_RIGHT){
        m_currentCardLevelNbr++;
    }
    //limits
    if(m_currentCardLevelNbr < 1){
        m_currentCardLevelNbr = 1;
    }
    C_Window& win= C_Locator::getWindow();
    int last = win.getLastLevelNbr();
    if(m_currentCardLevelNbr > last){
        m_currentCardLevelNbr = last;
    }

    std::string name = "Card_" + to_string(m_currentCardLevelNbr);

    m_itemsList.erase("Card_Level");
    m_itemsList["Card_Level"] = new C_MB_LevelCard(m_currentCardLevelNbr,name,m_screen.x,m_screen.y);
    m_itemsList["Level_Load"]->getCommand()->setNbr(m_currentCardLevelNbr);
}


C_Tab_endGame::C_Tab_endGame(std::string name)
	:C_Page(name)
{
	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_levelStatus = ONGOING;
	m_open = false;

    std::string replay = "EndGame_Replay";
	m_itemsList[replay]  = new C_MB_CardButton(replay, m_screen.x, m_screen.y - 20);
	if(m_itemsList[replay] != nullptr){
	    C_LoadALevel *command = new C_LoadALevel();
	    m_itemsList[replay]->setCommand(command);
	}

	std::string next = "EndGame_Next";
	m_itemsList[next]  = new C_MB_CardButton(next, m_screen.x, m_screen.y + 30);
	if(m_itemsList[next]!= nullptr){
	    C_LoadALevel *command1 = new C_LoadALevel();
	    m_itemsList[next]->setCommand(command1);
	}

	std::string quit = "EndGame_Quit";
	m_itemsList[quit]  = new C_MB_CardButton(quit, m_screen.x, m_screen.y + 80);
	if(m_itemsList[quit]!= nullptr){
	    C_QuitProgram *command2 = new C_QuitProgram();
	    m_itemsList[quit]->setCommand(command2);
	    m_itemsList[quit]->setText("Quit");
	}

	m_itemsList["EndGameResultText"]  = new C_MenuText("EndGameResultText","Winner", m_screen.x  +  92, m_screen.y - 80 );
	refresh();
}


void C_Tab_endGame::refresh(){
    std::string replay = "EndGame_Replay";
    C_Settings& settings=C_Locator::getSettings();
    int current = settings.getCurrentLevelNbr();
    if(m_itemsList[replay] != nullptr){
	        m_itemsList[replay]->setText("Replay level :" + to_string(current));
	        if(m_itemsList[replay]->getCommand() != nullptr){
	        	   m_itemsList[replay]->getCommand()->setNbr(current);
	        }
	}
	std::string next = "EndGame_Next";
	int nextLevel = current + 1;
	if (nextLevel > settings.getNbrOfLevels()){
	    nextLevel = 1;
	}
	if(m_itemsList[next] != nullptr){
	    m_itemsList[next]->setText("Next level : " + to_string(nextLevel));
	       if(m_itemsList[next]->getCommand() != nullptr){
	        	  m_itemsList[next]->getCommand()->setNbr(nextLevel);
	        }
    }
}

void C_Tab_endGame::render()
{
    if(m_open){
	    C_TextureList& t= C_Locator::getTextureList();
	    t.renderTexture("Menu_01_background", m_screen.x - 10, m_screen.y,CENTER);
	    if(m_levelStatus == WIN)
			    m_itemsList["EndGameResultText"]->setText("Your castle is safe for now");
		    else if(m_levelStatus == LOSE)
			    m_itemsList["EndGameResultText"]->setText("You lost this castle");
		    else if(m_levelStatus == ONGOING)
			    m_itemsList["EndGameResultText"]->setText("The game continue");
	}
}

C_Menu_Bottom::C_Menu_Bottom(std::string name)
	:C_Page(name)
{
	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight());

	std::string text = "Add";
	m_itemsList[text]  = new C_MB_CardButton(text, m_screen.x - 320, m_screen.y - 50);
	if(m_itemsList[text] != nullptr){
	    m_itemsList[text]->setText("Add");
	}

	int size = 64 + 20;
	int x_button = m_screen.x - 100;
	int y_button = m_screen.y - 70;
	//left buttons
	m_itemsList["AddTower"] = new C_GB_AddUnit("AddTower","AddTower",x_button,y_button);
	x_button +=size;
	m_itemsList["AddCatapult"] = new C_GB_AddUnit("AddCatapult","AddCatapult",x_button,y_button);
	x_button +=size;
	m_itemsList["AddBarricade"] = new C_GB_AddUnit("AddBarricade","AddBarricade",x_button,y_button);
}

void C_Menu_Bottom::render(){
    if(m_open){
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Menu_01_background", m_screen.x,m_screen.y + 100,CENTER);
	}
}


C_Menu_Top::C_Menu_Top(std::string name)
	:C_Page(name)
{

	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = 30;
    if(m_open){
        std::vector <string> names = {"popOutMenu","home","play"};
        S_Coord first = {m_screen.x + m_width/3 - 30,m_screen.y+28};
        flagLine(names,first);
	}
	int x_button = m_screen.x - 100;
	int y_button = m_screen.y - 30;
	if(m_itemsList["gold_pile"]== nullptr) {
        m_itemsList["gold_pile"] = new C_MenuItem("gold_pile",x_button,y_button);
    }
    if(m_itemsList["walletBar"]== nullptr) {
		m_itemsList["walletBar"] = new C_GP_Status("walletBar",x_button + 70,y_button + 30, GREEN, BLUE);
	}
    //progress bar value
	if(m_itemsList["playerlife"] == nullptr) {
		m_itemsList["playerlife"] = new C_GP_Status("playerlife",x_button + 70,y_button + 10, GREEN, RED);
	}

	if(m_itemsList["waveCount"] == nullptr) {
		m_itemsList["waveCount"] = new C_GP_Status("waveCount",x_button - 170,y_button + 10, GREEN,BLUE);
	}
}

void C_Menu_Top::refresh(){
    if(m_open){
        if(m_itemsList["walletBar"] != nullptr) {
		    C_Wallet& wallet= C_Locator::getWallet();
		    std::string text = C_Tools::nbrToString(wallet.getBalance());
		    m_itemsList["walletBar"]->setPercentage(wallet.getBalance(),wallet.getWalletMax());
		    m_itemsList["walletBar"]->setText(text, 18);
	    }

	    if(m_itemsList["playerlife"] != nullptr) {
	        C_Grid& grid= C_Locator::getGrid();
	        int playerLife = grid.getAllTownsLifeLevel();
	    	string text = "Life: " + C_Tools::nbrToString(playerLife);
		    m_itemsList["playerlife"]->setPercentage(playerLife);
		    m_itemsList["playerlife"]->setText(text, 18);
	    }

	    if(m_itemsList["waveCount"] != nullptr) {
	        C_Window& win=C_Locator::getWindow();
	        S_LevelData l = win.getCurrentLevel()->getData();
		    std::string text = "Wave " + std::to_string(l.currentWave  + 1) + "/" + to_string(l.totalWaves);
		    m_itemsList["waveCount"]->setPercentage(l.currentWave,l.totalWaves);
		    m_itemsList["waveCount"]->setText(text, 18);
	    }

    }
}

void C_Menu_Top::render(){
    if(m_open){
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Menu_01_background", m_screen.x,m_screen.y - 100,CENTER);
	}
}


