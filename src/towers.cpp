#include "towers.h"

using namespace std;

C_Towers::C_Towers():C_GameUnits("TOWER",1)
{
	m_weapon = new C_Weapon();
	cout << "add a new default Tower" << endl;
}

C_Towers::C_Towers(std::string name, int rank, std::string weaponName, int weaponDamage, int weaponFireRate):
	C_GameUnits(name, rank)
{
	m_weapon = new C_Weapon(weaponName, weaponDamage, weaponFireRate);

	cout << "add new Tower named " << weaponName << " Damage: "<< weaponDamage
		<< " Fire Rate:"<< weaponFireRate << endl;
}

C_Towers::~C_Towers()
{
	delete m_weapon;
}

/* make a copy of the C_Towers and the C_Weapon.
This avoid the copy to share the same weapon with the original */

C_Towers::C_Towers(C_Towers const& original)
{
//	m_name = original.m_name;
//	m_rank = original.m_rank;
//	m_life = original.m_life;
	m_weapon = new C_Weapon(*(original.m_weapon));
}


C_Towers& C_Towers::operator=(C_Towers const& original)
{
	if(this != &original)
	{
		m_name=original.m_name;
		m_rank=original.m_rank;
		m_life = original.m_life;
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
	C_GameUnits::displayStatus();
 	m_weapon->displayStatus();
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
