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

//Menu elements like progressbar and buttons

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>


class C_Command {
public:
	C_Command() {};
	virtual ~C_Command() {};
	virtual void action() = 0;
	virtual void actionHover(bool state){if(state){};};
	virtual void setNbr(int nbr)
	{
		m_nbr = nbr;
	};
	virtual bool getBool()
	{
		std::cout << "return false \n";
		return false;
	};
protected:
	int m_nbr;
};

class C_OpenMenu : public C_Command {
public:
	C_OpenMenu() {};
	virtual ~C_OpenMenu() {};
	virtual void action();
	virtual bool getBool();
};



class C_ChangeTab : public C_Command {
public:
	virtual void action();
};

class C_ChangeLevelLeft : public C_Command {
public:
	virtual void action();
};

class C_ChangeLevelRight : public C_Command {
public:
	virtual void action();
};

class C_LoadALevel : public C_Command {
public:
	virtual void action();
};

class C_CenterCamera : public C_Command {
public:
	virtual void action();
};

class C_Play : public C_Command {
public:
	virtual void action();
};

class C_UpgradeUnit : public C_Command {
public:
	virtual void action();
	virtual void actionHover(bool state);
};

class C_OpenEndLevelMenu : public C_Command {
public:
	virtual void action();
};
#endif
