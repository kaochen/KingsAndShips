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


#ifndef TAB_H
#define TAB_H
#include "../settings.h"
#include "menuItems.h"
#include <string>
#include <map>

class C_Page {
public :
    C_Page(std::string name):m_name(name){};
	virtual ~C_Page();
	virtual std::string getName(){return m_name;};
	virtual std::map<std::string, C_MenuItem*> getItemList(){return m_itemsList;};
    virtual void render() = 0;
    virtual std::vector<std::string> getListOfVisibleItems();
protected:
  	std::string m_name;
	std::map<std::string, C_MenuItem*> m_itemsList;
	S_Coord m_screen /*!< top left corner of the tab*/;
};

class C_Tab : public C_Page {
public:
	C_Tab(std::string title);
	virtual void displayTab(bool open);
    virtual void render(){displayTab(true);};
	virtual std::string getTitle(){return m_title;};
    virtual std::vector<std::string> getListOfVisibleItems();
    virtual void go(int direction){std::cout << direction << std::endl;};

protected:
    void fillWithClosedFlags();
	static int m_id;
	std::string m_title;
	S_Coord m_flagScreen /*!< first flag position in menu*/;
	int m_height;
	int m_width;
    int m_flagOffset;
	Sint16 m_tabSize;
};

class C_Tab_Settings : public C_Tab {
public:
	C_Tab_Settings();
};

class C_Tab_Levels : public C_Tab {
public:
	C_Tab_Levels();
    virtual void go(int direction);
protected:
    int m_currentCardLevelNbr;
};

class C_Tab_endGame : public C_Page {
public:
	C_Tab_endGame(std::string name);
    void render();
};


#endif
