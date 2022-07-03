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

//Menu elements like progressbar and buttons

#ifndef MENUITEMS_H
#define MENUITEMS_H

#include <iostream>
#include <string>
#include "../level/gameUnits.h"
#include "../level/shooter.h"
#include "command.h"
#include "../message.h"

enum menuLayer {BACK,FRONT};
enum buttonState {ACTIVE,HOVER,DISABLED};

struct S_Line{
    std::string name;
    std::string text;
};

class C_MenuItem {
public:
	C_MenuItem(std::string name, int x_screen, int y_screen);
	virtual ~C_MenuItem();
	virtual int getType(){return m_type;};
	virtual int getXScreen() const{return m_x_screen;};
	virtual int getYScreen() const{return m_y_screen;};
	virtual void setScreen(S_Coord const screen) {m_x_screen = screen.x; m_y_screen = screen.y;};

	virtual int getWidth() const{return m_width;};
	virtual void setWidth(int width) {m_width = width;};
	virtual int getHeight() const{return m_height;};
	virtual	std::string getName(){return m_name;};
	virtual void setCommand(C_Command *c){m_command = c;};
	virtual C_Command * getCommand(){return m_command;};

	virtual void setPercentage(int percentage){std::cout << percentage;};
	virtual void setPercentage(int a, int b){std::cout << a << b;};
	virtual void setPercentage2(int a, int b){std::cout << a << b;};
	virtual	void render();
    virtual void refresh(){};
	virtual void action();
	virtual void actionHover(bool state);
	virtual int getEnable(){return m_enable;};
	virtual void setEnable(bool enable){m_enable = enable;};
	virtual void setState(int state){m_state = state;};
	virtual int getState(){	return m_state;};
	virtual void drag(S_Coord screen){std::cout << "GCC calm: " << screen.x;};

	//set Text
	virtual void setText(std::string text, int fontSize);
	virtual void setText(std::string text){m_text = text;};
    virtual void setText2(std::string text2){m_text2 = text2;};
	virtual void setTextColor(SDL_Color color){m_color = color;};

	virtual void setImage(std::string image){m_image = image;};
protected:
    static unsigned m_count;
    unsigned m_id;

    virtual SDL_Color getTextColor();
	std::string getStateAsStr();

	int m_type;
	std::string m_name;
	std::string m_image;
	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
	bool m_enable;
	//text
	std::string m_textName;
	std::string m_text;
    std::string m_textName2;
    std::string m_text2;
	int  m_fontSize = 18;
	SDL_Color m_color;
	const SDL_Color m_colorText = {79,54,51,255};
	const SDL_Color m_colorTextHover = {147,106,84,255};
    const SDL_Color m_colorTextDisabled = {81,75,75,255};
	int m_state; //buttonState ACTIVE,HOVER,DISABLED

	C_Command * m_command;
};

class C_MenuText : public C_MenuItem {
public :
    C_MenuText(std::string name,std::string text,int x_screen, int y_screen);
   	virtual void render();
};

class C_MB_TabSelect: public C_MenuItem { /*!Main Menu button to select a specific tab*/
public:
	C_MB_TabSelect(std::string name,std::string text, int fontSize, int x_screen, int y_screen);
	virtual void render();
};
enum arrows {GO_LEFT,GO_RIGHT,Go_UP,GO_DOWN};

class C_MB_Arrows: public C_MenuItem { /*!Main menu button in a shape of an arrow*/
public:
	C_MB_Arrows(std::string name,int direction , int x_screen, int y_screen);
	virtual void render();
protected:
    int m_direction;
};

class C_MB_1Line: public C_MenuItem { /*!Button Menu one line two texts*/
public:
	C_MB_1Line(std::string name,std::string text, int x_screen, int y_screen);
  	virtual	~C_MB_1Line() {};
	virtual void render();
protected:
	std::string m_title;
	std::string m_titleName;
	std::string m_oldTitle;
};

class C_MB_LevelCard: public C_MenuItem { /*!Level Card to select a level*/
public:
	C_MB_LevelCard(int nbr,std::string name, int x_screen, int y_screen);
  	virtual	~C_MB_LevelCard() {};
	virtual void render();
protected:
    std::vector <S_Line> m_list;
};

class C_MB_CardButton: public C_MenuItem{
public:
    C_MB_CardButton(std::string name, int x_screen, int y_screen);
	virtual void render();
};

class C_Button: public C_MenuItem { /*!Game Button*/
public:
	C_Button(std::string name,std::string image,int x_screen, int y_screen);
	virtual void render();
};

class C_GB_Button: public C_Button { /*!Game Button use in the bottom menu*/
public:
	C_GB_Button(std::string name,std::string image,int x_screen, int y_screen);
	virtual ~C_GB_Button();
	virtual void render();
    virtual void refresh();
protected:
    virtual double convertFireRate(double value);
    std::vector<C_MenuItem*> m_itemsList;;
	C_GameUnits * m_unit;
};

class C_GB_AddUnit: public C_GB_Button { /*!Game Button add a new unit on the ground*/
public:
	C_GB_AddUnit(std::string name,std::string image,int x_screen, int y_screen);
	virtual void drag(S_Coord screen);
	virtual void render();
};

class C_GU_Upgrade: public C_GB_Button{ /*!Game Button to upgrade a unit already on the ground*/
public:
  	C_GU_Upgrade(std::string name,S_Coord screen);
  	virtual void render();
    virtual void refresh();
};

enum colorList {GREEN,RED,BLUE};

class C_GP_Status: public C_MB_CardButton { /*!In Game Progress to show the status (Life, wallet)*/
public:
	C_GP_Status(std::string name,int x_screen, int y_screen, int color1, int color2);
	virtual void render();
	virtual void setPercentage(int percentage)
	{
		m_percentage1 = percentage;
	};
	virtual void setPercentage(int a, int b);
	virtual void setPercentage2(int a, int b);
protected:
    virtual int  getPercentage(int a, int b);
	std::string colorToStr(int color);
	int m_percentage1;
    int m_percentage2;
    int m_max;
	int m_xOffset;
	int m_color1;
	int m_color2;
};

class C_MenuStateIcon: public C_MenuItem{ /*Items with an image to indicate a state, like a star yellow or gray, no action*/
public:
    C_MenuStateIcon(std::string name, S_Coord screen, int state);
    virtual void render();
};

#endif
