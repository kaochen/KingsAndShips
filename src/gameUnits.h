#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>



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
	virtual void render(int           x_iso,
	                   int           y_iso,
	                   SDL_Renderer *renderer);
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
