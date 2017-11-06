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
	C_Shooter(std::string name, int x_grid, int y_grid, int rank);
	virtual ~C_Shooter();
	virtual	void displayStatus() const;
	virtual void shoot(C_GameUnits &target);
	virtual SDL_Texture* getImage() const;
	virtual void move(int direction,
		      C_GameUnits::S_layer grid[][TABLE_SIZE]);
	virtual int testFirerange(C_GameUnits &target);

	virtual void render(int x_screen, int y_screen, SDL_Renderer *renderer);
	virtual void shootTarget(C_GameUnits &target);
	virtual void stopShooting(void);
	virtual void renderMissile(SDL_Renderer *renderer);
	protected:
	C_Weapon *m_weapon;
	SDL_Texture* m_image;
	long m_lastShootTime;
};

#endif
