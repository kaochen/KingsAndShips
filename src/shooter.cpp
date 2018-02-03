#include "shooter.h"
#include "grid.h"

using namespace std;


C_Shooter::C_Shooter(std::string name, int x_grid, int y_grid, int rank):
	C_GameUnits(name, x_grid, y_grid, rank)
{
	m_weapon = nullptr;
	m_y_center_offset = 30;
	m_lastShootTime = 0;
}

C_Shooter::~C_Shooter()
{
	delete m_weapon;
}
C_GameUnits*  C_Shooter::searchNextTarget(string type){
	int gridDiag = m_weapon->getFireRange();
	C_Grid& grid=C_Grid::Instances();
	int x_grid = m_coord->getXGrid();
	int y_grid = m_coord->getYGrid();
	C_GameUnits* target = nullptr;

	map<int, C_GameUnits*> list;
	for(int y = (y_grid - gridDiag); y <= (y_grid + gridDiag); y++){
		for(int x = (x_grid - gridDiag); x <= (x_grid + gridDiag); x++){
			if((x != x_grid || y != y_grid)){
				C_GameUnits* tmp = grid.getUnits(x,y);
				if(tmp != nullptr){
					if(tmp->getName() == type){
					 	int dist = getDistance(x,y);
					 	list[dist] = tmp;
				 	}
				}
			}
		}
	}
	if(!list.empty()){
		target = list.begin()->second;
	}

	return target;
}
void C_Shooter::shoot()
{
	C_GameUnits* target = searchNextTarget("boat");
	if(target != nullptr){
		long currentTime = SDL_GetTicks();
		if ((currentTime ) > m_weapon->getLastShootTime() + m_weapon->getFireRate()){
			m_weapon->setShooting(true);
			shootTarget(*target);
			//cout << target.getName() << " has been shot" << endl;
		}
		else {
			m_weapon->setShooting(false);
			}
		}
	else {
		m_weapon->setShooting(false);
		}
}

void C_Shooter::displayStatus() const
{
	C_GameUnits::displayStatus();
	if(m_weapon != nullptr)
 		m_weapon->displayStatus();
}


SDL_Texture* C_Shooter::getImage() const
{
	return m_image;
}


void C_Shooter::move()
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
			target.receiveDamage(m_weapon->getWeaponInfo());
		}
	}
}
void C_Shooter::stopShooting(void){
		m_weapon->setShooting(false);
		}


void C_Shooter::render(S_Coord screen){
	C_GameUnits::render(screen);
	if (m_weapon->getShooting())
		renderMissile();
}

void C_Shooter::renderMissile(){
		int x_s = m_weapon->getXScreen();
		int y_s = m_weapon->getYScreen();
		int direction = m_weapon->getDirection();
		string name;
		switch(direction){
		case NORTH:
			name = "Arrow01_North.png";
			break;
		case EAST:
			name = "Arrow01_East.png";
			break;
		case SOUTH:
			name = "Arrow01_South.png";
			break;
		case WEST:
			name = "Arrow01_West.png";
			break;
		case NORTH_EAST:
			name = "Arrow01_NorthEast.png";
			break;
		case NORTH_WEST:
			name = "Arrow01_NorthWest.png";
			break;
		case SOUTH_EAST:
			name = "Arrow01_SouthEast.png";
			break;
		case SOUTH_WEST:
			name = "Arrow01_SouthWest.png";
			break;
		case UNKNOWN:
			name = "Arrow01_North.png";
			break;
		}

		C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(name, x_s,y_s);
}
