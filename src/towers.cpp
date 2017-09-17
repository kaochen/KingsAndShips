#include "towers.h"

using namespace std;

C_Towers::C_Towers():
	m_level(1),
	m_name("TOWER")
{

	m_weapon = new C_Weapon();
	cout << "add a new default Tower" << endl;
}

C_Towers::C_Towers(std::string name, std::string weaponName, int weaponDamage, int weaponFireRate):
	m_level(1),
	m_name(name)
{
	m_weapon = new C_Weapon(weaponName, weaponDamage, weaponFireRate);

	cout << "add new Tower named " << weaponName << "Damage: "<< weaponDamage
		<< "Fire Rate:"<< weaponFireRate << endl;
}

C_Towers::~C_Towers()
{
	delete m_weapon;
}

/* make a copy of the C_Towers and the C_Weapon.
This avoid the copy to share the same weapon with the original */

C_Towers::C_Towers(C_Towers const& original)
	:m_level(original.m_level),m_name(original.m_name)
{
	m_weapon = new C_Weapon(*(original.m_weapon));
}


C_Towers& C_Towers::operator=(C_Towers const& original)
{
	if(this != &original)
	{
		m_level=original.m_level;
		m_name=original.m_name;
		delete m_weapon;
		m_weapon = new C_Weapon(*(original.m_weapon));
	}
	return *this;
}


void C_Towers::shoot(C_invaders &target)
{
	target.receiveDamage(m_weapon->getDamage());
}

void C_Towers::displayStatus() const
{
 	cout << "Towers: " << m_name << " level: "<< m_level;
 	m_weapon->displayStatus();
}

int C_Towers::getLevel() const
{
	return m_level;
}

string C_Towers::getName() const
{
	return m_name;
}

bool operator==(C_Towers const& a, C_Towers const& b)
{
	return a.isEqualTo(b);
}

bool C_Towers::isEqualTo(C_Towers const& b) const
{
	return(m_name == b.m_name);
}


bool operator!=(C_Towers const& a, C_Towers const& b)
{
	return!(a==b);
}
