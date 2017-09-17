#ifndef INVADERS_H
#define INVADERS_H

#include <iostream>
#include <string>
#include "weapons.h"

class C_invaders
{
	public:
	//methods
	C_invaders();
	C_invaders(std::string name, std::string weaponName, int weaponDamage, int weaponFireRate);
	~C_invaders();
	void receiveDamage(int nbDamage);
	void displayStatus() const;
	bool alive() const;

	private:
	//attibuts
	int m_life;
	std::string m_name;
	C_Weapon m_weapon;
};

#endif
