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
    C_Page(std::string name);
	virtual ~C_Page();
	virtual std::string getName(){return m_name;};
	virtual std::map<std::string, C_MenuItem*> getItemList(){return m_itemsList;};
    virtual void render();
    virtual void refresh(){};
    virtual void go(int direction){std::cout << direction << std::endl;};
    virtual S_Coord getCoord(){return m_screen;};
protected:
    virtual void column(std::vector <std::string> names, S_Coord first);
    virtual void flagLine(std::vector <std::string> names, S_Coord last);
    virtual void renderBackgoundTop(S_Coord coord);
    virtual void renderBackgoundMiddle(S_Coord coord);
    virtual void renderBackgoundBottom(S_Coord coord);
  	std::string m_name;
	std::map<std::string, C_MenuItem*> m_itemsList;
	S_Coord m_screen /*!< top left corner of the tab*/;
    int m_height;
	int m_width;
};

class C_Tab : public C_Page {
public:
	C_Tab(std::string title);
    virtual void render();

protected:
	S_Coord m_flagScreen /*!< first flag position in menu*/;
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

class C_Tab_Status : public C_Tab {
public:
	C_Tab_Status();
    virtual void refresh();
protected:
};

//-----------------------Bottom Menu-----------------------
class C_Menu_Bottom : public C_Page {
    public:
    	C_Menu_Bottom(std::string name);
        virtual void render();
        void refresh();
        std::string lastHover;
};
class C_Menu_Bottom_Add : public C_Menu_Bottom {
    public:
    	C_Menu_Bottom_Add();
};

class C_Menu_Bottom_Select : public C_Menu_Bottom {
    public:
    	C_Menu_Bottom_Select();
};

//-----------------------Top Menu-----------------------

class C_Menu_Top : public C_Page {
    public:
    	C_Menu_Top(std::string name);
        virtual void refresh();
        virtual void render();
};

class C_Frame {
public :
    C_Frame(std::string name);
	virtual ~C_Frame();
    virtual std::string getName(){return m_name;};
    virtual void addPage(C_Page *page);
    virtual C_Page* getCurrent();
    virtual void refresh();
    virtual void render();
    virtual std::map<std::string, C_MenuItem*> getItemList();
    virtual bool getOpen(){return m_open;};
    virtual void setOpen(bool open){m_open =  open;};
    virtual void openTabIfExist(std::string tabname);
protected:
    std::string m_name;
    int m_currentPage = 0;
    std::string m_current;
    std::vector <C_Page*> m_list;
    bool m_open = true;
};


#endif
