#include "time.h"
#include <string>

using namespace std;

//constructor

C_Time C_Time::m_instance=C_Time();

C_Time::C_Time():
	m_frameNbr(0),
	m_sec(0),
	m_lastSec(0),
	m_framerate(FRAMERATE),
	m_delay(0)
{
	m_frame_duration = 1000/m_framerate;
	m_lastFrameTime = 0;
	m_previousFrameNbr = -1;
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

void C_Time::updateFrameNbr()
{
	long current = SDL_GetTicks();
	if ((current - m_lastFrameTime) > m_frame_duration){
			m_frameNbr++;
			m_lastFrameTime = current;
			}

	if (m_sec != m_lastSec){
		m_frameNbr = 1;
		m_lastSec = m_sec;
		}
}

void C_Time::updateTime()
{
	m_currentTime = SDL_GetTicks();
	m_sec = m_currentTime/1000;
	long duration = m_currentTime - m_previousTime;
	long delay = m_frame_duration - duration;
	cout << "Frame duration:" << duration;
	if ( duration < m_frame_duration){
			if (delay < 0){
				delay = 0;
				}
			SDL_Delay(delay);
			}
		else{
			m_previousTime = m_currentTime;
			}

	cout << " delay: " << delay << endl;
}

long C_Time::getSec()
{
	return m_sec;
}

long C_Time::getFrameNbr()
{
	return m_frameNbr;
}

long C_Time::getFramerate()
{
	return m_frameNbr;
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
