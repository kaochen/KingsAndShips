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


#include "time.h"
#include "window.h"
#include "locator.h"
#include <string>

using namespace std;

//constructor

C_Time::C_Time():
	m_frameNbr(0),
	m_frameNbrFromStart(0),
	m_previousFrameNbr(-1),
	m_sec(0),
	m_lastSec(-1),
	m_currentTime(0),
	m_framerate(FRAMERATE),
	m_delay(0),
	m_start_frame(0),
	m_fpsMax(-1),
	m_fpsLast(m_fpsMax)
{
	m_frame_duration = 1000/m_framerate;
	m_lastFrameTime = 0;
	C_Message::printM("Constructor C_Time() : done\n");
}



void C_Time::displayTime() const
{
	cout << "Time: "<< SDL_GetTicks() <<  " s:"<< m_sec;
	cout << " - frame:" << m_frameNbr << "/" << m_framerate << endl;
}

void C_Time::showFPS()
{
	C_Settings& settings= C_Locator::getSettings();
	int x_screen = settings.getWindowWidth() - 80;
	int y_screen = settings.getWindowHeight() - 2;
	C_TextureList& t= C_Locator::getTextureList();
	string textureName = "Menu_details_black";
	if(m_frameNbr > m_fpsLast)
		m_fpsLast = m_frameNbr;
	if(m_frameNbr <= 0){
		m_fpsMax = m_fpsLast + 1;
		m_fpsLast = -1;
	}

	//background
	for (int i = 0; i < 36 ; i++){
		t.renderTexture("Menu_details_white", x_screen + (i*2) ,y_screen - 10 ,CENTER);
	}
	//text
	string fpsText = "FPS ("+ to_string (m_framerate) +"): " + to_string(m_fpsMax) ;
		if( fpsText !="") {
			SDL_Color m_color = {0,0,0,255};
			t.loadTextAsTexturesIntoMap("fpsText", fpsText, 10, m_color);
			t.renderTexture("fpsText", x_screen , y_screen -10, LEFT);
		}

}

void C_Time::updateFrameNbr()
{
	m_currentTime = SDL_GetTicks();
	if ((m_currentTime - m_start_frame) > (1000/FRAMERATE)) {
		m_frameNbr++;
		m_frameNbrFromStart++;
		//cout << m_frameNbr << "/" << m_frameNbrFromStart << endl;
	}

	m_sec = m_currentTime/1000;
	if (m_sec != m_lastSec) {
		m_frameNbr = 0;
		m_lastSec = m_sec;
	}
}

void C_Time::delayGameLoop()
{
	long delay = 0;
	long frameDuration = SDL_GetTicks() - m_start_frame;
	if (frameDuration < m_frame_duration) {
		delay = m_frame_duration - frameDuration;
	}
	SDL_Delay(delay);
	updateFrameNbr();
}


bool C_Time::testNewFrame()
{
	if (m_frameNbr != m_previousFrameNbr) {
		m_previousFrameNbr = m_frameNbr;
		return true;
	} else
		return false;
}


