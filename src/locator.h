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
/*!
 * \file locator.h
 */

#ifndef LOCATOR_H
#define LOCATOR_H

#include <iostream>
#include "message.h"

/*! \class C_Locator
 * \brief C_Locator find services .
 * http://gameprogrammingpatterns.com/service-locator.html
 */
class CC_Time{
public:
	virtual ~CC_Time(){};
	virtual void clock() = 0;
};

class C_Clock : public CC_Time
{
public:
	virtual void clock(){std::cout << "I am a clock" << std::endl;};
};

class C_Locator
{
public:
	static C_Clock& getMessage() { return *m_message; };
	static void provide(C_Clock* service){m_message = service;};

private:
	static C_Clock* m_message;
};

#endif

