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


#ifndef ANIM_H
#define ANIM_H

#include <iostream>
#include <SDL2/SDL.h>
#include <map>
#include <string>


class C_Anim
{
public:
	C_Anim();
	C_Anim(std::string name, int imageStart,int imageEnd,long delay);
	~C_Anim(){};
	void play();
	void playAndRewind();
	void reset();
	int getImageNbr();
	std::string getName(){return m_name;};
	void status();
	bool end();
	void start();
private:
	std::string m_name;

	int m_imageStart;
	int m_imageEnd;
	int m_imageCurrent;

	bool m_started;
	long m_timeStart;
	long m_timeDelay;
	long m_timeLast;

	bool m_rewind;
};


class C_AnimList
{
public:
	C_AnimList();
	~C_AnimList(){};
	void add(C_Anim anim);
	int getImageNbr(std::string name);
	C_Anim& get(std::string name);
	void play(std::string name);
	void playAndRewind(std::string name);
	bool end(std::string name){return get(name).end();};
	void reset(std::string name){get(name).reset();};
	void start(std::string name){get(name).start();};
	void show();

private:
	std::map<std::string, C_Anim> m_list;
};
#endif
