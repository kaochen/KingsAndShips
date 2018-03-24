#include "grid.h"
#include "surfaces.h"
#include "invaders.h"
#include "towers.h"
#include "level.h"
#include "menu.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

using namespace std;

C_Grid C_Grid::m_instance=C_Grid();

C_Grid::C_Grid()
{
	cout << "Construct C_Grid" << endl;
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		m_grid[x][y].main = nullptr;
		m_grid[x][y].dead = nullptr;
		m_grid[x][y].str_ground = "";
		m_grid[x][y].plot = true;
		m_grid[x][y].water = false;
		}
	}
}

C_Grid::~C_Grid()
{
}

void C_Grid::reset()
{
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		if(m_grid[x][y].main != nullptr){
		        delete m_grid[x][y].main;
		        m_grid[x][y].main = nullptr;
		}
		if(m_grid[x][y].dead != nullptr){
		        delete m_grid[x][y].dead;
		        m_grid[x][y].dead = nullptr;
		}
		m_grid[x][y].str_ground = "";
		m_grid[x][y].plot = true;
		m_grid[x][y].water = false;
		}
	}
}

C_Grid& C_Grid::Instances()
{
	return m_instance;
}


void C_Grid::renderLayer(int layer){
	C_Set& settings=C_Set::Instances();
	C_TextureList& t=C_TextureList::Instances();

	int x_start = 0, x_end = x_start + settings.getGridWidth() + 4;
	int y_start = 13, y_end = y_start + settings.getGridHeight() + 3;

	//cout << "Line ";
	for (int lineNbr = y_start; lineNbr < y_end; lineNbr++){
		int x = x_start;
		int y = y_start;
	for (int rowNbr = x_start; rowNbr < x_end; rowNbr++){
				//cout << "|" << x << ":"<< y << ":";
				if (layer == GROUND){
						C_CoordGrid coord(x,y);
						t.renderTexture(m_grid[x][y].str_ground,coord.getXScreen(),coord.getYScreen() + 36);
						if(!waterway(x,y)){
						    if ((x+y)%2 == 0){
                                darkenGround(coord.getXScreen(), coord.getYScreen());
						    }
						}
						}
						//draw the deads
				if (layer == DEAD){
						if (m_grid[x][y].dead != nullptr){
							int x_s = m_grid[x][y].dead->getXScreen();
							int y_s = m_grid[x][y].dead->getYScreen();
							t.renderTexture("boat_01_Dead.png", x_s,y_s);
							}
						}
				if (layer == UNITS){
						if (m_grid[x][y].main != nullptr){
								S_Coord screen;
								//cout << "render units " << endl;
								screen.x = m_grid[x][y].main->getXScreen();
								screen.y = m_grid[x][y].main->getYScreen();
								m_grid[x][y].main->render(screen);
							}
						}
				x++;
				y--;
				}
		if (lineNbr%2 == 0)
			x_start++;
		else
			y_start++;

		//cout << endl;
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
			m_grid[x][y].str_ground = "Ground_01_Grass00";
		 break;
		 case ADDNEWTURBINE :
			m_grid[x][y].main = new C_Turbine(x,y,rank);
			m_grid[x][y].str_ground = "Ground_01_Grass00";
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

C_GameUnits* C_Grid::getUnits(int x_grid, int y_grid){
	return m_grid[x_grid][y_grid].main;
}

void C_Grid::setGround(int x, int y, int id){
	C_TextureList& t=C_TextureList::Instances();
	string str = t.getNameFromID(id);
	m_grid[x][y].str_ground = str;
	//cout << str << endl;
	string pattern = "Water";
	size_t found = str.find(pattern);
	if(found != string::npos){
		m_grid[x][y].water = true;
	}
}

bool C_Grid::waterway(int x_grid, int y_grid){
	return m_grid[x_grid][y_grid].water;
}


string C_Grid::getStrGround(int x, int y){
	return m_grid[x][y].str_ground;
}
bool C_Grid::isThisConstructible(S_Coord grid){
	if ( m_grid[grid.x][grid.y].water == true){
		return false;
		}
	else if(m_grid[grid.x][grid.y].main != nullptr){
		return false;
	}
	else{
		return true ;
		}
}

bool C_Grid::isThisConstructible(int x_grid,int y_grid){
	S_Coord tmp;
	tmp.x = x_grid;
	tmp.y = y_grid;
	return isThisConstructible(tmp);
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

void C_Grid::playAllUnits(){
	for (size_t y = 0; y < GRID_SIZE; y++){
			for (size_t x = 0; x < GRID_SIZE; x++){
				if (m_grid[x][y].main != nullptr){
					m_grid[x][y].main->play();
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



bool C_Grid::selectATower(C_Coord clic){
	S_Coord grid = clic.getGrid();
	bool selected = false;
	string name ="";
	if (m_grid[grid.x+1][grid.y+1].main != nullptr){
		unselectedAll(grid.x+1,grid.x+1);
		name = m_grid[grid.x+1][grid.y+1].main->getName();
			if(name == "ArcherTower" || name == "Turbine"){
				cout << "Found top " << name;
				m_grid[grid.x+1][grid.y+1].main->reverseSelectedStatus();
				selected = true;
			}
			else{
				cout << "Found " << name;
			}
	}
	else{
		if (m_grid[grid.x][grid.y].main != nullptr){
			name = m_grid[grid.x][grid.y].main->getName();
				unselectedAll(grid.x,grid.y);
				if(name == "ArcherTower"|| name == "Turbine"){
					cout << "Found bottom" << name;
					m_grid[grid.x][grid.y].main->reverseSelectedStatus();
					selected = true;
				}
				else{
					cout << "Found " << name;
				}
		}
		else{
			unselectedAll(grid.x,grid.y);
			cout << "Nothing";
		}

	}
	cout << " at " << grid.x << ":" << grid.y << endl;
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

bool C_Grid::mainEmpty(int x_grid, int y_grid, C_GameUnits *current){
    if(m_grid[x_grid][y_grid].main == nullptr || m_grid[x_grid][y_grid].main == current ){
        return false;
    }
    else{
        return true;
    }
};

void C_Grid::darkenGround(int x_screen, int y_screen){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	Sint16 w =  TILE_HALF_WIDTH;
	Sint16 h =  w/2;
	Sint16 x1 = x_screen - w;

	Sint16 y1 = y_screen - h + 36; //center
	Sint16 x2 = x1 + w;
	Sint16 y2 = y1 + h;
	Sint16 x3 = x1 + (w*2);
	Sint16 y3 = y1;
	Sint16 x4 = x2;
	Sint16 y4 = y1 - h;
	Sint16 vx[] = {x1,x2,x3,x4};
	Sint16 vy[] = {y1,y2,y3,y4};
	int R = 30, G = 30, B = 30, A = 50;

	filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);
}
