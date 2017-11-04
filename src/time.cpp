#include "time.h"
#include <string>

using namespace std;

//constructor

C_Time::C_Time():
	m_frameNbr(0),
	m_framerate(FRAMERATE)
{
 	m_startTime = SDL_GetTicks();
 	m_currentTime = SDL_GetTicks();
}

C_Time::~C_Time(){
}

void C_Time::displayTime()
{
	double uptime = SDL_GetTicks() - m_startTime;
	int sec = uptime /1000;
	string milliSec = to_string(uptime);
	milliSec.erase(0,1);
	milliSec.erase(3,7);
	cout << "Time: "<< sec << "s"<< milliSec;
	cout << " - frame:" << m_frameNbr << "/" << m_framerate << endl;
}

void C_Time::updateFrameNbr()
{
	m_frameNbr++;
	if (m_frameNbr > m_framerate)
		m_frameNbr = 1;
}
