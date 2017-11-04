#include "shooter.h"

using namespace std;


C_Shooter::C_Shooter(std::string name, int x_grid, int y_grid, int rank, C_GameUnits* grid_units[][TABLE_SIZE]):
	C_GameUnits(name, x_grid, y_grid, rank, grid_units)
{
	m_weapon = new C_Weapon;
	m_y_center_offset = 36;
	m_lastShootTime = SDL_GetTicks();
}

C_Shooter::~C_Shooter()
{
	delete m_weapon;
}

void C_Shooter::shoot(C_GameUnits &target)
{
	double currentTime = SDL_GetTicks();
	if ((currentTime - m_lastShootTime) > m_weapon->getFireRate()){
		target.receiveDamage(m_weapon->getDamage());
		m_lastShootTime = currentTime;
		cout << target.getName() << " has been shot" << endl;
	}
}

void C_Shooter::displayStatus() const
{
	C_GameUnits::displayStatus();
 	m_weapon->displayStatus();
}


SDL_Texture* C_Shooter::getImage() const
{
	return m_image;
}

SDL_Texture* C_Shooter::updateImage(SDL_Texture *image)
{
	return m_image;
}

void C_Shooter::move(int direction,
			C_GameUnits* grid_units[][TABLE_SIZE])
{
}

int C_Shooter::testFirerange(int x, int y)
{
	int dist = getDistance(x,y);
	if (dist > m_weapon->getFireRange())
		return -1;
	else
		return dist;
}
