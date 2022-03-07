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


#include "menuItems.h"
#include "../window.h"
#include "../locator.h"
#include "../textureList.h"
#include "../message.h"
#include "../level/factory.h"
#include "../level/grid.h"
#include <string>
#include <libintl.h>
#include <locale.h>


using namespace std;
C_MenuItem::C_MenuItem(string name, int x_screen, int y_screen):
	m_type(ACTION),
	m_name(name),
	m_image(name),
	m_x_screen(x_screen),
	m_y_screen(y_screen),
	m_width(64),
	m_height(64),
	m_enable(true),
	m_state(ACTIVE)
{
	m_textName = name + "Text";
	m_text = "";
	m_textName2 = name + "Text2";
	m_text2 = "";
	m_color = {200,200,200,255};
	m_command = nullptr;
	m_fontSize = FONT_BIG;
}

C_MenuItem::~C_MenuItem(){
	if(m_command != nullptr)
	    delete m_command;
}

void C_MenuItem::setText(string text, int fontSize)
{
	m_text = text;
	m_fontSize = fontSize;
}

SDL_Color C_MenuItem::getTextColor(){
    SDL_Color ret = {200,200,200,255};
	if(m_state == HOVER){
		ret = m_colorTextHover;
	} else {
		ret = m_colorText;
	}
	return ret;
}

void C_MenuItem::render()
{
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(m_image, m_x_screen + m_width/2,m_y_screen+m_height/2,CENTER);
	if(m_text !="") {
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
		t.renderText(m_textName, m_x_screen + m_width/2, m_y_screen +  m_height/2,CENTER);
	}
}

void C_MenuItem::action()
{
	C_Message::printM("push button " + m_name + "\n");
	if(m_command != nullptr) {
		m_command->action();
	}
}

void C_MenuItem::actionHover(bool state)
{
	if(m_command != nullptr) {
		m_command->actionHover(state);
		if(state){
			m_state = HOVER;
		}
		else {
			m_state = ACTIVE;
		}
	}
}


std::string C_MenuItem::getStateAsStr(){
		string ret = "_Disabled";
		if(m_state == ACTIVE) {
			ret ="_Active";
		} else if(m_state == HOVER) {
			ret ="_Hover";
		} else {
			ret ="_Disabled";
		}

		if(m_enable == false) {
			ret ="_Disabled";
		}
		return ret;
}

//-------------------------------------------------------------


C_MenuText::C_MenuText(std::string name,std::string text,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
    m_text = text;
    m_type = STATUS;
}

void C_MenuText::render()
{
	if(m_text !="") {
    	C_TextureList& t= C_Locator::getTextureList();
    	t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, getTextColor());
		t.renderText(m_textName, m_x_screen , m_y_screen ,CENTER);
	}
}

//-------------------------------------------------------------

C_Button::C_Button(string name,string image,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	m_image = image;
}


void C_Button::render()
{
	string name = "Buttons_"+m_image;

	C_Settings& settings= C_Locator::getSettings();
	if(m_name == "play") {
		if(settings.getPlaying() == PLAYING) {
			name = "Buttons_pause";
		}
	}

	name += getStateAsStr();
	C_TextureList& t= C_Locator::getTextureList();
	int up = 0;
	if(m_state == HOVER){
	    up = 1;
	}
	t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height/2 - up,CENTER);
}

//-------------------------------------------------------------

C_MB_TabSelect::C_MB_TabSelect(string name,string text, int fontSize,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	setText(text, fontSize);
	m_width = 80;
	m_height = 60;
	m_color = m_colorText;
}


void C_MB_TabSelect::render()
{
	C_TextureList& t= C_Locator::getTextureList();
    int up = 0;
	if(m_state == HOVER){
		up = +2;
    }

	Sint16 x1 = m_x_screen + m_width/2;
	Sint16 y1 = m_y_screen + m_height/2 + up;

	if(m_text !="") {
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, getTextColor());
		t.renderText(m_textName, x1, y1 ,CENTER);
	}
    t.renderTexture("Menu_01_lineShort", x1 ,y1 + 10 + up,CENTER);
}

C_MB_Arrows::C_MB_Arrows(std::string name,int direction , int x_screen, int y_screen)
    :C_MenuItem(name,x_screen,y_screen)
{
    m_direction = direction;
    m_width = 32;
	m_height = 32;
}

void C_MB_Arrows::render(){
    std::string name = "Menu_01_Go";
    if(m_direction == GO_LEFT){
        name += "Left";
    } else {
        name += "Right";
    }

    if(m_state == HOVER){
        name += "_Hover";
    } else {
        name += "_Active";
    }

    C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height/2,CENTER);
}

//-------------------------------------------------------------

C_MB_1Line::C_MB_1Line(string name,string text,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	m_fontSize = FONT_BIG;
	m_title = name;
	m_titleName = "Settings_Title_" + name;
	m_text = text;
	m_textName = "Settings_Text_" + name;
	m_width = 400;
	m_height = 24;
	m_color = m_colorText;
}


void C_MB_1Line::render()
{
	if(m_state == HOVER){
		m_color = m_colorTextHover;
	} else {
		m_color = m_colorText;
	}
	C_TextureList& t= C_Locator::getTextureList();
	Sint16 x1 = m_x_screen + m_width/2;
	Sint16 y1 = m_y_screen + m_height/2;
	if(m_title !="") {
		t.loadTextAsTexturesIntoMap(m_titleName, m_title, m_fontSize, m_color);
		t.renderText(m_titleName, x1 - m_width/2 + 30, y1, LEFT);
		if(m_text !="") {
	        t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
		    t.renderText(m_textName, x1 + m_width/4 + 30 , y1, RIGHT);
	    }
	    t.renderTexture("Menu_01_line1", x1 - m_width/4 + 8,m_y_screen + 28,CENTER);
		t.renderTexture("Menu_01_line2", x1 + m_width/4 ,m_y_screen + 28,CENTER);
	}

}

//-------------------------------------------------------------

C_MB_LevelCard::C_MB_LevelCard(int nbr, string name,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	m_type = STATUS;
	m_fontSize = FONT_BIG;
	C_Window& win= C_Locator::getWindow();
    S_LevelData model = win.getLevelModel(nbr);
    S_Line t0 = {"Card_name",model.name};
    m_list.push_back(t0);
    S_Line t1 = {"Card_levelNbr", "Level: "+to_string(model.nbr)};
    m_list.push_back(t1);
    S_Line t2 = {"Card_size","Grid Size: " + to_string(model.gridSize)};
    m_list.push_back(t2);

	C_TextureList& t= C_Locator::getTextureList();
    for(auto i :m_list){
		t.loadTextAsTexturesIntoMap(i.name, i.text, m_fontSize,  m_colorText);
    }

	m_width = 280;
	m_height = 330;
}

void C_MB_LevelCard::render()
{
	C_TextureList& t= C_Locator::getTextureList();

    //Text
    int y = m_y_screen;
    for(auto i :m_list){
	    t.renderText(i.name, m_x_screen , y,CENTER);
			y += 20;
    }
}

C_MB_CardButton::C_MB_CardButton(std::string name, int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen){
	m_width = 200;
	m_height = 32;
	m_text = "Waiting...";
	m_fontSize = FONT_BIG;

}

void C_MB_CardButton::render(){
	int up = 0;
	m_color = m_colorText;
	if(m_state == HOVER){
		m_color = m_colorTextHover;
		up = +1;
    }

    Sint16 x1 = m_x_screen + m_width/2;
	Sint16 y1 = m_y_screen + m_height/2 + up;

	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("Menu_01_lineShort", x1 , y1 + 10,CENTER);
	t.loadTextAsTexturesIntoMap(m_name, m_text, m_fontSize,  m_color);
	t.renderText(m_name, x1, y1 ,CENTER);
}
//-------------------------------------------------------------

C_GB_AddUnit::C_GB_AddUnit(string name,string image,int x_screen, int y_screen)
	:C_Button(name,image,x_screen,y_screen)
{
	C_Grid& grid= C_Locator::getGrid();
	C_UnitFactory factory = grid.getFactory();

	m_type = DRAGUNIT;
	S_Unit unit;
	m_unit = nullptr;
	if(name == "AddTower") {
		unit.name = "ArcherTower_0";
		m_unit = factory.create(unit);
	}  else if(name == "AddCatapult") {
		unit.name = "Catapult_0";
		m_unit = factory.create(unit);
	} else if(name == "AddBarricade") {
		unit.name = "barricade_1";
		m_unit = factory.create(unit);
	}
	if(m_unit != nullptr)
		m_text = to_string(m_unit->getCost());
	else
		m_text = "empty";
	m_fontSize = FONT_BIG;
}

C_GB_AddUnit::~C_GB_AddUnit()
{
	if(m_unit !=nullptr)
		delete m_unit;
}

void C_GB_AddUnit::drag(S_Coord screen)
{
	if (m_unit != nullptr) {
		m_unit->drag(screen);
	}
}

void C_GB_AddUnit::render()
{
	C_Wallet& wallet= C_Locator::getWallet();
	if(m_unit != nullptr) {
		if(wallet.getBalance() < m_unit->getCost()) {
			m_enable = false;
		} else {
			m_enable = true;
		}
	    C_Button::render();
	    //render price:
        if(!m_enable){
	        m_color = {0,0,0,255};
	    } else {
	        m_color = getTextColor();
	    }
	    C_TextureList& t= C_Locator::getTextureList();
	    t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
	    t.renderText(m_textName, m_x_screen + m_width + 10, m_y_screen + m_height/2,CENTER);
	}
}
//-------------------------------------------------------------

C_GP_Status::C_GP_Status(string name,int x_screen, int y_screen, int colorIn, int colorOut)
	:C_MB_CardButton(name,x_screen,y_screen)
{
	m_type = STATUS;
	m_width = 152;
	m_height = 42;
	m_percentage = 100;
	m_oldPercentage = m_percentage;
	m_xOffset = 0;
	m_colorIn = colorIn;
	m_colorOut = colorOut;
}


void C_GP_Status::setPercentage(int a, int b)
{
	if(a != 0 && b !=0)
		m_percentage = ((100*a)/b);
	else
		m_percentage = 0;

	if (m_percentage > 100)
		m_percentage = 100;
	if (m_percentage < 0)
		m_percentage = 0;
}


void C_GP_Status::render()
{
	C_TextureList& t= C_Locator::getTextureList();
    std::string line;
    size_t length = m_width/10;
    int x1 = m_x_screen;
	int y1 = m_y_screen + m_height/2 + 10 ;
	int offset = 0;
	for (int i = 0; i <= length; i++){
    	t.renderTexture("Buttons_Progress_Bright", x1 + offset ,y1,CENTER);
    	offset += 9;
	}

	int length2 = (m_percentage * length)/(100);
	if(m_percentage>0){
	    offset = 0;
	    std::string color = colorToStr(m_colorIn);
	    for (int i = 0; i <= length2; i++){
        	t.renderTexture("Buttons_Progress_"+color, x1 + offset ,y1,CENTER);
        	offset += 9;
	    }
    }
	m_color = m_colorText;
    Sint16 x2 = m_x_screen + m_width/2;
	Sint16 y2 = m_y_screen + m_height/2;
	if(m_text !="") {
    	t.loadTextAsTexturesIntoMap(m_textName, m_text, FONT_SMALL,  m_color);
    	t.renderText(m_textName, m_x_screen, y2 ,LEFT);
	    if(m_text2 !="") {
        	t.loadTextAsTexturesIntoMap(m_textName2, m_text2, FONT_SMALL,  m_color);
        	t.renderText(m_textName2, m_x_screen + m_width - 10, y2 ,RIGHT);
	    }
	}
}


string C_GP_Status::colorToStr(int color)
{
	if (color == GREEN) {
		return "Green";
	} else if(color == RED) {
		return "Red";
	} else if(color == BLUE) {
		return "Blue";
	} else {
		return "Dark";
	}

}


C_GU_Upgrade::C_GU_Upgrade(string name,S_Coord screen)
	:C_Button(name,"upgrade",screen.x,screen.y)
{
    m_type = ACTION;
    m_command = new C_UpgradeUnit();
    m_text = "0";
    m_textName = "upgrade_Text_" + name;
	m_fontSize = FONT_SMALL;
    m_color = {255,255,255,255};
}


void C_GU_Upgrade::render()
{
	C_Grid& grid= C_Locator::getGrid();
	C_UnitFactory factory = grid.getFactory();
	S_UnitModel model;
	bool check = factory.getSelectedModel(1,model);
	if(check) {
    	m_text = to_string(model.cost);
	}
    std::string name = "Menu_01_action" + getStateAsStr();
	std::string text = gettext("Upgrade (") +  m_text +")";
	int up = 0;
	if(m_state == HOVER){
        up = 2;
    }

    if (m_state == DISABLED){
    	m_color = m_colorText;
    }

	C_TextureList& t= C_Locator::getTextureList();
	//t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height/2 - up,CENTER);
	t.loadTextAsTexturesIntoMap(m_textName, text, m_fontSize, m_color);
	t.renderText(m_textName, m_x_screen + m_width/2, m_y_screen + m_height/2 - up +2,CENTER);
}

//-------------------------------------------------------------

C_MenuStateIcon::C_MenuStateIcon(std::string name, S_Coord screen, int state)
        :C_MenuItem(name,screen.x,screen.y)
{
    m_state = state;
}

void C_MenuStateIcon::render()
{
    std::string img = m_image;
    if(m_state == ACTIVE){
        img += "_Active";
    } else {
        img += "_Disabled";
    }
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(img, m_x_screen + m_width/2,m_y_screen+m_height/2,CENTER);
}

