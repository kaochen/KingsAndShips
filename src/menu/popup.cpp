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


#include <string>
#include "popup.h"
#include "../texture.h"
#include "../locator.h"
#include "../level/grid.h"

using namespace std;
int C_Sentence::id = 0;

C_Sentence::C_Sentence(string text, S_Coord screen):
	m_text(text),
	m_oldText(text)
{
	++id;
	m_name = "Sentence_" + to_string(id);
	m_screen = screen;
	m_color = {0,0,0,255};
	m_color = {0,0,0,255};
	m_fontSize = 14;
}

void C_Sentence::render(S_Coord screen, int align){
	C_TextureList& t= C_Locator::getTextureList();
	bool checkColor = false;
	if(m_color.r != m_oldColor.r || m_color.g != m_oldColor.g
		|| m_color.b != m_oldColor.b || m_color.a != m_oldColor.a){
		checkColor = true;
	}


	if(t.searchTexture(m_name)== nullptr || m_text != m_oldText || checkColor) {
		t.loadTextAsTexturesIntoMap(m_name, m_text, m_fontSize, m_color);
		m_oldText = m_text;
		m_oldColor = m_color;
	}
	t.renderTexture(m_name, screen.x + m_screen.x, screen.y + m_screen.y,align);
}

void C_Sentence::update(string text){
	m_text = text;
}

void C_Sentence::changeColor(string color){
	if(color == "red")
		m_color = {200,0,0,255};
	else
		m_color = {0,0,0,255};

}



int C_Popup::id = 0;

C_Popup::C_Popup()
{
	++id;
	m_name =  "popup_" + to_string(id);
	m_mode = "normal";
}
C_Popup::~C_Popup(){
	for(auto const& x : m_sentences) {
		if(x.second != nullptr)
			delete  x.second;
	}
}

void C_Popup::render(S_Coord screen){
	C_TextureList& t= C_Locator::getTextureList();
	screen.x -=120;
	screen.y -=40;
	t.renderTexture("Parchment_Parchment", screen.x,screen.y,CENTER);
	m_sentences["line1"]->render(screen,CENTER);
	m_sentences["underline"]->render(screen,CENTER);
	m_sentences["line2.0"]->render(screen,LEFT);
	m_sentences["line2.1"]->render(screen,RIGHT);
	m_sentences["line3.0"]->render(screen,LEFT);
	m_sentences["line3.1"]->render(screen,RIGHT);
	m_sentences["line4.0"]->render(screen,LEFT);
	m_sentences["line4.1"]->render(screen,RIGHT);
	m_sentences["line5.0"]->render(screen,LEFT);
	m_sentences["line5.1"]->render(screen,RIGHT);
	m_sentences["line6.0"]->render(screen,LEFT);
	m_sentences["line6.1"]->render(screen,RIGHT);

}

void C_Popup::getInfo(S_UnitModel current){
	S_UnitModel unit = current;
	string color = "black";
	if(m_mode == "upgrade"){
		C_Grid& grid= C_Locator::getGrid();
		C_UnitFactory factory = grid.getFactory();
		S_UnitModel up;
		bool check = factory.getSelectedModel(1,up);
		if(check){
			unit = up;
		} else {
			unit = current;
		}
	}

	S_Coord screen = {0,-65};
	string text = current.type + " " + to_string(unit.rank);
	addLine("line1", text, screen, "black");
	screen.y +=4;
	text = "_________";
	addLine("underline", text, screen, "black");
	///
	screen.x = -55;
	screen.y +=20;
	addLine("line2.0","Health " , screen, "black");
	screen.x = 55;
	addLine("line2.1",to_string(current.health) , screen, "black");

	screen.x = -55;
	screen.y +=20;
	addLine("line3.0","Firerange " , screen, "black");
	screen.x = 55;
	if( unit.weapon.fireRange > current.weapon.fireRange){
		addLine("line3.1",to_string(unit.weapon.fireRange), screen, "red");
	} else {
		addLine("line3.1",to_string(current.weapon.fireRange), screen, "black");
	}

	screen.x = -55;
	screen.y +=20;
	addLine("line4.0","Firerate ", screen, "black");
	screen.x = 55;
	if( unit.weapon.fireRate < current.weapon.fireRate){
		addLine("line4.1",to_string(unit.weapon.fireRate), screen, "red");
	} else {
		addLine("line4.1",to_string(current.weapon.fireRate), screen, "black");
	}

	screen.x = -55;
	screen.y +=20;
	addLine("line5.0","Damage ", screen, "black");
	screen.x = 55;
	if( unit.weapon.damage > current.weapon.damage){
		addLine("line5.1",to_string(unit.weapon.damage), screen, "red");
	} else {
		addLine("line5.1",to_string(current.weapon.damage), screen, "black");
	}

	screen.x = -55;
	screen.y +=20;
	addLine("line6.0","Speed Impact ", screen, "black");
	screen.x = 55;
	if( unit.weapon.speedImpact > current.weapon.speedImpact){
		addLine("line6.1",to_string(unit.weapon.speedImpact), screen, "red");
	} else {
		addLine("line6.1",to_string(current.weapon.speedImpact), screen, "black");
	}

}

void C_Popup::addLine(string name, string text, S_Coord screen, string color){
	if(m_sentences[name] == nullptr)
		m_sentences[name]= new C_Sentence(text, screen);

	else
		m_sentences[name]->update(text);

	m_sentences[name]->changeColor(color);

}


void C_Popup::setMode(std::string mode){
	if(mode == "upgrade" || mode == "normal")
		m_mode = mode;
}


C_EndLevelMenu::C_EndLevelMenu(){
	C_Settings& settings=C_Locator::getSettings();
	m_size.w = 100;
	m_size.h = 200;
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
}

void C_EndLevelMenu::render(){
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("Parchment_Parchment", m_screen.x,m_screen.y,CENTER);
}
