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
	m_y_grid(y_grid),
	m_y_center_offset(0),
	m_strDirection("EE"),
	m_direction(UNKNOWN),
	m_selected(false)
{

	C_Grid& grid=C_Grid::Instances();
	m_x_screen = grid.xGridToXScreen(x_grid,y_grid);
	m_y_screen = grid.yGridToYScreen(x_grid,y_grid);
	for (int i = 0; i < MAX_ANIM; i++){
		m_animation[i]= new C_AnimTime();
	}
	cout << "Add new unit: "<< m_name <<" life: "<< m_life <<" rank: "<< m_rank << endl;
 	cout << "\tx_grid:"<< m_x_grid << " y_grid:"<< m_y_grid;
 	cout << "\tx_screen:"<< m_x_screen << " y_screen:"<< m_y_screen << endl;

	m_coord = new C_Coord(x_grid,y_grid);
	m_coord->displayStatus();
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
	m_y_center_offset (original.m_y_center_offset),
	m_selected(false)
{
	m_direction = UNKNOWN;
	m_lastDirection = m_direction;
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
	string fileName = m_name + "_0" + to_string(m_rank) + "_" + m_strDirection + "_0" ;
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

			int ab = x_cursor - m_x_screen;
			int bc = y_cursor - m_y_screen;
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
