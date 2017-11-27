#include "level.h"
#include "invaders.h"
#include "grid.h"

using namespace std;


C_Level::C_Level():
	m_name("level"),
	m_id(1)
{
	m_count = ++m_id;
}


C_Level::~C_Level()
{
}

void C_Level::status(){
	cout << m_name << " " << m_id << endl;
}

void C_Level::sendNextWave(list<C_GameUnits*>& lB){

	C_Grid& grid=C_Grid::Instances();
		vector <S_boat> l;
		S_boat temp = {1,1,14};
		l.push_back(temp);
		temp.x = 6;
		temp.y = 14;
		l.push_back(temp);
		temp.x = 5;
		temp.y = 12;
		l.push_back(temp);
		temp.x = 7;
		l.push_back(temp);
		temp.x = 3;
		l.push_back(temp);

		for (size_t i = 0; i < l.size();i++){
		grid.addANewBoat(l[i].x,l[i].y,l[i].rank);
		lB.push_back(grid.getUnits (l[i].x,l[i].y));
	}
	cout << "Next wave" << endl;
}

