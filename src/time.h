#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <SDL2/SDL.h>

#include "game.h"


class C_Time: public C_Game
{
	public:
	C_Time();
	~C_Time();
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
