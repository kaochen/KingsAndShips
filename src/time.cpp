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


#include "time.h"
#include "window.h"
#include <string>

using namespace std;

//constructor

C_Time C_Time::m_instance=C_Time();

C_Time::C_Time():
	m_frameNbr(0),
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
}

C_Time::~C_Time(){
}

C_Time& C_Time::Instances()
{
	return m_instance;
}

void C_Time::displayTime() const
{
	cout << "Time: "<< SDL_GetTicks() <<  " s:"<< m_sec;
	cout << " - frame:" << m_frameNbr << "/" << m_framerate << endl;
}

void C_Time::showFPS() const
{
		C_Window& win=C_Window::Instances();
		SDL_Renderer* renderer = win.getRenderer();
		C_Set& settings=C_Set::Instances();
		//add a life status above the boat

		int red = 0, green = 200;
		SDL_Rect f, b, m;
		    b.x = settings.getWindowWidth() - TILE_HALF_WIDTH;
		    b.y = 3*TILE_HALF_HEIGHT;
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
		    for(int i = 0; i < 5; i++){
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

	if ((m_currentTime - m_start_frame) > (1000/FRAMERATE)){
		m_frameNbr++;
		}

	m_sec = m_currentTime/1000;
	if (m_sec != m_lastSec){
		m_frameNbr = 0;
		m_lastSec = m_sec;
		}
}

void C_Time::delayGameLoop()
{
	long delay = 0;
	long frameDuration = SDL_GetTicks() - m_start_frame;
		if (frameDuration < m_frame_duration){
			delay = m_frame_duration - frameDuration;
			}
	SDL_Delay(delay);
	updateFrameNbr();
}

void C_Time::updateFrameTime()
{
	m_start_frame = SDL_GetTicks();
}

long C_Time::getSec()
{
	return m_sec;
}

long C_Time::getFrameNbr()
{
	return m_frameNbr;
}

long C_Time::getFrameDuration()
{
	return m_frame_duration;
}

long C_Time::getFramerate()
{
	return m_framerate;
}

long C_Time::getDelay()
{
	return m_delay;
}


bool C_Time::testNewFrame(){
	if (m_frameNbr != m_previousFrameNbr){
		m_previousFrameNbr = m_frameNbr;
		return true;
		}
	else
		return false;
}

//-------------------------------
C_AnimTime::C_AnimTime():
	m_animNbr(0),
	m_lastAnimTime(0)
{
}

C_AnimTime::~C_AnimTime()
{
}

int C_AnimTime::getAnimNbr(int startNbr, int endNbr, long delay){
	long current = SDL_GetTicks();
	if (current > m_lastAnimTime + delay){
		m_animNbr++;
		m_lastAnimTime = current;
		}
	//loop
	if (m_animNbr > endNbr){
		m_animNbr = startNbr;
	}
	return m_animNbr;
}



//---------------C_Message -----------------

    C_Message::C_Message()
    {
    }
    C_Message::~C_Message()
    {
    }

    void C_Message::printM(string message)

    {
        timestamp();
        cout << "[LOG] " << message;
    }

    void C_Message::printDebug(string message)

    {
       C_Set& settings=C_Set::Instances();
        if(settings.getDebugMode()){
            timestamp();
            cout << "[DEBUG] " << message;
        }

    }

    void C_Message::printDebugPath(string message)

    {
       C_Set& settings=C_Set::Instances();
        if(settings.getDebugPathMode()){
            timestamp();
            cout << "[PATH] " << message;
        }

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

        for(int i = 0; i < size; i++){
            if(time[i].length()==1){
                        time[i] = "0" + time[i];
            }
        }

        cout << time[3] << ":" <<  time[2] << ":" << time[1] << ":" <<  time[0] << " ";
    }
