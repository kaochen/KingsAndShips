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


C_Menu::C_Menu()
{
	C_Message::printM("Constructor C_Menu() : start\n");

    C_Frame *top = new C_Frame("topMenu");
    top->addPage(new C_Menu_Top("topMenu"));
    m_frames.push_back(top);

    C_Frame *bottom = new C_Frame("bottomMenu");
    bottom->addPage(new C_Menu_Bottom("bottomMenu"));
    m_frames.push_back(bottom);

    C_Frame *select = new C_Frame("unitSelected");
    select->addPage(new C_Unit_Selected());
    select->setOpen(false);
    m_frames.push_back(select);

	C_Message::printM("Constructor C_Menu() : done\n");
}

C_Menu::~C_Menu()
{
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

void C_Menu::openTab(std::string name){
    for(auto const& f : m_frames) {
		if(f != nullptr){
		    f->openTabIfExist(name);
		}
	}
}

void C_Menu::refresh()
{
	C_Window& win=C_Locator::getWindow();
	S_LevelData data = win.getCurrentLevel()->getData();
	if(data.status == WIN){
	    C_Frame* main = getFrame("mainMenu");
	    if(main != nullptr){
	        main->setOpen(false);
	    }
		if(getFrame("endGame") !=  nullptr){
	    	getFrame("endGame")->setOpen(true);
    		C_Settings& settings=C_Locator::getSettings();
		    settings.setPlaying(PAUSE);
	    }
	}
	//When a unit is selected :
	if(getFrame("unitSelected") !=  nullptr){
	    C_Grid& grid= C_Locator::getGrid();
	    C_GameUnits * unit = grid.getSelected();
	    if(unit != nullptr){
		            getFrame("unitSelected")->setOpen(true);
        } else {
		            getFrame("unitSelected")->setOpen(false);
        }
    }

}

std::vector <C_MenuItem *> C_Menu::getMenuItems(){
    std::vector <C_MenuItem *> ret;
        for(auto const& f : m_frames) {
		        if(f != nullptr){
			        std::map<std::string, C_MenuItem*> items = f->getItemList();
			        for(auto i : items){
			            if (i.second !=  nullptr){
			                ret.push_back(i.second);
			            }
			        }
		        }
	        }
    return ret;
}

void C_Menu::render()
{
	for(auto const& f : m_frames) {
		if(f != nullptr){
			f->refresh();
			f->render();
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
		    if(getFrame("unitSelected") !=  nullptr)
		        getFrame("unitSelected")->setOpen(false);
		    settings.setPlaying(PAUSE);
	    }
	}
}

void C_Menu::menuBanner()
{
	C_Frame *main = new C_Frame("mainMenu");
	if(main != nullptr){
	    main->setOpen(false);
        main->addPage( new C_Tab_Levels());
        main->addPage( new C_Tab_Settings());
        main->addPage( new C_Tab("About"));
        main->addPage( new C_Tab_endGame());
        m_frames.push_back(main);
    }
}


void C_Menu::go(int direction){
    if(getFrame("mainMenu") != nullptr){
        if(getFrame("mainMenu")->getCurrent() != nullptr){
            getFrame("mainMenu")->getCurrent()->go(direction);
         }
    }
}




