#include "grid.h"

using namespace std;

C_Grid C_Grid::m_instance=C_Grid();

C_Grid::C_Grid()
{
	cout << "Construct C_Grid" << endl;
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		m_grid[x][y].main = nullptr;
		m_grid[x][y].dead = nullptr;
		m_grid[x][y].plot = true;
		m_grid[x][y].water = false;
		m_grid[x][y].ground = GROUND_01;
		}
	}

}

C_Grid::~C_Grid()
{
}

C_Grid& C_Grid::Instances()
{
	return m_instance;
}
