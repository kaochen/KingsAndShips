#include "invaders.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank,
			 C_GameUnits* grid_units[][TABLE_SIZE]):C_Shooter("boat", x_grid, y_grid ,rank, grid_units)
{
}

C_invaders::~C_invaders()
{
}


void C_invaders::move(int direction,
		      C_GameUnits* grid_units[][TABLE_SIZE])
{
	int newX_grid = m_x_grid;
	int newY_grid = m_y_grid;
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
	grid_units[newX_grid][newY_grid] = grid_units[m_x_grid][m_y_grid];
	// update status
	grid_units[m_x_grid][m_y_grid] = nullptr;
	m_x_grid = newX_grid;
	m_y_grid = newY_grid;
}

