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

using namespace std;

int C_Tab::m_id = -1;

C_Tab::C_Tab(std::string title)
{
	m_id++;
	m_name = "tab_" + to_string(m_id);
	m_title = title;
	C_Settings& settings=C_Locator::getSettings();
	m_width = 900;
	m_height = 400;
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_flagScreen.x = m_screen.x - 295;
	m_flagScreen.y = m_screen.y - 145;
	m_flagOffset = 35;


	m_tabSize = 120;
	int x = m_screen.x - ((m_tabSize *3) / 2);

	m_itemsList[m_name] = new C_MB_TabSelect(m_name,m_title,18, x + (m_tabSize*m_id) ,m_screen.y - 220);
	if(m_itemsList[m_name] != nullptr) {
		m_itemsList[m_name]->setCommand(new C_ChangeTab);
		if( m_itemsList[m_name]->getCommand() != nullptr)
			m_itemsList[m_name]->getCommand()->setNbr(m_id);
	}
}

C_Tab::~C_Tab()
{
    m_itemsList.clear(); //items are allready delete
}

void C_Tab::displayTab(bool open)
{
	if(open) {
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Menu_01_background", m_screen.x,m_screen.y,CENTER);
	}
}

void C_Tab::fillWithClosedFlags(){
    size_t start = m_itemsList.size();
    if(start > 0){
        start--;
    }
    for(size_t i = start; i < 10 ; i++){
        std::string name = "closedFlag" + std::to_string(i);
     	m_itemsList[name] = new C_MB_1Line("","",m_flagScreen.x,m_flagScreen.y + m_flagOffset*i);
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
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x,m_flagScreen.y);

	name = "Grid Size";
	text =  std::to_string(settings.getGridWidth())+ "x" +std::to_string(settings.getGridHeight());
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x,m_flagScreen.y + m_flagOffset);
    fillWithClosedFlags();
}

C_Tab_Levels::C_Tab_Levels()
	:C_Tab("Levels")
{
	C_Settings& settings=C_Locator::getSettings();
	std::string name = "Number of Levels";
	std::string text = std::to_string(settings.getNbrOfLevels());
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x,m_flagScreen.y);
    fillWithClosedFlags();

    int currentNbr = settings.getCurrentLevelNbr();
	name = "Card_" + to_string(currentNbr);
	m_itemsList[name] = new C_MB_LevelCard(currentNbr,name,m_screen.x + 50,m_screen.y);
	C_LoadALevel *command = new C_LoadALevel();
	m_itemsList[name]->setCommand(command);
	m_itemsList[name]->getCommand()->setNbr(currentNbr);
}
