#include "weapons.h"
#include <cmath>

using namespace std;

C_Weapon::C_Weapon():m_name("CANON"),
		m_damage(10),
		m_fireRate(2000),
		m_fireRange(200),
		m_x_screen(0),
		m_y_screen(0),
		m_shooting(false)
{
}

C_Weapon::C_Weapon(std::string name, int damage, int fireRate, int fireRange):
	m_name(name),
	m_damage(damage),
	m_fireRate(fireRate),
	m_fireRange(fireRange),
	m_dist(80)
{
	m_lastShootTime = 0;
}

C_Weapon::~C_Weapon()
{
}

void C_Weapon::change(string name, int damage, int fireRate, int fireRange)
{
 	m_name = name;
 	m_damage = damage;
 	m_fireRate = fireRate;
 	m_fireRange = fireRange;
}

void C_Weapon::displayStatus() const
{
 	cout << "\tWeapon: " << m_name << " (Damage: "<< m_damage << ", firerate: " << m_fireRate << ")" << endl;
}

int C_Weapon::getDamage() const
{
	return m_damage;
}

int C_Weapon::getFireRate() const
{
	return m_fireRate;
}

int C_Weapon::getFireRange() const
{
	return m_fireRange;
}

int C_Weapon::getXScreen() const
{
	return m_x_screen;
}

int C_Weapon::getYScreen() const
{
	return m_y_screen;
}

bool C_Weapon::getShooting() const
{
	return m_shooting;
}

void C_Weapon::setShooting(bool status)
{
	m_shooting = status;
}

void C_Weapon::setMissile(C_GameUnits &shooter, C_GameUnits &target){
			int x_s_target = target.getXScreen();
			int y_s_target = target.getYScreen() + 100;
			int x_s_shooter = shooter.getXScreen();
			int y_s_shooter = shooter.getYScreen() + 100;
			int ab = x_s_target - x_s_shooter;
			int bc = y_s_target - y_s_shooter;
			int hyp = sqrt((ab*ab + bc*bc));
			hyp -= hyp*m_dist/100;
			double angle = atan2(ab,bc);
			int newA = hyp*sin(angle);
			int newB = hyp*cos(angle);
			cout << "ab:" << ab << " b:" << bc << " hyp:"<< hyp << " a:"<< angle << endl;
			m_x_screen = x_s_shooter + newA;
			m_y_screen = y_s_shooter + newB;
			m_dist -= 5;
			if (m_dist < 20)
				m_dist = 80;
}
