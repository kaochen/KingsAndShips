#ifndef SHOOTER_H
#define SHOOTER_H

#include <iostream>
#include <string>
#include "gameUnits.h"
#include "weapons.h"
#include "surfaces.h"

class C_Shooter : public C_GameUnits
{
	public:
	C_Shooter(std::string name, int x_grid, int y_grid, int rank);
	virtual ~C_Shooter();

	virtual	void displayStatus() const;
	virtual void shoot(std::string type);
	virtual void move();

	protected:

	virtual void renderLifeBar(int x_screen, int y_screen);
	virtual void render(S_Coord screen);
	virtual	C_GameUnits* searchNextTarget(std::string type);
	virtual void shootTarget(C_GameUnits &target);


	C_Weapon *m_weapon;
	long m_lastShootTime;
};

#endif
