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

//design the background of the level, grass, water...
#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <iostream>
#include <string>
#include "gameUnits.h"

class C_Landscape {
public:
	C_Landscape(S_Coord town);
	~C_Landscape();
	void render(int gridSize);
	void play();
	void renderBottomMask(int gridSize);
	void renderTopMask(int gridSize);
protected:
private:
	//methods
	int waterDirection(S_Coord town);

	//attributes
	S_Coord m_waterDrift;
	int m_waterDirection;
	C_Anim* m_animWater;
};

class C_Decors : public C_GameUnits {
public:
	C_Decors(S_Unit unit):C_GameUnits(unit.name, unit.coord.x, unit.coord.y,0){};
	virtual void render(S_Coord screen);
	virtual void render()
	{
		std::cout << ":D";
	};
protected:
private:
};

class C_Clouds : public C_GameUnits {
public:
	C_Clouds(int x_grid, int y_grid);
	virtual void render();
	virtual void play();
protected:
	int m_type;
	int m_typeOnTop;
	std::string m_cloudName;
	std::string m_cloudOnTopName;
	std::string m_shadowName;
	std::string m_shadowOnTopName;
	int m_randStart;
};


class C_Ground : public C_GameUnits {
public:
	C_Ground(S_Unit unit):C_GameUnits(unit.name, unit.coord.x, unit.coord.y,0){};
	virtual void render();
};

class C_Water : public C_GameUnits {
public:
	C_Water(S_Unit unit):C_GameUnits(unit.name, unit.coord.x, unit.coord.y,0){};
	virtual void render(){};
};

class C_Trees : public C_Decors {
public:
	C_Trees(S_Unit unit);
	virtual void play();
	virtual void render(S_Coord screen);

private:
};


class C_OutsideTile: public C_GameUnits {
public:
	C_OutsideTile();
	virtual void render(S_Coord grid);
};
#endif
