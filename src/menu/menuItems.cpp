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
#include "../texture.h"
#include "../message.h"
#include "../level/factory.h"
#include "../level/grid.h"
#include <string>


using namespace std;
C_MenuItem::C_MenuItem(string name, int x_screen, int y_screen):
	m_type(ACTION),
	m_name(name),
	m_image(name),
	m_x_screen(x_screen),
	m_y_screen(y_screen),
	m_width(64),
	m_height(64),
	m_layer(FRONT),
	m_enable(true),
	m_state(ACTIVE)
{
	m_textName = name + "Text";
	m_text = "";
	m_color = {200,200,200,255};
	m_x_text = 0;
	m_y_text = 0;
	m_command = nullptr;
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
void C_MenuItem::setTextPosition(int x_text, int y_text)
{
	m_x_text = x_text;
	m_y_text = y_text;
}


void C_MenuItem::renderText()
{
	C_TextureList& t= C_Locator::getTextureList();
	if(m_text !="") {
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
		t.renderTexture(m_textName, m_x_screen + m_width/2, m_y_screen +  m_height/2,CENTER);
	}
}

void C_MenuItem::render()
{
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(m_image, m_x_screen + m_width/2,m_y_screen+m_height/2,CENTER);
	renderText();
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

void C_MenuItem::background(){
	C_TextureList& t= C_Locator::getTextureList();
	string textureName = "Menu_details_background_line_";
	if(m_state == HOVER){
		textureName +="hover";
	} else {
		textureName +="active";
	}
	for(int j= 0; j <= 1; j++){
		Sint16 y1 = m_y_screen + 2 + m_height/4  + 11*j;
		t.renderTexture(textureName,m_x_screen ,y1,CENTER);
		t.renderTexture(textureName,m_x_screen + m_width ,y1,CENTER);
		for(int i = 0; i <= (m_width/4); i++){
			t.renderTexture(textureName, m_x_screen + 4*i, y1, CENTER);
		}
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
		if(settings.getPlaying() == PLAY) {
			name = "Buttons_pause";
		}
	}

	string prefix;
	if(m_state == ACTIVE) {
		prefix ="_Active";
	} else if(m_state == HOVER) {
		prefix ="_Hover";
	} else {
		prefix ="_Disabled";
	}

	if(m_enable == false) {
		prefix ="_Disabled";
	}
	name += prefix;
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height/2,CENTER);
}

//-------------------------------------------------------------

C_MB_TabSelect::C_MB_TabSelect(string name,string text, int fontSize,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	setText(text, fontSize);
	m_width = 100;
	m_height = 30;
	m_color = {165,146,113,255};
}


void C_MB_TabSelect::render()
{

	SDL_Color color = m_color;

	if(m_state == HOVER){
		m_color = {195,176,143,255};
	}

	background();

	C_TextureList& t= C_Locator::getTextureList();
	Sint16 y1 = m_y_screen + m_height/2;

	if(m_text !="") {
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, color);
		t.renderTexture(m_textName, m_x_screen + m_width/2, y1 ,CENTER);
	}
}

//-------------------------------------------------------------

C_MB_1Line::C_MB_1Line(string name,string text,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	m_fontSize = 18;
	m_title = name;
	m_titleName = "Settings_Title_" + name;
	m_text = text;
	m_textName = "Settings_Text_" + name;
	m_width = 408;
	m_height = 24;
	m_color = {165,146,113,255};
}


void C_MB_1Line::render()
{
	SDL_Color color = m_color;

	if(m_state == HOVER){
		color = {23,4,0,255};
	}

	background();

	C_TextureList& t= C_Locator::getTextureList();
	Sint16 y1 = m_y_screen + m_height/2;

	int border = 4;
	if(m_title !="") {
		t.loadTextAsTexturesIntoMap(m_titleName, m_title, m_fontSize, m_color);
		t.renderTexture(m_titleName, m_x_screen+border, y1, LEFT);
	}

	if(m_text !="") {
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
		t.renderTexture(m_textName, m_x_screen + m_width-border, y1, RIGHT);
	}
}

//-------------------------------------------------------------

C_MB_LevelCard::C_MB_LevelCard(int nbr, string name,int x_screen, int y_screen)
	:C_MenuItem(name,x_screen,y_screen)
{
	m_nbr = nbr;
	C_Window& win= C_Locator::getWindow();

	S_LevelModel model = win.getLevelModel(m_nbr);

	m_fontSize = 18;
	m_title = model.name;
	m_titleName = "Card_Title_" + name;

	m_id = to_string(model.nbr);
	m_idName = "Card_id_" + name;

	m_text = "Grid Size: " + to_string(model.gridSize);
	m_textName = "Card_Text_" + name;
	m_width = 144;
	m_height = 218;
}

void C_MB_LevelCard::render()
{
	string name = "Parchment_Card";

	string prefix;
	if(m_state == ACTIVE) {
		prefix ="_Active";
	} else if(m_state == HOVER) {
		prefix ="_Hover";
	} else {
		prefix ="_Disabled";
	}

	if(m_enable == false) {
		prefix ="_Disabled";
	}
	name += prefix;
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(name, m_x_screen + m_width/2,m_y_screen + m_height/2,CENTER);

    //Text
    m_color = {0,0,0,255};
	if(t.searchTexture(m_titleName)== nullptr) {
		t.loadTextAsTexturesIntoMap(m_titleName, m_title, m_fontSize, m_color);
	}
	t.renderTexture(m_titleName, m_x_screen + m_width/2, m_y_screen + 40,CENTER);

	if(t.searchTexture(m_idName)== nullptr) {
		t.loadTextAsTexturesIntoMap(m_idName, m_id, m_fontSize - 4, m_color);
	}
	t.renderTexture(m_idName, m_x_screen + 16, m_y_screen + 20,LEFT);

	if(t.searchTexture(m_textName)== nullptr) {
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize - 4, m_color);
	}
	t.renderTexture(m_textName, m_x_screen + m_width/2, m_y_screen + m_height - 40,CENTER);
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
	} else if(name == "AddTurbine") {
		unit.name = "Turbine_0";
		m_unit = factory.create(unit);
	} else if(name == "AddBarricade") {
		unit.name = "barricade_1";
		m_unit = factory.create(unit);
	}
	if(m_unit != nullptr)
		m_text = to_string(m_unit->getCost());
	else
		m_text = "empty";
	m_fontSize = 9;
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
		C_TextureList& t= C_Locator::getTextureList();
		m_color = {165,146,113,255};
		if(t.searchTexture(m_textName)== nullptr) {
			t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
		}
		t.renderTexture(m_textName, m_x_screen + 11, m_y_screen+10,CENTER);
	}
}
//-------------------------------------------------------------

C_GP_Status::C_GP_Status(string name,int x_screen, int y_screen, int colorIn, int colorOut)
	:C_MenuItem(name,x_screen,y_screen)
{
	m_type = STATUS;
	m_width = 128;
	m_height = 24;
	m_percentage = 100;
	m_oldPercentage = m_percentage;
	m_xOffset = 0;
	m_layer = BACK;
	m_y_text = -4;
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
	int y = m_y_screen + 13;
	int size = 6; //ProgressBar_Center are 6px wide
	int all = m_width/size;
	int mark = 0;
	if(m_percentage !=0) {
		mark = (m_percentage * m_width)/(100*size);
	}
	int xOffset = 0;
	if(m_oldPercentage !=100) {
		xOffset = (100 - m_percentage) * m_width/100;
	}
	string name = "ProgressBar_";
	for (int i = 0; i <= all; i++) {
		string image = name + "Center" ;
		if(i < mark) {
			if(i % 2 == 0) {
				image += "2_";
			} else {
				image += "1_";
			}
			image +=colorToStr(m_colorIn);
		} else {
			if(i % 2 == 0) {
				image += "2_";
			} else {
				image += "1_";
			}
			image +=colorToStr(m_colorOut);
		}
		t.renderTexture(image, m_x_screen + i*size - xOffset, y,CENTER);
	}

	string leftImage = name + "Left_";
	if(m_percentage > size)
		leftImage += colorToStr(m_colorIn);
	else
		leftImage += colorToStr(m_colorOut);

	t.renderTexture(leftImage, m_x_screen, y,CENTER);


	string rightImage = name + "Right_";
	if(100 - size <= m_percentage)
		rightImage += colorToStr(m_colorIn);
	else
		rightImage += colorToStr(m_colorOut);

	t.renderTexture(rightImage, m_x_screen + m_width, y,CENTER);
	renderText();
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
		return "Green";
	}

}


C_GU_Upgrade::C_GU_Upgrade(string name,S_Coord screen)
	:C_Button(name,"upgrade",screen.x,screen.y)
{
    m_type = ACTION;
    m_command = new C_UpgradeUnit();
    m_text = "0";
    m_textName = "upgrade_Text_" + name;
    m_fontSize = 12;
}


void C_GU_Upgrade::render()
{
	C_Button::render();

	C_Grid& grid= C_Locator::getGrid();
	C_UnitFactory factory = grid.getFactory();
	S_UnitModel model;
	bool check = factory.getSelectedModel(1,model);
	if(check) {
		m_text = to_string(model.cost);

		C_TextureList& t= C_Locator::getTextureList();
		t.loadTextAsTexturesIntoMap(m_textName, m_text, m_fontSize, m_color);
		t.renderTexture(m_textName, m_x_screen + 32, m_y_screen + 50,CENTER);
	}
}
