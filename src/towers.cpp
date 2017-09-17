#include "towers.h"

using namespace std;

C_Towers::C_Towers():
	m_level(1),
	m_name("TOWER"),
	m_weapon("GUN",10,3)
{}

C_Towers::C_Towers(std::string name, std::string weaponName, int weaponDamage, int weaponFireRate):
	m_level(1),
	m_name(name),
	m_weapon(weaponName, weaponDamage, weaponFireRate)
{}

C_Towers::~C_Towers()
{}

void C_Towers::shoot(C_invaders &target)
{
	target.receiveDamage(m_weapon.getDamage());
}

void C_Towers::displayStatus() const
{
 	cout << "Towers: " << m_name << " level: "<< m_level;
 	m_weapon.displayStatus();
}

int C_Towers::getLevel() const
{
	return m_level;
}


