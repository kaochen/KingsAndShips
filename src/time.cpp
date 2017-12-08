#include "time.h"
#include <string>

using namespace std;

//constructor

C_Time C_Time::m_instance=C_Time();

C_Time::C_Time():
	m_frameNbr(0),
	m_previousFrameNbr(-1),
	m_sec(0),
	m_lastSec(0),
	m_currentTime(0),
	m_framerate(FRAMERATE),
	m_delay(0)
{
	m_frame_duration = 1000/m_framerate;
	m_lastFrameTime = 0;
	m_lastFrameDuration = m_frame_duration;
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
	cout << " - frame:" << m_frameNbr << "/" << m_framerate;
	cout << " Last Frame duration: " << m_lastFrameDuration << endl;
}

void C_Time::updateFrameNbr(long startTime, long finishTime)
{
	m_lastFrameDuration = finishTime - startTime;
	if (m_sec != m_lastSec){
		m_frameNbr = 0;
		m_lastSec = m_sec;
		}
	else{
		m_frameNbr++;
		}
}

void C_Time::delayGameLoop()
{
	long delay = 0;
	if (m_frameNbr != m_previousFrameNbr){
		if (m_lastFrameDuration < m_frame_duration){
			delay = m_frame_duration - m_lastFrameDuration;
			}
		}
		else{
			delay = 0;
			int jump = m_lastFrameDuration/m_frame_duration;
			m_frameNbr = jump;
		}

	//cout << "Delay: " << delay << endl;
	SDL_Delay(delay);
}

void C_Time::updateTime()
{
	m_currentTime = SDL_GetTicks();
	m_sec = m_currentTime/1000;
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
