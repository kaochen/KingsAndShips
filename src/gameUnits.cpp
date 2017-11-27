#include "gameUnits.h"
#include "surfaces.h"
#include "grid.h"
#include <cmath>

using namespace std;


//constructor

C_GameUnits::C_GameUnits(string name, int x_grid, int y_grid, int rank):
	m_name(name),
	m_life(MAX_LIFE),
	m_rank(rank),
	m_x_grid(x_grid),
	m_y_grid(y_grid)
{
	m_y_center_offset = 0;
	xyGridToXYScreen();
	cout << "Add new unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
 	cout << "\tx_grid:"<< m_x_grid << " y_grid:"<< m_y_grid;
 	cout << "\tx_screen:"<< m_x_screen << " y_screen:"<< m_y_screen << endl;
}

//delete
C_GameUnits::~C_GameUnits()
{
}

//copy
C_GameUnits::C_GameUnits(C_GameUnits const& original):
	m_name(original.m_name),
	m_life(original.m_life),
	m_rank(original.m_rank),
	m_x_grid(original.m_x_grid),
	m_y_grid(original.m_y_grid),
	m_x_screen (original.m_x_screen),
	m_y_screen (original.m_y_screen),
	m_y_center_offset (original.m_y_center_offset)
{}

//get attibuts
string C_GameUnits::getName() const
{
	return m_name;
}


void C_GameUnits::displayStatus() const
{
 	cout << "Name: " << m_name << " Life: " << m_life  << " Rank : " << m_rank<< endl;
 	cout << "\tx_grid:"<< m_x_grid << " y_grid:"<< m_y_grid << endl;
}

void C_GameUnits::render(int x_screen, int y_screen, SDL_Renderer *renderer){
	string name = getName();
	int rank = getRank();
	string fileName = name + "_0" + to_string(rank) + "_00.png" ;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, renderer, x_screen,y_screen + m_y_center_offset);
}

void C_GameUnits::receiveDamage(int nbDamage)
{
	m_life -=nbDamage;
	if (m_life < 0)
	{
		m_life = 0;
	}
}

bool C_GameUnits::alive() const
{
	return m_life>0;
}


int C_GameUnits::getRank() const
{
	return m_rank;
}

int C_GameUnits::getXGrid() const
{
	return m_x_grid;
}

int C_GameUnits::getYGrid() const
{
	return m_y_grid;
}

void C_GameUnits::setGridXY(int x_grid, int y_grid){
	m_x_grid = x_grid;
	m_y_grid = y_grid;
}

void C_GameUnits::xyGridToXYScreen(){
			C_Set& settings=C_Set::Instances();
			m_x_screen = settings.getWindowWidth()/2 + (m_x_grid - m_y_grid)* TILE_HALF_WIDTH;
			m_y_screen = (m_y_grid + m_x_grid - 4) * TILE_HALF_HEIGHT - settings.getWindowHeight()/2;
}

int C_GameUnits::getXScreen() const
{
	return m_x_screen;
}

int C_GameUnits::getYScreen() const
{
	return m_y_screen;
}

int C_GameUnits::xScreenToXGrid(int x_screen, int y_screen){
		C_Set& settings=C_Set::Instances();
		float xOffset = (settings.getWindowWidth() /2);
		float yOffset = (settings.getWindowHeight() /2);
		float tempX = 0.0;
		tempX = ( ((x_screen - xOffset ) / TILE_HALF_WIDTH + (y_screen + yOffset)/TILE_HALF_HEIGHT )/2);
		return tempX + 2;
		}

int C_GameUnits::yScreenToYGrid(int x_screen, int y_screen){
		C_Set& settings=C_Set::Instances();
		float xOffset = (settings.getWindowWidth() /2);
		float yOffset = (settings.getWindowHeight() /2);
		float tempY = 0.0;
		tempY = ( (y_screen + yOffset )/(TILE_HALF_HEIGHT*2) - (x_screen - xOffset)/(TILE_HALF_WIDTH*2));
		return tempY + 2;
		}



int C_GameUnits::getYCenterOffset() const
{
	return m_y_center_offset;
}



int C_GameUnits::getDistance(int x, int y) const
{
	int sideX = m_x_screen - x;
	int sideY = m_y_screen - y;
	int dist = sqrt(sideX*sideX + sideY*sideY);
	return dist;
}

void C_GameUnits::kill()
{
	C_Grid& grid=C_Grid::Instances();
	cout << "kill boat from:" << m_x_grid << ":" << m_y_grid << endl;
 	grid.moveToDead(m_x_grid, m_y_grid);
}




