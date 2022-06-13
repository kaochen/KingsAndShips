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
#include <libintl.h>
#include <locale.h>

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
        if(m_list.empty()){
            m_current = page->getName();
        }
        for(auto i : m_list){
            if(page->getName() == i->getName()){
                std::cout << "Page " << page->getName() << " exist already." << std::endl;
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
		        if(i->getName()== m_current){
			            i->render();
			        }
			    }
	    }
	}
}

std::map<std::string, C_MenuItem*> C_Frame::getItemList(){
    std::map<std::string, C_MenuItem*> ret;
	if(m_open){
        for(auto const& i : m_list) {
    		if(i != nullptr){
    		    if(i->getName()== m_current){
	                ret = i->getItemList();
	            }
		    };
		}
	}
	return ret;
}

C_Page* C_Frame::getCurrent(){
    C_Page* ret = nullptr;
    for(auto const& i : m_list) {
    		if(i != nullptr){
    		    if(i->getName()== m_current){
                    ret = i;
	            }
		    };
	}
	return ret;
}

void C_Frame::openTabIfExist(std::string tabname){
    for(auto const& i : m_list) {
    		if(i != nullptr){
    		    if(i->getName()== tabname){
	                m_open = true;
	                m_current = tabname;
	            }
		    }
		}
}



C_Page::C_Page(std::string name):
    m_name(name)
{
	m_width = 768;
	m_height = 512;
}

C_Page::~C_Page()
{
    m_itemsList.clear(); //items are already delete
}

void C_Page::flagLine(std::vector <std::string> names, S_Coord last)
{
        int size = 64;
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

void C_Page::column(std::vector <std::string> names, S_Coord first)
{
        int size = 32;
	    //left buttons
	    int offset = 0;
	    for(auto i : names){
	        std::string uniqueName = m_name+"_"+i;
	        if(m_itemsList[uniqueName] == nullptr){
	            C_Command *command = new C_ChangeTab();
	            command->setText(i);
	            m_itemsList[uniqueName]  = new C_MB_CardButton(i,first.x , first.y + offset);
		        if(m_itemsList[uniqueName]!= nullptr){
	                m_itemsList[uniqueName]->setText(i);
		            if(command){
		                m_itemsList[uniqueName]->setCommand(command);
		            }
	            }
	        }
	        offset += size;
	    }
}

void C_Page::render(){
        //render each items
		for(auto const& i : m_itemsList) {
		    if(i.second != nullptr)
			    i.second->render();
	    }
}

void C_Page::renderBackgoundTop(S_Coord coord){
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("Menu_01_background_1", coord.x-320,coord.y,CENTER);
	t.renderTexture("Menu_01_background_2", coord.x-192,coord.y,CENTER);
	t.renderTexture("Menu_01_background_2", coord.x- 128,coord.y,CENTER);
	t.renderTexture("Menu_01_background_2", coord.x,coord.y,CENTER);
	t.renderTexture("Menu_01_background_2", coord.x + 128,coord.y,CENTER);
	t.renderTexture("Menu_01_background_3", coord.x + 192,coord.y,CENTER);
	t.renderTexture("Menu_01_background_4", coord.x+320,coord.y,CENTER);
}

void C_Page::renderBackgoundMiddle(S_Coord coord){
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("Menu_01_background_5", coord.x-320,coord.y,CENTER);
	t.renderTexture("Menu_01_background_6", coord.x-192,coord.y,CENTER);
	t.renderTexture("Menu_01_background_6", coord.x- 128,coord.y,CENTER);
	t.renderTexture("Menu_01_background_6", coord.x,coord.y,CENTER);
	t.renderTexture("Menu_01_background_6", coord.x + 128,coord.y,CENTER);
	t.renderTexture("Menu_01_background_6", coord.x + 192,coord.y,CENTER);
	t.renderTexture("Menu_01_background_8", coord.x+320,coord.y,CENTER);
}

void C_Page::renderBackgoundBottom(S_Coord coord){
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("Menu_01_background_9", coord.x-320,coord.y,CENTER);
	t.renderTexture("Menu_01_background_11", coord.x-210,coord.y,CENTER);
	t.renderTexture("Menu_01_background_10", coord.x- 128,coord.y,CENTER);
	t.renderTexture("Menu_01_background_10", coord.x,coord.y,CENTER);
	t.renderTexture("Menu_01_background_10", coord.x + 128,coord.y,CENTER);
	t.renderTexture("Menu_01_background_10", coord.x+192,coord.y,CENTER);
	t.renderTexture("Menu_01_background_12", coord.x+320,coord.y,CENTER);
}



C_Tab::C_Tab(std::string title)
    : C_Page(title)
{
	C_Settings& settings=C_Locator::getSettings();
	m_width = 768;
	m_height = 512;
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_flagScreen.x = m_screen.x;
	m_flagScreen.y = m_screen.y - m_height/3 + 20;
	m_flagOffset = 35;

	std::vector <std::string> names = {"popOutMenu2", "quit"};
    S_Coord first = {m_screen.x + m_width/3 - 30,m_screen.y + m_height/3 + 20};
    flagLine(names,first);

    std::vector <std::string> columnNames = {"Status","Levels","Settings", "About"};
    S_Coord first_c = {m_screen.x - m_width/2 + 40,m_screen.y - m_height/3 + 20};
    column(columnNames,first_c);
}

void C_Tab::render(){
		C_TextureList& t= C_Locator::getTextureList();
		//background first line
		S_Coord coord = {m_screen.x, m_screen.y -184};
        renderBackgoundTop(coord);
		//render second line twice
		for(size_t i = 1 ; i <=2 ; i++){
		    coord.y += 128;
            renderBackgoundMiddle(coord);
		}
		//background bottom line
		coord.y += 128;
        renderBackgoundBottom(coord);

		t.renderTexture("Menu_01_vflag_1", m_screen.x - 130,m_screen.y-138,CENTER);
		t.renderTexture("Menu_01_vflag_2", m_screen.x - 130,m_screen.y -10,CENTER);
		t.renderTexture("Menu_01_vflag_3", m_screen.x - 130,m_screen.y+118,CENTER);
		C_Page::render();
}

C_Tab_Settings::C_Tab_Settings()
	:C_Tab("Settings")
{
	std::string name = "Resolution";
	C_Settings& settings=C_Locator::getSettings();
	std::string width = std::to_string(settings.getWindowWidth());
	std::string height = std::to_string(settings.getWindowHeight());
	std::string text = width + "x" + height;
	m_itemsList["Resolution"] = new C_MB_1Line(gettext("Resolution"),text,m_flagScreen.x - 128,m_flagScreen.y);

	text =  std::to_string(settings.getGridWidth())+ "x" +std::to_string(settings.getGridHeight());
	m_itemsList["Grid Size"] = new C_MB_1Line(gettext("Grid Size"),text,m_flagScreen.x - 128,m_flagScreen.y + m_flagOffset);
}

C_Tab_Levels::C_Tab_Levels()
	:C_Tab("Levels")
{
	C_Settings& settings=C_Locator::getSettings();
	std::string text = std::to_string(settings.getNbrOfLevels());
	m_itemsList["Number of Levels"] = new C_MB_1Line(gettext("Number of Levels"),text,m_flagScreen.x - 128,m_flagScreen.y);

    m_currentCardLevelNbr = settings.getCurrentLevelNbr();
	m_itemsList["Card_Level"] = new C_MB_LevelCard(m_currentCardLevelNbr,"Card_" + to_string(m_currentCardLevelNbr),m_flagScreen.x + 82,m_screen.y -32);

	std::string arrowLeft = "Level_Change_Arrow_Left";
	m_itemsList[arrowLeft]  = new C_MB_Arrows(arrowLeft,GO_LEFT,m_flagScreen.x - 48 ,m_screen.y - 32);
	m_itemsList[arrowLeft]->setCommand(new C_ChangeLevelLeft());

	std::string arrowRight = "Level_Change_Arrow_Right";
	m_itemsList[arrowRight]  = new C_MB_Arrows(arrowRight,GO_RIGHT,m_flagScreen.x + 192,m_screen.y - 32);
	m_itemsList[arrowRight]->setCommand(new C_ChangeLevelRight());

	std::string load = "Level_Load";
	m_itemsList[load]  = new C_MB_CardButton(load, m_flagScreen.x - 10, m_screen.y + 64);
	if(m_itemsList[load]!= nullptr){
	    m_itemsList[load]->setText(gettext("Load"));
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
    C_Settings& settings=C_Locator::getSettings();
	int last = settings.getNbrOfLevels();
    if(m_currentCardLevelNbr > last){
        m_currentCardLevelNbr = last;
    }

    std::string name = "Card_" + to_string(m_currentCardLevelNbr);

    m_itemsList.erase("Card_Level");
    m_itemsList["Card_Level"] = new C_MB_LevelCard(m_currentCardLevelNbr,name,m_flagScreen.x + 82,m_screen.y -32);
    if(m_itemsList["Level_Load"] != nullptr){
        if( m_itemsList["Level_Load"]->getCommand() != nullptr){
        m_itemsList["Level_Load"]->getCommand()->setNbr(m_currentCardLevelNbr);
     }
    }
}


C_Tab_Status::C_Tab_Status()
	:C_Tab("Status")
{
	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;

    std::string replay = "status_Replay";
	m_itemsList[replay]  = new C_MB_CardButton(replay, m_screen.x, m_screen.y - 20);
	if(m_itemsList[replay] != nullptr){
	    C_LoadALevel *command = new C_LoadALevel();
	    m_itemsList[replay]->setCommand(command);
	}

	std::string next = "status_Next";
	m_itemsList[next]  = new C_MB_CardButton(next, m_screen.x, m_screen.y + 30);
	if(m_itemsList[next]!= nullptr){
	    C_LoadALevel *command1 = new C_LoadALevel();
	    m_itemsList[next]->setCommand(command1);
	}

	std::string quit = "status_Quit";
	m_itemsList[quit]  = new C_MB_CardButton(quit, m_screen.x, m_screen.y + 80);
	if(m_itemsList[quit]!= nullptr){
	    C_QuitProgram *command2 = new C_QuitProgram();
	    m_itemsList[quit]->setCommand(command2);
	    m_itemsList[quit]->setText(gettext("Quit"));
	}

	m_itemsList["statusResultText"]  = new C_MenuText("statusResultText","Winner", m_screen.x  +  92, m_screen.y - 40 );
	S_Coord starPos {m_screen.x  +  10 , m_screen.y - 130  };
	m_itemsList["gold_start1"] = new C_MenuStateIcon("gold_starSmall",starPos,DISABLED);
	starPos.x = m_screen.x  +  120;
	m_itemsList["gold_start2"] = new C_MenuStateIcon("gold_starSmall",starPos,DISABLED);
	starPos.x = m_screen.x  +  65;
	m_itemsList["gold_start3"] = new C_MenuStateIcon("gold_starBig",starPos,DISABLED);
}


void C_Tab_Status::refresh(){
    std::string replay = "status_Replay";
    C_Settings& settings=C_Locator::getSettings();
    int current = settings.getCurrentLevelNbr();
    if(m_itemsList[replay] != nullptr){
	        m_itemsList[replay]->setText(gettext("Replay level :") + to_string(current));
	        if(m_itemsList[replay]->getCommand() != nullptr){
	        	   m_itemsList[replay]->getCommand()->setNbr(current);
	        }
	}
	std::string next = "status_Next";
	int nextLevel = current + 1;
	if (nextLevel > settings.getNbrOfLevels()){
	    nextLevel = 1;
	}
	if(m_itemsList[next] != nullptr){
	    m_itemsList[next]->setText(gettext("Next level : ") + to_string(nextLevel));
	       if(m_itemsList[next]->getCommand() != nullptr){
	        	  m_itemsList[next]->getCommand()->setNbr(nextLevel);
	        }
    }

	if(m_itemsList["statusResultText"] != nullptr){
        C_Window& win=C_Locator::getWindow();
        C_Level * current = win.getCurrentLevel();
        if(current != nullptr){
	        S_LevelData l = current->getData();
	        std::string text;
	        std::string starType = "gold_star";
	        int starStatus = DISABLED;
	        if(l.status == WIN){
		        text = gettext("Your castle is safe for now");
		        starStatus = ACTIVE;
	        } else if(l.status == LOSE){
	            text = gettext("You lost this castle");
	        } else if(l.status == ONGOING){
		        text = gettext("The battle is not over");
	        }

		    //FIXME Use the stars to differiance clear victories from others
	        if(m_itemsList["gold_start1"] != nullptr) {
                m_itemsList["gold_start1"]->setState(starStatus);
            }
            if(m_itemsList["gold_start2"] != nullptr) {
                m_itemsList["gold_start2"]->setState(starStatus);
            }
            if(m_itemsList["gold_start3"] != nullptr) {
                m_itemsList["gold_start3"]->setState(starStatus);
	        }

	        m_itemsList["statusResultText"]->setText(text);

	    }
	}
}

//-----------------------Bottom Menu-----------------------

C_Menu_Bottom::C_Menu_Bottom(std::string name)
	:C_Page(name)
{
	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight());
	lastHover = "AddTower";
}

void C_Menu_Bottom::refresh(){
    //use the lastHover string keep the focus open on the last button where the mousse pass over"
    std::string last = lastHover;
    for(auto [ k, i] : m_itemsList ){
        if(i !=nullptr){
	        if (i->getState() == HOVER){
	            if(k != lastHover){
	                last = k;
	            }
	        }
	    }
	}
	lastHover = last;

    if(m_itemsList[lastHover]!= nullptr){
        m_itemsList[lastHover]->setState(HOVER);
    }
    for ( auto [ k, i] : m_itemsList ){
        if(i !=nullptr){
	        i->refresh();
	    }
	}
    C_Grid& grid= C_Locator::getGrid();
	C_GameUnits * unit = grid.getSelected();
	if(unit != nullptr){
	    lastHover = "upgradeTower";
    	S_UnitModel data = unit->getInfo();
		if(m_itemsList["upgradeTower"] != nullptr){
			if(grid.isUnitupgradable(unit)){
				m_itemsList["upgradeTower"]->setEnable(true);
			} else {
				m_itemsList["upgradeTower"]->setEnable(false);
			}
		}
    } else {
        lastHover = "AddTower";
    }
}

void C_Menu_Bottom::render(){
		C_TextureList& t= C_Locator::getTextureList();
		//background first line
		S_Coord coord = {m_screen.x, m_screen.y -128};
        renderBackgoundTop(coord);
        coord.y += 128;
        renderBackgoundMiddle(coord);

		C_Page::render();
}

C_Menu_Bottom_Add::C_Menu_Bottom_Add()
	:C_Menu_Bottom("Add")
{
	int size = 64;
	S_Coord coord = {m_screen.x - m_width/4 - 20, m_screen.y - 128};
	//left buttons
	m_itemsList["AddTower"] = new C_GB_AddUnit("AddTower","AddTower",coord.x,coord.y);
	coord.x +=size;
	m_itemsList["AddCatapult"] = new C_GB_AddUnit("AddCatapult","AddCatapult",coord.x,coord.y);
	coord.x +=size;
	m_itemsList["AddBarricade"] = new C_GB_AddUnit("AddBarricade","AddBarricade",coord.x,coord.y);
}

C_Menu_Bottom_Select::C_Menu_Bottom_Select()
	:C_Menu_Bottom("Select")
{
	S_Coord coord = {m_screen.x - m_width/4 - 20 , m_screen.y - 128};
	//left buttons
    if(m_itemsList["upgradeTower"]== nullptr) {
        m_itemsList["upgradeTower"] = new C_GU_Upgrade("upgradeTower",coord);
    }
}

//----------------------- Top-----------------------

C_Menu_Top::C_Menu_Top(std::string name)
	:C_Page(name)
{

	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = -50;
    std::vector <string> names = {"popOutMenu","home","play"};
    S_Coord first = {m_screen.x + m_width/3 - 30,m_screen.y + m_height/4 + 8 };
    flagLine(names,first);

	int x_button = m_screen.x - 90;
	int y_button = m_screen.y + 60;
	if(m_itemsList["gold_pile"]== nullptr) {
        m_itemsList["gold_pile"] = new C_MenuItem("gold_Big_Pile",x_button +40,y_button );
    }
    if(m_itemsList["walletBar"]== nullptr) {
		m_itemsList["walletBar"] = new C_GP_Status("walletBar",x_button + 100 ,y_button + 30, GREEN, BLUE);
		m_itemsList["walletBar"]->setText(gettext("Money: "), 18);
	}
    //progress bar value
    if(m_itemsList["gold_heart"]== nullptr) {
        m_itemsList["gold_heart"] = new C_MenuItem("gold_heart",x_button +40,y_button - 40);
    }
	if(m_itemsList["playerlife"] == nullptr) {
		m_itemsList["playerlife"] = new C_GP_Status("playerlife",x_button + 100,y_button -10, GREEN, RED);
		m_itemsList["playerlife"]->setText(gettext("Life: "), 18);
	}

	if(m_itemsList["levelCount"] == nullptr) {
		m_itemsList["levelCount"] = new C_GP_Status("levelCount",x_button - 140,y_button -10, GREEN,BLUE);
		m_itemsList["levelCount"]->setText(gettext("Level "), 18);
	}
	if(m_itemsList["waveCount"] == nullptr) {
		m_itemsList["waveCount"] = new C_GP_Status("waveCount",x_button - 140,y_button + 30, GREEN,BLUE);
		 m_itemsList["waveCount"]->setText(gettext("Wave "), 18);
	}
}

void C_Menu_Top::refresh(){
        if(m_itemsList["walletBar"] != nullptr) {
		    C_Wallet& wallet= C_Locator::getWallet();
		    m_itemsList["walletBar"]->setPercentage(wallet.getBalance(),wallet.getWalletMax());
		    m_itemsList["walletBar"]->setText2(C_Tools::nbrToString(wallet.getBalance()));
	    }

	    if(m_itemsList["playerlife"] != nullptr) {
	        C_Grid& grid= C_Locator::getGrid();
	        int playerLife = grid.getAllTownsLifeLevel();
		    m_itemsList["playerlife"]->setPercentage(playerLife);
		    m_itemsList["playerlife"]->setText2(C_Tools::nbrToString(playerLife));
	    }

    C_Window& win=C_Locator::getWindow();
    S_LevelData l = win.getCurrentLevel()->getData();

	    if(m_itemsList["waveCount"] != nullptr) {
		    m_itemsList["waveCount"]->setPercentage(l.currentWave,l.totalWaves);
		    m_itemsList["waveCount"]->setText2(std::to_string(l.currentWave) + "/" + to_string(l.totalWaves));
	    }
	    if(m_itemsList["levelCount"] != nullptr) {
	    	C_Settings& settings=C_Locator::getSettings();
	        int nbrOfLevels = settings.getNbrOfLevels();
		    m_itemsList["levelCount"]->setPercentage(l.nbr,nbrOfLevels);
		    m_itemsList["levelCount"]->setText2(std::to_string(l.nbr) + "/" + to_string(nbrOfLevels));
	    }

}

void C_Menu_Top::render(){
		C_TextureList& t= C_Locator::getTextureList();

		//background first line
		int y1 = m_screen.y + 18;
				//background first line
		S_Coord coord = {m_screen.x, m_screen.y +18};
        renderBackgoundMiddle(coord);
		//render second line twice
		coord.y += 128;
        renderBackgoundBottom(coord);


		//background decor
		t.renderTexture("Menu_01_vflag_2", m_screen.x - 60,m_screen.y,CENTER);
		t.renderTexture("Menu_01_vflag_3", m_screen.x - 60,m_screen.y+128,CENTER);
		t.renderTexture("Buttons_Big_icon_boat", m_screen.x - m_width/2 + 80,m_screen.y + 85,CENTER);
		t.renderTexture("Buttons_Big_icon_castle", m_screen.x + m_width/2 - 140,m_screen.y + 90,CENTER);
		C_Page::render();
}



