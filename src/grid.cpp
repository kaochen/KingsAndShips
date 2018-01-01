#include "grid.h"
#include "surfaces.h"
#include "invaders.h"
#include "towers.h"
#include "level.h"
#include "menu.h"

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

	C_Level l;
	l.extractTMXfile("data/levels/Level_00.tmx");

	//calcultate Path from 3:15 to 27:15
	//C_Path p(27,15);
	//p.calcPath(3,15,27,15);
	//p.showPath();
	cout << "Level "<< levelNbr <<" Loaded" << endl;
}

void C_Grid::renderLayer(int layer){
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
						t.renderTextureFromId(m_grid[x][y].ground,  x_s,y_s + 36);
						}
						//draw the deads
				if (layer == DEAD){
						if (m_grid[x][y].dead != nullptr){
							int x_s = m_grid[x][y].dead->getXScreen();
							int y_s = m_grid[x][y].dead->getYScreen();
							t.renderTexture("boat_01_Dead.png", x_s,y_s + 36);
							}
						}
				if (layer == UNITS){
						if (m_grid[x][y].main != nullptr){
								S_Coord screen;
								screen.x = m_grid[x][y].main->getXScreen();
								screen.y = m_grid[x][y].main->getYScreen();
								m_grid[x][y].main->render(screen);
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

void C_Grid::addANewTower(int type, int x, int y, int rank){
	if (m_grid[x][y].main == nullptr && m_grid[x][y].water == false){
		switch(type){
		 case ADDNEWTOWER :
			m_grid[x][y].main = new C_ArcherTower(x,y,rank);
			m_grid[x][y].ground = 25;
		 break;
		 case ADDNEWTURBINE :
			m_grid[x][y].main = new C_Turbine(x,y,rank);
			m_grid[x][y].ground = 25;
		 break;
		 case  NONE:
		 break;
		 }
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

void C_Grid::setGround(int x, int y, int id){
	m_grid[x][y].ground = id;
	if (id >= 49 && id <= 57){
		m_grid[x][y].water = true;
		}
}

bool C_Grid::waterway(int x_grid, int y_grid){
	return m_grid[x_grid][y_grid].water;
}

int C_Grid::getGround(int x, int y){
	return m_grid[x][y].ground;
}

bool C_Grid::isThisConstructible(int x, int y){
	if ( m_grid[x][y].water == true){
		return false;
		}
	else if(m_grid[x][y].main != nullptr){
		return false;
	}
	else{
		return true ;
		}
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


int C_Grid::xGridToXScreen(int xGrid, int yGrid){
			C_Set& settings=C_Set::Instances();
			int x = settings.getWindowWidth()/2 + (xGrid - yGrid)* TILE_HALF_WIDTH;
			return x;
}
int C_Grid::yGridToYScreen(int xGrid, int yGrid){
			C_Set& settings=C_Set::Instances();
			int y = (yGrid + xGrid - 4) * TILE_HALF_HEIGHT - settings.getWindowHeight()/2;
			return y;
}


int C_Grid::xScreenToXGrid(int x_screen, int y_screen){
		C_Set& settings=C_Set::Instances();
		float xOffset = (settings.getWindowWidth() /2);
		float yOffset = (settings.getWindowHeight() /2);
		float tempX = 0.0;
		tempX = ( ((x_screen - xOffset ) / TILE_HALF_WIDTH + (y_screen + yOffset)/TILE_HALF_HEIGHT )/2);
		return tempX;
		}

int C_Grid::yScreenToYGrid(int x_screen, int y_screen){
		C_Set& settings=C_Set::Instances();
		float xOffset = (settings.getWindowWidth() /2);
		float yOffset = (settings.getWindowHeight() /2);
		float tempY = 0.0;
		tempY = ( (y_screen + yOffset )/(TILE_HALF_HEIGHT*2) - (x_screen - xOffset)/(TILE_HALF_WIDTH*2));
		return tempY;
		}

bool C_Grid::selectATower(int x_screen, int y_screen){
	int x_grid = xScreenToXGrid(x_screen, y_screen);
	int y_grid = yScreenToYGrid(x_screen, y_screen);
	bool selected = false;
	string name ="";
	if (m_grid[x_grid+1][y_grid+1].main != nullptr){
		unselectedAll(x_grid+1,y_grid+1);
		name = m_grid[x_grid+1][y_grid+1].main->getName();
			if(name == "ArcherTower" || name == "Turbine"){
				cout << "Found top " << name;
				m_grid[x_grid+1][y_grid+1].main->reverseSelectedStatus();
				selected = true;
			}
			else{
				cout << "Found " << name;
			}
	}
	else{
		if (m_grid[x_grid][y_grid].main != nullptr){
			name = m_grid[x_grid][y_grid].main->getName();
				unselectedAll(x_grid,y_grid);
				if(name == "ArcherTower"|| name == "Turbine"){
					cout << "Found bottom" << name;
					m_grid[x_grid][y_grid].main->reverseSelectedStatus();
					selected = true;
				}
				else{
					cout << "Found " << name;
				}
		}
		else{
			unselectedAll(x_grid,y_grid);
			cout << "Nothing";
		}

	}
	cout << " at " << x_grid << ":" << y_grid << endl;
	return selected;
}

void C_Grid::unselectedAll(int x_grid, int y_grid){
	bool status = false;
	//backup status
	if ( m_grid[x_grid][y_grid].main != nullptr){
		status = m_grid[x_grid][y_grid].main->getSelectedStatus();
		}
		//erase all
		for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
				if ( m_grid[x][y].main != nullptr)
					m_grid[x][y].main->setSelectedStatus(false);
			}
		}
	//restore status
	if ( m_grid[x_grid][y_grid].main != nullptr){
		m_grid[x_grid][y_grid].main->setSelectedStatus(status);
		}


}


C_GameUnits* C_Grid::getSelectedUnit(){
		C_GameUnits* current = nullptr;
		for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
				if ( m_grid[x][y].main != nullptr)
					if (m_grid[x][y].main->getSelectedStatus())
						current = m_grid[x][y].main->getUnit();
			}
		}
		return current;
}


