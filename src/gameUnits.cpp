#include "gameUnits.h"
#include "surfaces.h"
#include "grid.h"
#include "time.h"
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
	m_animNbr = 0;
	m_lastAnimTime = 0;
	m_anim2Nbr = 0;
	m_lastAnim2Time = 0;
	C_Grid& grid=C_Grid::Instances();
	m_x_screen = grid.xGridToXScreen(x_grid,y_grid);
	m_y_screen = grid.yGridToYScreen(x_grid,y_grid);

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
{
	m_direction = UNKNOWN;
}

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

void C_GameUnits::render(int x_screen, int y_screen){
	string name = getName();
	int rank = getRank();
	string fileName = name + "_0" + to_string(rank) + "_00.png" ;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, x_screen,y_screen + m_y_center_offset);
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


int C_GameUnits::getXScreen() const
{
	return m_x_screen;
}

int C_GameUnits::getYScreen() const
{
	return m_y_screen;
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


int C_GameUnits::getAnimTileNbr(int firstTileNbr, int lastTileNbr, int frameRate){
	C_Time& time=C_Time::Instances();
	long delay = time.getFrameDuration()*frameRate;
	long current = SDL_GetTicks();
	if (current > m_lastAnimTime + delay){
		m_animNbr++;
		m_lastAnimTime = current;
		}

	if (m_animNbr > lastTileNbr){
		m_animNbr = firstTileNbr;
	}
	return m_animNbr;
}

int C_GameUnits::getAnim2FrameNbr(int lastFrameNbr, int frameRate){
	C_Time& time=C_Time::Instances();
	long delay = time.getFrameDuration()*frameRate;
	long current = SDL_GetTicks();
	if (current > m_lastAnim2Time + delay){
		m_anim2Nbr++;
		m_lastAnim2Time = current;
		}

	if (m_anim2Nbr > lastFrameNbr){
		m_anim2Nbr = 0;
	}
	return m_anim2Nbr;
}

