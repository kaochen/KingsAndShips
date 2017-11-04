#include "time.h"
#include <string>

using namespace std;

//constructor

C_Time::C_Time():
	m_frameNbr(0),
	m_sec(0),
	m_framerate(FRAMERATE),
	m_delay(0)
{
 	m_startTime = SDL_GetTicks();
 	m_currentTime = m_startTime;
	m_frame_duration = 1000/m_framerate;
	m_lastFrameTime = 0;
}

C_Time::~C_Time(){
}

void C_Time::displayTime() const
{
	string milliSec = to_string(m_currentTime);
	milliSec.erase(0,1);
	milliSec.erase(3,7);
	cout << "Time: "<< m_sec << "s"<< milliSec;
	cout << " - frame:" << m_frameNbr << "/" << m_framerate << endl;
}

void C_Time::updateFrameNbr()
{
	if ((m_currentTime - m_lastFrameTime) > m_frame_duration){
			m_frameNbr++;
			m_lastFrameTime = m_currentTime;
			}
	if (m_frameNbr > m_framerate)
		m_frameNbr = 1;
}

void C_Time::updateTime()
{
	m_currentTime = SDL_GetTicks() - m_startTime;
	m_sec = m_currentTime/1000;
	if ((m_currentTime - m_previousTime) < m_frame_duration){
			int delay = (m_frame_duration - m_currentTime - m_previousTime);
			cout << "delay: " << delay << endl;
			SDL_Delay(delay);
			}
		else{
			m_previousTime = m_currentTime;
			}
}

int C_Time::getSec()
{
	return m_sec;
}

int C_Time::getFrameNbr()
{
	return m_frameNbr;
}

int C_Time::getFramerate()
{
	return m_frameNbr;
}

double C_Time::getDelay()
{
	return m_delay;
}

