#include "gameUnits.h"

using namespace std;


//contrusctor
C_GameUnits::C_GameUnits():
	m_name("UNIT"),
	m_life(100),
	m_rank(1),
	m_x(0),
	m_y(0)
{
	cout << "Add new default unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
}

C_GameUnits::C_GameUnits(string name, int rank):
	m_name(name),
	m_life(100),
	m_rank(rank),
	m_x(0),
	m_y(0)
{
	cout << "Add new unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
}

//delete
C_GameUnits::~C_GameUnits()
{
}

//copy
C_GameUnits::C_GameUnits(C_GameUnits const& original):
	m_name(original.m_name),
	m_life(original.m_life),
	m_rank(original.m_rank)
{}

//get attibuts
string C_GameUnits::getName() const
{
	return m_name;
}


void C_GameUnits::displayStatus() const
{
 	cout << "Name: " << m_name << " Life: " << m_life  << " Rank : " << m_rank<< endl;
}


void C_GameUnits::receiveDamage(int nbDamage)
{
	m_life -=nbDamage;
	if (m_life < 0)
	{
		m_life = 0;
	}
}

bool C_GameUnits::alive() const
{
	return m_life>0;
}


int C_GameUnits::getRank() const
{
	return m_rank;
}
