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


#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>

class C_Message {
public:
	static void printM(std::string message);
	static void printV(std::string message);
	static void printError(std::string message);
	static void printDebug(std::string message);
	static void printDebugPath(std::string message);
	static void printSDLerror(std::string message);
	static void printTTFerror(std::string message);

private:
	static void timestamp();
};

class C_Text {
public:
    C_Text(){};
	C_Text(std::string text, std::string translatedText);
    virtual std::string& getText(){return m_text;};
    virtual void setText(std::string text) {m_text = text;};
    virtual std::string& getTranslatedText(){return m_translatedText;};
    virtual void setTranslatedText(std::string translatedText) {m_translatedText = translatedText;};
private:
    std::string m_text;
    std::string m_translatedText;
};


#endif
