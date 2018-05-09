/*
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
*/


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
  long getFrameNbrFromStart();
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
	long m_frameNbrFromStart;
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

	int getAnimNbr(int startNbr, int endNbr, long delay); //at the endNbr the numbers restart to startNbr
  int getLoopAnimNbr(int startNbr, int endNbr, long delay); //at the endNbr the numbers goes back
  bool frameDelay(int delay);

	protected:
	private:
	long m_animNbr;
	long m_lastAnimTime;
	long m_lastFrameNbr;
  bool m_rewind;
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
