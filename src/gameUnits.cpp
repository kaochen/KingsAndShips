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
	m_y_center_offset(0),
	m_strDirection("EE"),
	m_direction(UNKNOWN),
	m_selected(false)
{

	for (int i = 0; i < MAX_ANIM; i++){
		m_animation[i]= new C_AnimTime();
	}
	cout << "Add new unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
	S_Coord coord;
	coord.x = x_grid;
	coord.y = y_grid;
	m_coord = new C_CoordGrid(coord);
	m_old_coord = new C_CoordGrid(coord);
	m_coord->displayStatus();
}

//delete
C_GameUnits::~C_GameUnits()
{
}



//get attibuts
string C_GameUnits::getName() const
{
	return m_name;
}


void C_GameUnits::displayStatus() const
{
 	cout << "Name: " << m_name << " Life: " << m_life  << " Rank : " << m_rank<< endl;
 	m_coord->displayStatus();
}

void C_GameUnits::render(S_Coord screen){
	string fileName = m_name + "_0" + to_string(m_rank) + "_" + m_strDirection + "_0" ;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, screen.x,screen.y + m_y_center_offset);
}

void C_GameUnits::receiveDamage(S_Weapon weapon)
{
	m_life -=weapon.damage;
	if (m_life < 0)
	{
		m_life = 0;
	}
}

bool C_GameUnits::alive() const
{
	return m_life>0;
}



int C_GameUnits::getXGrid() const
{
	return m_coord->getXGrid ();
}

int C_GameUnits::getYGrid() const
{
	return m_coord->getYGrid ();
}


int C_GameUnits::getXScreen() const
{
	return m_coord->getXScreen ();
}

int C_GameUnits::getYScreen() const
{
	return m_coord->getYScreen ();
}




int C_GameUnits::getDistance(int x, int y) const
{
	int sideX = m_coord->getXScreen () - x;
	int sideY = m_coord->getYScreen () - y;
	int dist = sqrt(sideX*sideX + sideY*sideY);
	return dist;
}

void C_GameUnits::kill()
{
	C_Grid& grid=C_Grid::Instances();
	cout << "kill boat from:" << m_coord->getXGrid () << ":" << m_coord->getYGrid () << endl;
 	grid.moveToDead(m_coord->getXGrid (), m_coord->getYGrid ());
}


bool C_GameUnits::getSelectedStatus() const
{
	return m_selected;
}


void C_GameUnits::setSelectedStatus(bool status)
{
	m_selected = status;
}

void C_GameUnits::reverseSelectedStatus()
{
	if (m_selected == false){
		m_selected =  true;
	}
	else
		m_selected =  false;
}



C_GameUnits * C_GameUnits::getUnit(){
	return this;
}
