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
	int newX_grid = m_x_grid;
	int newY_grid = m_y_grid;
	int speed = 10;
	bool changeCase = false;
	cout << "In offset " << m_x_screen_offset << ":" << m_y_screen_offset << endl;
	if (direction == NORTH){
		m_y_screen_offset -= speed/2;
		m_x_screen_offset += speed;
			if (m_y_screen_offset < 0){
				m_x_screen_offset += TILE_HALF_WIDTH;
				m_y_screen_offset += TILE_HALF_HEIGHT;
				changeCase = true;
				}
	}
	if (direction == SOUTH){
		m_y_screen_offset += speed/2;
		m_x_screen_offset -= speed;
			if (m_y_screen_offset < TILE_HALF_HEIGHT){
				m_x_screen_offset -= TILE_HALF_WIDTH;
				m_y_screen_offset -= TILE_HALF_HEIGHT;
				changeCase = true;
				}
		}
	if (direction == EAST){
		m_x_screen_offset += speed;
		m_y_screen_offset += speed/2;
			if (m_x_screen_offset > TILE_HALF_WIDTH){
				m_x_screen_offset -= TILE_HALF_WIDTH;
				m_y_screen_offset -= TILE_HALF_HEIGHT;
				changeCase = true;
				}
		}
	if (direction == WEST){
		m_x_screen_offset -= speed;
		m_y_screen_offset -= speed/2;
			if (m_x_screen_offset < 0){
				m_x_screen_offset += TILE_HALF_WIDTH;
				m_y_screen_offset += TILE_HALF_HEIGHT;
				changeCase = true;
				}
		}

	cout << "Out offset " << m_x_screen_offset << ":" << m_y_screen_offset << endl;
	if (changeCase == true){
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
