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

#include "message.h"
#include "locator.h"
#include "time.h"
#include "window.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;


void C_Message::printM(string message)
{
	timestamp();
	cout << "[LOG] " << message;
}

void C_Message::printV(string message)
{	C_Settings& settings= C_Locator::getSettings();
	if(settings.getVerboseMode()) {
		timestamp();
		cout << "\033[1;34m[LOG] \033[0m" << message;
	}
}

void C_Message::printError(string message)
{
	timestamp();
	cout << "\033[1;31m[ERR] \033[0m" << message;
}

void C_Message::printDebug(string message)
{
	C_Settings& settings= C_Locator::getSettings();
	if(settings.getDebugMode()) {
		timestamp();
		cout << "[DEBUG] " << message;
	}
}

void C_Message::printDebugPath(string message)
{
	C_Settings& settings= C_Locator::getSettings();
	if(settings.getDebugPathMode()) {
		timestamp();
		cout << "[PATH] " << message;
	}
}

void C_Message::printSDLerror(string message)
{
	timestamp();
	cout << "[SDL_] " << message << " " << SDL_GetError() << endl;
	SDL_ClearError();
}

void C_Message::printTTFerror(string message)
{
	timestamp();
	cout << "[TTF_] " << message  << " " << TTF_GetError() << endl;
	SDL_ClearError();
}



void C_Message::timestamp()
{
	Sint32 current = SDL_GetTicks();
	int size = 4;
	string time[size] = {""};
	time[0] = to_string(current%1000); //milli
	time[1] = to_string(current/1000); //sec
	time[2] = to_string(current/(1000*60));//min
	time[3] = to_string(current/(1000*60*60));//hour

	//0:1:2:3 -> 00:01:02:03
	for(int i = 0; i < size; i++) {
		if(time[i].length()==1) {
			time[i] = "0" + time[i];
		}
	}
	//00:01:02:03 -> 00:01:02:003
	if(time[0].length()==2) {
		time[0] = "0" + time[0];
	}

	cout << time[3] << ":" <<  time[2] << ":" << time[1] << ":" <<  time[0] << " ";
}
