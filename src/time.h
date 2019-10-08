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
#include <map>

#include "settings.h"


class C_Time
{
	public:
	C_Time();
	virtual ~C_Time(){};
	void displayTime() const;
	void showFPS();

	void updateFrameTime(){m_start_frame = SDL_GetTicks();};
	void delayGameLoop();
	long getSec(){return m_sec;};
	long getFrameNbr(){return m_frameNbr;};
  long getFrameNbrFromStart(){return m_frameNbrFromStart;};
	long getFramerate(){return m_framerate;};
	long getFrameDuration(){return m_frame_duration;};
	long getDelay(){return m_delay;};
	bool testNewFrame();

	protected:
	void updateFrameNbr();

	private:


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
  long m_fpsMax;
  long m_fpsLast;
};

#define MAX_ANIM  7
enum AnimTime{JUSTADDED,MAIN_ANIM,SELECTED,DRAG,MOVE,PAUSESEARCHPATH,SELECT};

class C_AnimTime
{
	public:
	C_AnimTime();
	~C_AnimTime(){};

  void setAnimNbr(const long &nbr){m_animNbr = nbr;};
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

#endif
