#include "grid.h"
#include "surfaces.h"
#include "invaders.h"
#include "towers.h"

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

void C_Grid::renderLayer(int layer, SDL_Renderer *renderer){
	C_Set& settings=C_Set::Instances();
	C_TextureList& t=C_TextureList::Instances();
	int x_start = settings.getGridFirstTileX(), y_start = settings.getGridFirstTileY();
	int length = 4;
	int y = y_start;

	for (int l = 0; l < settings.getGridNbrOfLine(); l++){
		int x = x_start;
		//cout << "\nline: "<< l << " --";
		for (int r = 0 ; r < length; r++){

				//cout << "|" << x << ":"<< y;
				if (layer == GROUND){
						int x_s = settings.getWindowWidth()/2 + (x - y)* TILE_HALF_WIDTH;
						int y_s = (y + x- 4) * TILE_HALF_HEIGHT - settings.getWindowHeight()/2;
						if (m_grid[x][y].water)
								t.renderTexture("SimpleWaterTile.png", renderer, x_s,y_s + 36);
						if (m_grid[x][y].ground == GROUND_01 && m_grid[x][y].water == false)
								t.renderTexture("Grass_01.png", renderer, x_s,y_s + 36);
						if (m_grid[x][y].ground == GROUND_02 && m_grid[x][y].water == false)
								t.renderTexture("Grass_02.png", renderer, x_s,y_s + 36);
						    }
						//draw the deads
				if (layer == DEAD){
						if (m_grid[x][y].dead != nullptr){
							int x_s = m_grid[x][y].dead->getXScreen();
							int y_s = m_grid[x][y].dead->getYScreen();
							t.renderTexture("boat_01_Dead.png", renderer, x_s,y_s + 36);
							}
						}
				if (layer == UNITS){
						if (m_grid[x][y].main != nullptr){
								int x_screen = m_grid[x][y].main->getXScreen();
								int y_screen = m_grid[x][y].main->getYScreen();
								m_grid[x][y].main->render(x_screen, y_screen, renderer);
							}
						}
				x++;
				}
		y++;
		if(l < 13){
			length += 2;x_start--;
			}
		else{
		  	length -=2; x_start++;
		  	}

	}
}


void C_Grid::addANewBoat(int x, int y, int rank){
	if (m_grid[x][y].water){
		m_grid[x][y].main = new C_invaders(x,y,rank);
	}
	else{
		cout << "You should place the boat into the water" << endl;
	}
}

void C_Grid::addANewTower(int x, int y, int rank){
	if (m_grid[x][y].main == nullptr){
		m_grid[x][y].main = new C_Towers(x,y,rank);
		m_grid[x][y].ground = GROUND_01;
	}
}

void C_Grid::moveUnit(int x_from, int y_from, int x_dest, int y_dest){
if(x_from == x_dest && y_from == y_dest){
	}
else{
	if (m_grid[x_from][y_from].main != nullptr){
		m_grid[x_dest][y_dest].main = m_grid[x_from][y_from].main;
		m_grid[x_from][y_from].main = nullptr;
	}
    }
}

C_GameUnits* C_Grid::getUnits(int x, int y){
	return m_grid[x][y].main;
}

void C_Grid::delUnit(int x_grid, int y_grid){
	m_grid[x_grid][y_grid].main = nullptr;
	}

void C_Grid::moveToDead(int x_grid, int y_grid){
	m_grid[x_grid][y_grid].dead = m_grid[x_grid][y_grid].main;
	m_grid[x_grid][y_grid].main = nullptr;
	}



void C_Grid::displayStatus(){
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
			if (m_grid[x][y].main != nullptr){
				cout <<"Case: " << x <<":" << y << endl;
				m_grid[x][y].main->displayStatus();
				}
			}
		}
}


void C_Grid::deleteGrid(){
	for (size_t y = 0; y < GRID_SIZE; y++){
			for (size_t x = 0; x < GRID_SIZE; x++){
				if (m_grid[x][y].main != nullptr){
					delete m_grid[x][y].main;
					m_grid[x][y].main = nullptr;
					}
				 if (m_grid[x][y].dead != nullptr){
					delete m_grid[x][y].dead;
					m_grid[x][y].dead = nullptr;
					}
			}
		}
}
