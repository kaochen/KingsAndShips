#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <SDL2/SDL.h>

#include "game.h"


class C_Time
{
	public:
	static	C_Time& Instances();
	void displayTime() const;
	void updateFrameNbr();
	void updateTime();

	long getSec();
	long getFrameNbr();
	long getFramerate();
	long getDelay();
	bool testNewFrame();


	protected:

	private:
	C_Time& operator= (const C_Time&){return *this;}
	C_Time (const C_Time&){}

	static C_Time m_instance;
	C_Time();
	~C_Time();

	long m_frameNbr;
	long m_previousFrameNbr;
	int m_sec;
	int m_lastSec;

	long m_currentTime;
	long m_previousTime;
	long m_lastFrameTime;
	long m_framerate;
	long m_frame_duration;
	long m_delay;
};


#endif
