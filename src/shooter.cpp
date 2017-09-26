#include "shooter.h"

using namespace std;

C_Shooter::C_Shooter():C_GameUnits("SHOOTER",1)
{
	m_weapon = new C_Weapon();
	SDL_Texture* m_image = nullptr;
}


C_Shooter::C_Shooter(std::string name, int rank):
	C_GameUnits(name, rank)
{
	m_weapon = new C_Weapon;
	SDL_Texture* m_image = nullptr;
}


C_Shooter::~C_Shooter()
{
	delete m_weapon;
}

void C_Shooter::shoot(C_Shooter &target)
{
	target.receiveDamage(m_weapon->getDamage());
	cout << target.getName() << " has been shot" << endl;
}

void C_Shooter::displayStatus() const
{
	C_GameUnits::displayStatus();
 	m_weapon->displayStatus();
}

void C_Shooter::printOnScreen(SDL_Renderer *renderer){

	renderTexture(m_image, renderer, m_x, m_y);

}

SDL_Texture* C_Shooter::getImage() const
{
	return m_image;
}

SDL_Texture* C_Shooter::updateImage(SDL_Texture *image)
{
	return m_image;
}
