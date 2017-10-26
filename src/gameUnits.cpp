#include "gameUnits.h"
#include "surfaces.h"

using namespace std;


//constructor
C_GameUnits::C_GameUnits():
	m_name("UNIT"),
	m_life(100),
	m_rank(1),
	m_x_grid(0),
	m_y_grid(0)
{
	cout << "Add new default unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
}

C_GameUnits::C_GameUnits(string name, int rank):
	m_name(name),
	m_life(100),
	m_rank(rank),
	m_x_grid(0),
	m_y_grid(0)
{
	cout << "Add new unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
}

//delete
C_GameUnits::~C_GameUnits()
{
}

//copy
C_GameUnits::C_GameUnits(C_GameUnits const& original):
	m_name(original.m_name),
	m_life(original.m_life),
	m_rank(original.m_rank)
{}

//get attibuts
string C_GameUnits::getName() const
{
	return m_name;
}


void C_GameUnits::displayStatus() const
{
 	cout << "Name: " << m_name << " Life: " << m_life  << " Rank : " << m_rank<< endl;
}

void C_GameUnits::render(int x_iso, int y_iso, SDL_Renderer *renderer){
	string name = getName();
	int rank = getRank();
	string fileName = name + "_0" + to_string(rank) + "_00.png" ;
	//cout << "image name is "<< fileName << endl;
	renderTexture(C_Texture::getText("SimpleWaterTile.png"), renderer, x_iso,y_iso);
	renderTexture(C_Texture::getText(fileName), renderer, x_iso,y_iso);
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


