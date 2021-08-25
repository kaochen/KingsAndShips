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
#include "../coord.h"

class C_Page {
public :
    C_Page(std::string name):m_name(name){};
	virtual ~C_Page();
	virtual std::string getName(){return m_name;};
	virtual std::map<std::string, C_MenuItem*> getItemList(){return m_itemsList;};
    virtual void render() = 0;
    virtual std::vector<std::string> getListOfVisibleItems();
    virtual void setWin(int win){ std::cout << win;};
    virtual void refresh(){};
    virtual bool getOpen(){return m_open;};
    virtual void setOpen(bool open){m_open =  open;};
protected:
    virtual void flagLine(std::vector <std::string> names, S_Coord first);
  	std::string m_name;
	std::map<std::string, C_MenuItem*> m_itemsList;
	S_Coord m_screen /*!< top left corner of the tab*/;
    bool m_open = true;
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
    virtual void render();
    virtual void setWin(int win){ m_levelStatus = win;};
    virtual void refresh();
protected:
    int m_levelStatus;
};

class C_Menu_Bottom : public C_Page {
    public:
    	C_Menu_Bottom(std::string name);
        virtual void render();
};

class C_Menu_Top : public C_Page {
    public:
    	C_Menu_Top(std::string name);
        virtual void render();
};

#endif
