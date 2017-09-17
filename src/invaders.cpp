#include "invaders.h"

using namespace std;

C_invaders::C_invaders():
	m_life(100),
	m_name("SOLDIER"),
	m_weapon("GUN",10,3)
{}

C_invaders::C_invaders(std::string name, std::string weaponName, int weaponDamage, int weaponFireRate):
	m_life(100),
	m_name(name),
	m_weapon(weaponName, weaponDamage, weaponFireRate)
{}

C_invaders::~C_invaders()
{
}

void C_invaders::receiveDamage(int nbDamage)
{
	m_life -=nbDamage;
	if (m_life < 0)
	{
		m_life = 0;
	}
}

void C_invaders::displayStatus() const
{
 	cout << "Invaders: " << m_name << " life: "<< m_life;
 	m_weapon.displayStatus();
}


bool C_invaders::alive() const
{
	return m_life>0;
}
	
