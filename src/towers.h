#ifndef TOWERS_H
#define TOWERS_H

#include <iostream>
#include <string>
#include "weapons.h"
#include "shooter.h"
#include "surfaces.h"


class C_Towers: public C_Shooter
{
	public:
	//methods
	C_Towers();
	C_Towers(std::string name, int x_grid, int y_grid, int rank);

	virtual void play();
	void drag(S_Coord screen);

	protected:
	virtual void render(S_Coord screen);
	virtual void renderSelected();
	void renderSmoke();
	virtual void drawEllipse(int x,int y, int width, int animNbr, bool ok);
	void drawRhombus(int x, int y,int width, int alpha, bool ok);

	long m_lastSmokeTime;
	int m_smokeNbr;
	bool m_justAdded;
};


class C_ArcherTower: public C_Towers
{
	public:
		C_ArcherTower(int x_grid, int y_grid, int rank);
	protected:

};

class C_Turbine: public C_Towers
{
	public:
		C_Turbine(int x_grid, int y_grid, int rank);
		virtual void render(S_Coord screen);
		virtual void drawEllipse(int x_screen,int y_screen,int width,int animNbr,bool ok);
		virtual void changeDirection(int x_cursor, int y_cursor);
	protected:
		virtual	C_GameUnits* searchNextTarget(std::string type);

};




#endif
