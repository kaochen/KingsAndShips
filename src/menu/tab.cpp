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
	m_height = (settings.getWindowHeight()*2)/3;
	m_screen.x = 0;
	m_screen.y = (settings.getWindowHeight() - m_height)/2;
	m_tabSize = 120;

	m_itemsList[m_name] = new C_MB_TabSelect(m_name,m_title,18,m_screen.x + m_id*(m_tabSize + 10) + 10,m_screen.y + 5);
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

void C_Tab::displayTab(bool open, size_t nbr)
{
	if(open) {
		focusTab(nbr);
	}
}

void C_Tab::focusTab(size_t nbr)
{

	Uint8 R = 0, G = 0, B = 0, A = 150;
	Sint16 x1 = m_screen.x - 5; //top left
	Sint16 y1 = m_screen.y; //top left
	Sint16 x2 = m_screen.x + m_width; //top right
	Sint16 y2 = y1;
	Sint16 x3 = x2; // bottom right
	Sint16 y3 = y1 + m_height;
	Sint16 x4 = x1; //bottom left;
	Sint16 y4 = y3;

	Sint16 vx[] = {x1,x2,x3,x4};
	Sint16 vy[] = {y1,y2,y3,y4};
	//draw
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer * renderer = win.getRenderer();
	filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);

	Sint16 xSep = m_screen.x + 20;
	drawAseparator(xSep,m_screen.y + 40,m_width - 40);

	drawBackgroundGrid(x1, y1, m_width, m_height);

}

void C_Tab::drawAseparator(Sint16 x, Sint16 y, Sint16 width)
{
	Uint8 R = 200, G = 200, B = 200, A = 50;
	Sint16 x1 = x; //top left
	Sint16 y1 = y;
	Sint16 x2 = x + width; //top right
	Sint16 y2 = y;
	Sint16 x3 = x2; // bottom right
	Sint16 y3 = y + 2;
	Sint16 x4 = x; //bottom left;
	Sint16 y4 = y3;

	Sint16 vx[] = {x1,x2,x3,x4};
	Sint16 vy[] = {y1,y2,y3,y4};
	//draw
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer * renderer = win.getRenderer();
	filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);
}


void C_Tab::drawBackgroundGrid(Sint16 x, Sint16 y, Sint16 width, Sint16 height)
{
	Sint16 x1 = x;
	Sint16 y1 = y;
	Sint16 y2 = y1 + height;
	C_Window& win=C_Locator::getWindow();
	for(int i = 0; i < width/20; i++) {
		x1 +=20;
		Sint16 x2 = x1 + 1;
		boxRGBA(win.getRenderer(),x1,y1,x2,y2,100,100,100,20);
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
	m_itemsList[name] = new C_MB_1Line(name,text,m_screen.x + 10,m_screen.y +  50);

	name = "Theme";
	text =  settings.getThemePath();
	m_itemsList[name] = new C_MB_1Line(name,text,m_screen.x + 10,m_screen.y +  80);

}

C_Tab_Levels::C_Tab_Levels()
	:C_Tab("Levels")
{
	C_Settings& settings=C_Locator::getSettings();
	string name = "Number of Levels";
	string text = to_string(settings.getNbrOfLevels());
	m_itemsList[name] = new C_MB_1Line(name,text,m_screen.x + 10,m_screen.y +  50);

	int j = 0;
	for(int i = 1; i <= settings.getNbrOfLevels(); i++) {
		name = "Card_" + to_string(i);
		m_itemsList[name] = new C_MB_LevelCard(i,name,m_screen.x + 10 + j*(180),m_screen.y +  100);
		C_LoadALevel *command = new C_LoadALevel();
		m_itemsList[name]->setCommand(command);
		m_itemsList[name]->getCommand()->setNbr(i);
		j++;
	}
}
