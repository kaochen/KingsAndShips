#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <SDL2/SDL.h>

#include "settings.h"


class C_Time
{
	public:
	static	C_Time& Instances();
	void displayTime() const;
	void showFPS() const;

	void updateFrameTime();
	void delayGameLoop();
	long getSec();
	long getFrameNbr();
	long getFramerate();
	long getFrameDuration();
	long getDelay();
	bool testNewFrame();

	protected:
	void updateFrameNbr();

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
	long m_framerate;
	long m_frame_duration;
	long m_delay;
	long m_start_frame;
};

#define MAX_ANIM  6
enum AnimTime{JUSTADDED,MAIN_ANIM,SELECTED,DRAG,MOVE,PAUSESEARCHPATH};

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



class C_Message{
  public:
    C_Message();
    ~C_Message();
    void printM(std::string message);
    void printDebug(std::string message);
    void printDebugPath(std::string message);

  private:
    void timestamp();
};


#endif
