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
	m_start_frame(0)
{
	m_frame_duration = 1000/m_framerate;
	m_lastFrameTime = 0;
	cout << "Constructor C_Time() : done" << endl;
}



void C_Time::displayTime() const
{
	cout << "Time: "<< SDL_GetTicks() <<  " s:"<< m_sec;
	cout << " - frame:" << m_frameNbr << "/" << m_framerate << endl;
}

void C_Time::showFPS() const
{
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer* renderer = win.getRenderer();
	C_Settings& settings= C_Locator::getSettings();
	//add a life status above the boat

	int red = 0, green = 200;
	SDL_Rect f, b, m;
	b.x = 20;
	b.y = settings.getWindowHeight() - 2*TILE_HALF_HEIGHT;
	b.w = 10;
	b.h = m_framerate;

	f.x = b.x + 1;
	f.y = b.y + 1;
	f.w = b.w - 2;
	f.h = m_frameNbr;

	m.x = b.x - 2;
	m.y = b.y;
	m.w = b.w + 4;
	m.h = 2;

	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	//draw background
	SDL_RenderFillRect( renderer, &b );
	//draw 4 little lines
	for(int i = 0; i < 5; i++) {
		m.y = b.y + i*(b.h/4);
		SDL_RenderFillRect( renderer, &m );
	}
	//fill with green
	SDL_SetRenderDrawColor( renderer, red, green, 0, 255 );
	SDL_RenderFillRect( renderer, &f );
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

//-------------------------------
C_AnimTime::C_AnimTime():
	m_animNbr(0),
	m_lastAnimTime(0),
	m_lastFrameNbr(0),
	m_rewind(false)
{
}

C_AnimTime::~C_AnimTime()
{
}

int C_AnimTime::getAnimNbr(int startNbr, int endNbr, long delay)
{
	long current = SDL_GetTicks();
	if (current > m_lastAnimTime + delay) {
		m_animNbr++;
		m_lastAnimTime = current;
	}
	//loop
	if (m_animNbr > endNbr) {
		m_animNbr = startNbr;
	}
	return m_animNbr;
}

int C_AnimTime::getLoopAnimNbr(int startNbr, int endNbr, long delay)
{
	long current = SDL_GetTicks();
	if (current > m_lastAnimTime + delay) {
		if(m_rewind) {
			m_animNbr--;
		} else {
			m_animNbr++;
		}

		m_lastAnimTime = current;
	}
	//loop
	if (m_animNbr >= endNbr) {
		m_animNbr = endNbr;
		m_rewind = true;
	} else if (m_animNbr <= startNbr) {
		m_animNbr = startNbr;
		m_rewind = false;
	}

	return m_animNbr;
}





bool C_AnimTime::frameDelay(int delay)
{
	C_Time& time = C_Locator::getTime();
	long current = time.getFrameNbrFromStart();
	//cout << "current " << current << " lastFrameNbr " << m_lastFrameNbr << "+" << delay << endl;
	if(current > (m_lastFrameNbr + delay)) {
		m_lastFrameNbr = current;
		return true;
	} else {
		return  false;
	}
}

