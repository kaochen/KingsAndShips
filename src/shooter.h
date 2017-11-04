#ifndef SHOOTER_H
#define SHOOTER_H

#include <iostream>
#include <string>
#include "gameUnits.h"
#include "weapons.h"
#include "surfaces.h"

class C_Shooter : public C_GameUnits
{
	public:
	C_Shooter(std::string name, int x_grid, int y_grid, int rank, C_GameUnits* grid_units[][TABLE_SIZE][LAYER]);
	virtual ~C_Shooter();
	virtual	void displayStatus() const;
	virtual void shoot(C_GameUnits &target);
	virtual SDL_Texture* getImage() const;
	virtual SDL_Texture* updateImage(SDL_Texture *image);
	virtual void move(int direction,
		      C_GameUnits* grid_units[][TABLE_SIZE][LAYER]);

	virtual int testFirerange(int x, int y);

	protected:
	C_Weapon *m_weapon;
	SDL_Texture* m_image;
	double m_lastShootTime;
};

#endif
