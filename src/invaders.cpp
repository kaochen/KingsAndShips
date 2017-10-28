#include "invaders.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank,
			 C_GameUnits* grid_units[][TABLE_SIZE]):C_Shooter("boat", x_grid, y_grid ,rank, grid_units)
{
	m_x_screen_offset = TILE_HALF_WIDTH;
	m_y_screen_offset = TILE_HALF_HEIGHT;
}

C_invaders::~C_invaders()
{
}


void C_invaders::move(int direction,
		      C_GameUnits* grid_units[][TABLE_SIZE])
{
	int speed = 2;
	switch (direction){
		case EAST:
			m_x_screen += speed;
			m_y_screen += speed/2;
		break;
		case WEST:
			m_x_screen -= speed;
			m_y_screen -= speed/2;
		break;
		case NORTH:
			m_x_screen += speed;
			m_y_screen -= speed/2;
		break;
		case SOUTH:
			m_x_screen -= speed;
			m_y_screen += speed/2;
		break;
	}

	xyScreenToXYGrid();
}


int C_invaders::getXoffset()
{
	return m_x_screen_offset;
}

int C_invaders::getYoffset()
{
	return m_y_screen_offset;
}

void C_invaders::render(int x_iso, int y_iso, SDL_Renderer *renderer){
	string name = getName();
	int rank = getRank();
	string fileName = name + "_0" + to_string(rank) + "_00.png" ;
	//cout << "image name is "<< fileName << endl;
	x_iso = x_iso + m_x_screen_offset - TILE_HALF_WIDTH/2;
	y_iso = y_iso + m_y_screen_offset - TILE_HALF_HEIGHT/2;
	//renderTexture(C_Texture::getText("SimpleWaterTile.png"), renderer, x_iso,y_iso);
	renderTexture(C_Texture::getText(fileName), renderer, x_iso,y_iso);
}
