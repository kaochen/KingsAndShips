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
#include "../texture.h"
#include "../locator.h"

using namespace std;

int C_Tab::m_id = -1;

C_Tab::C_Tab(string title)
{
	m_id++;
	m_name = "tab_" + to_string(m_id);
	m_title = title;
	C_Settings& settings=C_Locator::getSettings();
	m_width = settings.getWindowWidth();
	m_height = 400;
	m_screen.x = 0;
	m_screen.y = (settings.getWindowHeight() - m_height)/2;
	m_tabSize = 120;

	m_itemsList[m_name] = new C_MB_TabSelect(m_name,m_title,18,m_screen.x + m_id*(m_tabSize + 10) + 10,m_screen.y + 20);
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
		int x = m_screen.x + m_width/2;
		int y = m_screen.y + m_height/2;
		t.renderTexture("Menu_01_background", x,y,CENTER);
		int nbr = (m_width/12) - 4;
		for(int i = 0; i < nbr; i++){
			int x2 = m_screen.x+20;
			if(i>0){
				x2 += (i*12);
			}
			int y2 = m_screen.y+50;
			t.renderTexture("Menu_details_bottom_line_active",x2 ,y2,CENTER);
		}
	}
}



std::vector<string> C_Tab::getListOfVisibleItems()
{
	vector <string> list;
	for(auto const& x : m_itemsList) {
		list.push_back(x.first);
	}
	return list;
}

C_Tab_Settings::C_Tab_Settings()
	:C_Tab("Settings")
{
	string name = "Resolution";
	C_Settings& settings=C_Locator::getSettings();
	string width = to_string(settings.getWindowWidth());
	string height = to_string(settings.getWindowHeight());
	string text = width + "x" + height;
	m_itemsList[name] = new C_MB_1Line(name,text,m_screen.x + 10,m_screen.y +  60);

	name = "Grid Size";
	text =  to_string(settings.getGridWidth())+ "x" +to_string(settings.getGridHeight());
	m_itemsList[name] = new C_MB_1Line(name,text,m_screen.x + 10,m_screen.y +  90);

}

C_Tab_Levels::C_Tab_Levels()
	:C_Tab("Levels")
{
	C_Settings& settings=C_Locator::getSettings();
	string name = "Number of Levels";
	string text = to_string(settings.getNbrOfLevels());
	m_itemsList[name] = new C_MB_1Line(name,text,m_screen.x + 10,m_screen.y +  60);

	int j = 0;
	for(int i = 1; i <= settings.getNbrOfLevels(); i++) {
		name = "Card_" + to_string(i);
		m_itemsList[name] = new C_MB_LevelCard(i,name,m_screen.x + 20 + j*(160),m_screen.y +  90);
		C_LoadALevel *command = new C_LoadALevel();
		m_itemsList[name]->setCommand(command);
		m_itemsList[name]->getCommand()->setNbr(i);
		j++;
	}
}
