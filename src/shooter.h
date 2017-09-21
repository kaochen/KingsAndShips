#ifndef SHOOTER_H
#define SHOOTER_H

#include <iostream>
#include <string>
#include "gameUnits.h"
#include "weapons.h"

class C_Shooter : public C_GameUnits
{
	public:
	C_Shooter();
	C_Shooter(std::string name, int grade);
	virtual ~C_Shooter();
	virtual	void displayStatus() const;
	virtual void shoot(C_Shooter &target);

	protected:
	C_Weapon *m_weapon;
};

#endif
