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
#include <vector>
#include "gameUnits.h"
#include "shooter.h"
#include "barricade.h"
#include "towers.h"
#include "town.h"
#include "boat.h"
#include "level.h"
#include "../xml.h"

template <typename T>
class C_Singleton {
public:
	static	T& Instances()
	{
		return m_instance;
	};

protected:
	T& operator= (const T&) {}
	C_Singleton() {};
	~C_Singleton() {};

private:
	static T m_instance;
};
template <typename T>
T C_Singleton<T>::m_instance;



class C_UnitFactory {
public:
	C_UnitFactory();
	virtual ~C_UnitFactory() {};
	virtual C_GameUnits* create(S_Unit type);
	virtual void setRank(std::string type, int rank){m_models[type].rank= rank;};
  virtual void upgrade(C_GameUnits * unit);
  virtual bool isUpgradable(C_GameUnits * unit);
  virtual bool getSelectedModel(int increment, S_UnitModel &model);
protected:
	S_UnitModel extractProperties(std::string filename);
private:
	bool tsxExist(const std::string &file);
	std::map<std::string, S_UnitModel> m_models;
};


class C_LevelFactory {
public:
	C_LevelFactory();
	virtual ~C_LevelFactory() {};
	virtual C_Level* create(int levelNbr);
	virtual int getLastLevelNbr(){return m_lastLevelNbr;};
	S_LevelData getModel(int leveNbr);
protected:
	S_LevelData extractInfosFromTmx(int levelNbr);
	int calcGridSize(int width, int height);
	std::vector <S_LevelData> m_levelList;
	int m_lastLevelNbr;

};
#endif
