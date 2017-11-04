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
	void displayTime();
	void updateFrameNbr();
	protected:

	private:
	double m_frameNbr;
	double m_startTime;
	double m_currentTime;
	int m_framerate;
};


#endif
