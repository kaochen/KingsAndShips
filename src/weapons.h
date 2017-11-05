#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <SDL2/SDL.h>
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

	int getXScreen() const;
	int getYScreen() const;
	bool getShooting() const;
	void setShooting(bool status);
	void setMissile(int x_s_target,int y_s_target, int x_s_shooter, int y_s_shooter);

	private:
	std::string m_name;
	int m_damage;
	int m_fireRate;
	int m_fireRange;
	int m_x_screen;
	int m_y_screen;
	bool m_shooting;
	double m_lastShootTime;
};


#endif
