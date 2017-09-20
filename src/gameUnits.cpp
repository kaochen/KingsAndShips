#include "gameUnits.h"

using namespace std;


//contrusctor
C_GameUnits::C_GameUnits():
	m_name("UNIT"),
	m_life(100),
	m_rank(1)
{}

C_GameUnits::C_GameUnits(string name, int rank):m_name(name),m_life(100),m_rank(rank)
{}

//delete
C_GameUnits::~C_GameUnits()
{}

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


int C_GameUnits::getRank() const
{
	return m_rank;
}
