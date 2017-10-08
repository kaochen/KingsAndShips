#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <string>
#include "surfaces.h"


class C_GameUnits
{
	public:
	//methods
	C_GameUnits();
	C_GameUnits(std::string name, int rank);
	C_GameUnits(C_GameUnits const& original);
	virtual ~C_GameUnits();

	virtual std::string getName() const;
	virtual void displayStatus() const;
	virtual void shoot(C_GameUnits &target) =0;
	virtual void receiveDamage(int nbDamage);
	virtual bool alive() const;
	virtual int getRank() const;

	protected:
	//attibuts
	std::string m_name;
	int m_life;
	int m_rank;
	int m_x;
	int m_y;
};

#endif
