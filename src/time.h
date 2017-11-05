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

	int getSec();
	int getFrameNbr();
	int getFramerate();
	double getDelay();
	bool testNewFrame();


	protected:

	private:
	double m_frameNbr;
	double m_previousFrameNbr;
	int m_sec;
	int m_lastSec;

	double m_startTime;
	double m_currentTime;
	double m_previousTime;
	double m_lastFrameTime;
	int m_framerate;
	double m_frame_duration;
	double m_delay;
};


#endif
