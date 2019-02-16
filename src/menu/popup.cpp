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

using namespace std;
int C_Sentence::id = 0;

C_Sentence::C_Sentence(string text):
	m_text(text),
	m_oldText(text)
{
	++id;
	m_name = "Sentence_" + to_string(id);
	m_screen = {0,0};
	m_color = {0,0,0,255};
	m_fontSize = 16;
}

void C_Sentence::render(S_Coord screen){
	C_TextureList& t=C_TextureList::Instances();
	if(t.searchTexture(m_name)== nullptr || m_text != m_oldText) {
		t.loadTextAsTexturesIntoMap(m_name, m_text, m_fontSize, m_color);
		m_oldText = m_text;
	}
	t.renderTexture(m_name, screen.x + m_screen.x, screen.y + m_screen.y,CENTER);
}

void C_Sentence::update(string text){
	m_text = text;
}



int C_Popup::id = 0;

C_Popup::C_Popup()
{
	++id;
	m_name =  "popup_" + to_string(id);
}

void C_Popup::render(S_Coord screen){
	C_TextureList& t=C_TextureList::Instances();
	screen.x -=120;
	screen.y -=40;
	t.renderTexture("Parchment_Parchment", screen.x,screen.y,CENTER);
	screen.y -=60;
	m_sentences["line1"]->render(screen);
	screen.y +=20;
	m_sentences["line2"]->render(screen);
	screen.y +=20;
}

void C_Popup::getInfo(S_UnitModel unit){
	if(m_sentences["line1"] == nullptr)
		m_sentences["line1"]= new C_Sentence(unit.name);
	else
		m_sentences["line1"]->update(unit.name);

	if(m_sentences["line2"] == nullptr)
		m_sentences["line2"]= new C_Sentence(to_string(unit.rank));
	else
		m_sentences["line2"]->update(to_string(unit.rank));
}
