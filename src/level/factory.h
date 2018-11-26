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


#ifndef FACTORY_H
#define FACTORY_H

#include <iostream>
#include <string>
#include <map>
#include "gameUnits.h"
#include "shooter.h"
#include "towers.h"
#include "boat.h"
#include "../xml.h"

template <typename T>
class C_Singleton{
public:
  	static	T& Instances() {return m_instance;};

protected:
    T& operator= (const T&){}
    C_Singleton(){};
    ~C_Singleton(){};

private:
  static T m_instance;
};
template <typename T>
T C_Singleton<T>::m_instance;



class C_UnitFactory : public C_Singleton<C_GameUnits>
{
  friend class C_Singleton<C_GameUnits>;
  public:
    C_UnitFactory();
    virtual ~C_UnitFactory(){};
    virtual C_GameUnits* create(std::string type, S_Coord pos);
    virtual void setRank(std::string type, int rank){m_models[type].rank= rank;};
  protected:
  private:
  	std::map<std::string, S_UnitModel> m_models;
};

#endif
