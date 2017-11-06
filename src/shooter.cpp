#include "shooter.h"

using namespace std;


C_Shooter::C_Shooter(std::string name, int x_grid, int y_grid, int rank, C_GameUnits::S_layer grid[][TABLE_SIZE]):
	C_GameUnits(name, x_grid, y_grid, rank, grid)
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
	m_weapon->setShooting(true);
//	long currentTime = SDL_GetTicks();
//	if ((currentTime - m_lastShootTime) > m_weapon->getFireRate()){
		shootTarget(target);
	//	m_lastShootTime = currentTime;
		cout << target.getName() << " has been shot" << endl;
//	}
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
                       C_GameUnits::S_layer grid[][TABLE_SIZE])
{
}

int C_Shooter::testFirerange(C_GameUnits &target)
{
	int dist = getDistance(target.getXScreen(),target.getYScreen());
	if (dist > m_weapon->getFireRange())
		return -1;
	else{
		return dist;
	    }
}

void C_Shooter::shootTarget(C_GameUnits &target){
	if(m_weapon->getShooting()){
		bool test = m_weapon->shoot(*this, target);
		if (test){
			target.receiveDamage(m_weapon->getDamage());
		}
	}
}
void C_Shooter::stopShooting(void){
		m_weapon->setShooting(false);
		}


void C_Shooter::render(int x_screen, int y_screen, SDL_Renderer *renderer){
	C_GameUnits::render(x_screen, y_screen,renderer);
	if (m_weapon->getShooting())
		renderMissile(renderer);
}

void C_Shooter::renderMissile(SDL_Renderer *renderer){
		SDL_Rect m;
		    m.x = m_weapon->getXScreen();
		    m.y = m_weapon->getYScreen();
		    m.w = 5;
		    m.h = 5;
		    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 128 );
		    SDL_RenderFillRect( renderer, &m);
}
