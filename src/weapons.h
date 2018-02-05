#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "gameUnits.h"


class C_Weapon
{
public:
	C_Weapon();
	C_Weapon(std::string name, int damage,int speedImpact, int fireRate, int fireRange);
	~C_Weapon();

	void change(std::string name, int damage, int fireRate, int fireRange);
	void displayStatus() const;
	int getDamage() const;
	int getSpeedImpact() const;
	int getFireRate() const;
	int getFireRange() const;

	int getXScreen() const;
	int getYScreen() const;
	bool getShooting() const;
	long getLastShootTime() const;
	void setShooting(bool status);
	bool shoot(C_GameUnits &shooter, C_GameUnits &target);
	int getDirection() const;
	S_Weapon getWeaponInfo() const;
	void render();

	private:
	std::string m_name;
	S_Weapon m_weapon;
	int m_x_screen;
	int m_y_screen;
	bool m_shooting;
	long m_lastShootTime;
	int m_dist;
	int m_direction;
	double m_angle;
};


#endif
