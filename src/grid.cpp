#include "grid.h"
#include "surfaces.h"

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

void C_Grid::renderFloor(SDL_Renderer *renderer){

		C_Set& settings=C_Set::Instances();
		//add a setup background
		renderTexture(C_Texture::getText("SetupBackground.png"), renderer, (settings.getWindowWidth()/2),0);
		//draw some water



int x_start = settings.getGridFirstTileX(), y_start = settings.getGridFirstTileY();
	for (int l = 0; l < settings.getGridNbrOfLine(); l++){
		int y = y_start;
		int x = x_start;
		//cout << "\nline: "<< l << " --";
		for (int r = 0 ; r < settings.getGridNbrOfRow(); r++){
				//cout << "|" << x << ":"<< y;
				int x_s = settings.getWindowWidth()/2 + (x - y)* TILE_HALF_WIDTH;
				int y_s = (y + x- 4) * TILE_HALF_HEIGHT - settings.getWindowHeight()/2;
				if (m_grid[x][y].water)
						renderTexture(C_Texture::getText("SimpleWaterTile.png"), renderer, x_s,y_s + 36);
				if (m_grid[x][y].ground == GROUND_01 && m_grid[x][y].water == false)
						renderTexture(C_Texture::getText("Grass_01.png"), renderer, x_s,y_s + 36);
				if (m_grid[x][y].ground == GROUND_02 && m_grid[x][y].water == false)
						renderTexture(C_Texture::getText("Grass_02.png"), renderer, x_s,y_s + 36);

				y++;
				x++;
		}
		if (l %2 == 0)
			y_start--;
		else
			x_start++;
	}
}
