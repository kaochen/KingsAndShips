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
	void updateFrameNbr(long startTime, long finishTime);

	void updateTime();
	void delayGameLoop();
	long getSec();
	long getFrameNbr();
	long getFramerate();
	long getFrameDuration();
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
	long m_lastFrameTime;
	long m_lastFrameDuration;
	long m_framerate;
	long m_frame_duration;
	long m_delay;
};

#define MAX_ANIM  5
enum AnimTime{JUSTADDED,MAIN_ANIM,SELECTED,DRAG,MOVE};

class C_AnimTime
{
	public:
	C_AnimTime();
	~C_AnimTime();

	int getAnimNbr(int startNbr, int endNbr, long delay);

	protected:
	private:
	long m_animNbr;
	long m_lastAnimTime;
};

#endif
