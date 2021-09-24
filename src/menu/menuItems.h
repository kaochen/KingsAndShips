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
#include "../level/shooter.h"
#include "command.h"

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
	virtual int getHeight() const{return m_height;};
	virtual	std::string getName(){return m_name;};
	virtual int getLayer(){return m_layer;};
	virtual void setCommand(C_Command *c){m_command = c;};
	virtual C_Command * getCommand(){return m_command;};

	virtual void setPercentage(int percentage){std::cout << percentage;};
	virtual void setPercentage(int a, int b){std::cout << a << b;};
	virtual	void render();
	virtual void action();
	virtual void actionHover(bool state);
	virtual int getEnable(){return m_enable;};
	virtual void setEnable(bool enable){m_enable = enable;};
	virtual void setState(int state){m_state = state;};
	virtual int getState(){	return m_state;};
	virtual void drag(S_Coord screen){std::cout << "GCC calm: " << screen.x;};

	//set Text
	virtual void renderText();
	virtual void setText(std::string text, int fontSize);
	virtual void setText(std::string text);
	virtual void setTextPosition(int x_text, int y_text);
	virtual void setTextColor(SDL_Color color){m_color = color;};

	virtual void setImage(std::string image){m_image = image;};
protected:
    virtual SDL_Color getTextColor();
	std::string getStateAsStr();

	int m_type;
	std::string m_name;
	std::string m_image;
	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
	int m_layer;
	bool m_enable;
	//text
	std::string m_textName;
	std::string m_text;
	int  m_fontSize = 18;
	SDL_Color m_color;
	const SDL_Color m_colorText = {79,54,51,255};
	const SDL_Color m_colorTextHover = {147,106,84,255};
	int m_x_text;
	int m_y_text;
	int m_state;

	C_Command * m_command;
};

class C_MenuText : public C_MenuItem {
public :
    C_MenuText(std::string name,std::string text,int x_screen, int y_screen);
   	virtual void render();
};


class C_Button: public C_MenuItem {
public:
	C_Button(std::string name,std::string image,int x_screen, int y_screen);

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

class C_GB_AddUnit: public C_Button { /*!Game Button add a new unit on the ground*/
public:
	C_GB_AddUnit(std::string name,std::string image,int x_screen, int y_screen);
	virtual ~C_GB_AddUnit();
	virtual void drag(S_Coord screen);
	virtual void render();

protected:
	C_GameUnits * m_unit;
};

enum colorList {GREEN,RED,BLUE};

class C_GP_Status: public C_MB_CardButton { /*!In Game Progress to show the status (Life, wallet)*/
public:
	C_GP_Status(std::string name,int x_screen, int y_screen, int colorIn, int colorOut);
  	virtual	~C_GP_Status() {};
	virtual void render();
	virtual void setPercentage(int percentage)
	{
		m_percentage = percentage;
	};
	virtual void setPercentage(int a, int b);
protected:
	std::string colorToStr(int color);
	int m_percentage;
	int m_oldPercentage;
	int m_xOffset;
	int m_colorIn;
	int m_colorOut;
};

class C_GU_Upgrade: public C_Button{
public:
  	C_GU_Upgrade(std::string name,S_Coord screen);
    	virtual ~C_GU_Upgrade(){};
  	virtual void render();
};

#endif
