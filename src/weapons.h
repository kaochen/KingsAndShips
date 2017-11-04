#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <string>


class C_Weapon
{
public:
	C_Weapon();
	C_Weapon(std::string name, int damage, int fireRate, int fireRange);
	~C_Weapon();

	void change(std::string name, int damage, int fireRate, int fireRange);
	void displayStatus() const;
	int getDamage() const;
	int getFireRate() const;
	int getFireRange() const;

private:
	std::string m_name;
	int m_damage;
	int m_fireRate;
	int m_fireRange;
};


#endif
