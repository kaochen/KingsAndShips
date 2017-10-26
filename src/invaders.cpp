#include "invaders.h"

using namespace std;

C_invaders::C_invaders():C_Shooter("boat",1)
{
}


C_invaders::C_invaders(int rank):C_Shooter("boat", rank)
{
}

C_invaders::~C_invaders()
{
}


void C_invaders::displayStatus() const
{
	C_Shooter::displayStatus();
 	cout << "\tx:"<< m_x << " y:"<< m_y << endl;
}

void C_invaders::move(int x_grid,
		      int y_grid,
		      int direction,
		      C_GameUnits* grid_units[][TABLE_SIZE])
{
	int newX_grid = x_grid;
	int newY_grid = y_grid;
	switch (direction){
	case NORTH:
		newY_grid--;
	break;
	case SOUTH:
		newY_grid++;
	break;
	case EAST:
		newX_grid++;
	break;
	case WEST:
		newY_grid--;
	break;
	}
	grid_units[newX_grid][newY_grid] = grid_units[x_grid][y_grid];
	grid_units[x_grid][y_grid] = nullptr;
}

