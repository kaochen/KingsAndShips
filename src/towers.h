#ifndef TOWERS_H
#define TOWERS_H

#include <iostream>
#include <string>
#include "weapons.h"
#include "invaders.h"

class C_Towers
{
	public:
	//methods
	C_Towers();
	C_Towers(std::string name, std::string weaponName, int weaponDamage, int weaponFireRate);
	~C_Towers();
	void shoot(C_invaders &target);
	void displayStatus() const;
	int getLevel() const;


	private:
	//attibuts
	int m_level;
	std::string m_name;
	C_Weapon m_weapon;
};

#endif
