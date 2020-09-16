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
#include "../textureList.h"
#include "../locator.h"
#include "../level/grid.h"

using namespace std;
int C_Sentence::id = 0;

C_Sentence::C_Sentence(string text, S_Coord screen):
	m_text(text)
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
	t.loadTextAsTexturesIntoMap(m_name, m_text, m_fontSize, m_color);
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

 //---------------------------C_Panel-------------------------------------------
int C_Panel::id = 0;

C_Panel::C_Panel(){
	m_screen = {0,0};
	++id;
	m_name =  "panel_" + to_string(id);
}
C_Panel::~C_Panel(){
	for(auto const& x : m_sentences) {
		if(x.second != nullptr)
			delete  x.second;
	}
}


void C_Panel::addLine(string name, string text, S_Coord screen, string color){
	if(m_sentences[name] == nullptr)
		m_sentences[name]= new C_Sentence(text, screen);

	else
		m_sentences[name]->update(text);

	m_sentences[name]->changeColor(color);

}



C_EndLevelMenu::C_EndLevelMenu():
	m_open(false)
{
	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_levelStatus = ONGOING;

	S_Coord screen = {0,0};
	string text = "Your castle is safe for now";
	addLine("win", text, screen, "black");
	text = "You lost this castle";
	addLine("lose", text, screen, "black");
	text = "The game continue";
	addLine("ongoing", text, screen, "black");
}

void C_EndLevelMenu::render(){
	if(m_open){
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Menu_01_parchment", m_screen.x,m_screen.y,CENTER);
		if(m_levelStatus == WIN)
			m_sentences["win"]->render(m_screen,CENTER);
		else if(m_levelStatus == LOSE)
			m_sentences["lose"]->render(m_screen,CENTER);
		else if(m_levelStatus == ONGOING)
			m_sentences["ongoing"]->render(m_screen,CENTER);
	}
}

