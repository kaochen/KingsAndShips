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


int C_GameUnits::getRank() const
{
	return m_rank;
}

int C_GameUnits::getXGrid() const
{
	return m_coord->getXGrid ();
}

int C_GameUnits::getYGrid() const
{
	return m_coord->getYGrid ();
}

void C_GameUnits::setGridXY(int x_grid, int y_grid){
	delete m_coord;
	m_coord = new C_CoordGrid (x_grid,y_grid);

}


int C_GameUnits::getXScreen() const
{
	return m_coord->getXScreen ();
}

int C_GameUnits::getYScreen() const
{
	return m_coord->getYScreen ();
}



int C_GameUnits::getYCenterOffset() const
{
	return m_y_center_offset;
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

void C_GameUnits::setDirection(string strDirection)
{
	m_strDirection =  strDirection;
}

string C_GameUnits::intDirectionToStr(int direction){
	string strDirection = "EE";
	switch (direction){
		case 0:
			strDirection = "SS";
		break;
		case 1:
			strDirection = "SE";
		break;
		case 2:
			strDirection = "EE";
		break;
		case 3:
			strDirection = "NE";
		break;
		case 4:
			strDirection = "NN";
		break;
		case 5:
			strDirection = "NW";
		break;
		case 6:
			strDirection = "WW";
		break;
		case 7:
			strDirection = "SW";
		break;
		default:
			strDirection = "EE";
		break;
	}
	m_strDirection = strDirection;
	cout << "Convert " << direction << " to " << strDirection << endl;
	return strDirection;

}

void C_GameUnits::changeDirection(int x_cursor, int y_cursor){

			int ab = x_cursor - m_coord->getXScreen ();
			int bc = y_cursor - m_coord->getYScreen ();
			double angle = atan2(ab,bc);
			angle = angle *180/3.14159265359;
			if (angle > -22.5 && angle <= 22.5){
				m_strDirection = "SS";
				}
			else if (angle > 22.5 && angle <= 67.5){
				m_strDirection = "SE";
			}
			else if(angle > 67.5 && angle <=112.5){
				m_strDirection = "EE";
				}
			else if(angle > 112.5 && angle <=157.5){
				m_strDirection = "NE";
				}
			else if((angle > 157.5 && angle <=180) || (angle > -180 && angle <= -157.5)){
				m_strDirection = "NN";
				}
			else if(angle > -157.5 && angle <= -112.5){
				m_strDirection = "NW";
				}
			else if(angle > -112.5 && angle <=-67.5){
				m_strDirection = "WW";
				}
			else if(angle > -67.5 && angle <=-22.5){
				m_strDirection = "SW";
				}
			else{
				m_strDirection = "EE";
				cout <<"Angle ??? " << angle << endl;
				}

}


C_GameUnits * C_GameUnits::getUnit(){
	return this;
}
