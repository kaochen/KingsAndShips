#include "town.h"

using namespace std;


C_Town::C_Town( int x_grid,
                 int y_grid):C_Shooter("town", x_grid, y_grid, 1)
{
	m_weapon = new C_Weapon("ARCHER",10,0,500,2);
}

void C_Town::play(){
	this->shoot("boat");
}

void C_Town::render(S_Coord screen){
	C_GameUnits::render(screen);
	renderLifeBar(screen.x, screen.y);
	if (m_weapon->getShooting()){
		m_weapon->render();
	}

}
