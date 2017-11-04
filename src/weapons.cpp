#include "weapons.h"

using namespace std;

C_Weapon::C_Weapon():m_name("CANON"),m_damage(10),m_fireRate(2000),m_fireRange(200)
{
}

C_Weapon::C_Weapon(std::string name, int damage, int fireRate, int fireRange):
	m_name(name),
	m_damage(damage),
	m_fireRate(fireRate),
	m_fireRange(fireRange)
{
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

