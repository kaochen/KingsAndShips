#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <string>

class C_GameUnits
{
	public:
	//methods
	C_GameUnits();
	C_GameUnits(std::string name, int rank);
	C_GameUnits(C_GameUnits const& original);
	virtual ~C_GameUnits();

	std::string getName() const;
	virtual void displayStatus() const;
	int getRank() const;

	protected:
	//attibuts
	std::string m_name;
	int m_life;
	int m_rank;
};

#endif
