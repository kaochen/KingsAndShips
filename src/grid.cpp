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


void C_Grid::loadLevel(int levelNbr){
		cout << "Level Number: " << levelNbr;
	//setup a basic level for now
		m_grid[16][17].ground = GROUND_02;
		m_grid[20][21].ground = GROUND_02;
		m_grid[10][15].ground = GROUND_02;
		m_grid[9][6].ground = GROUND_02;

	// add some water
	for (size_t x = 0; x < GRID_SIZE; x++){
		m_grid[x][12].plot = false;
		m_grid[x][12].water = true;
		m_grid[x][14].plot = false;
		m_grid[x][14].water = true;
		}
}

C_Grid& C_Grid::Instances()
{
	return m_instance;
}
