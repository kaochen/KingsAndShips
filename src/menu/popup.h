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
/*!
 * \file popup.h
 */

#ifndef POPUP_H
#define POPUP_H

#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "../coord.h"
#include "../level/gameUnits.h"

/*! \class C_Popup
 * \brief C_Popup display informations when passing over a Tower.
 */

class C_Sentence{
public:

	C_Sentence(std::string text, S_Coord screen);
	~C_Sentence(){};
	void render(S_Coord screen);
	void update(std::string text);
private:
	static int id;
	std::string m_name;
	std::string m_text;
	std::string m_oldText;
	S_Coord m_screen;
	SDL_Color m_color;
	int m_fontSize;
};

class C_Popup{
public:
	C_Popup();
	~C_Popup(){};
	void render(S_Coord screen);
	void getInfo(S_UnitModel unit);
private:
	void addLine(std::string name, std::string text, S_Coord screen);
	static int id;
	std::string m_name;
	std::map<std::string, C_Sentence*> m_sentences;
};

#endif
