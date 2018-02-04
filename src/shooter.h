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
	virtual void shoot();
	virtual void move();

	virtual void render(S_Coord screen);

	protected:

	virtual	C_GameUnits* searchNextTarget(std::string type);
	virtual void shootTarget(C_GameUnits &target);
	virtual void renderMissile();


	C_Weapon *m_weapon;
	long m_lastShootTime;
};

#endif
