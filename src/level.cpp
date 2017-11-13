#include "level.h"
#include "invaders.h"

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

void C_Level::sendNextWave(C_GameUnits::S_layer grid[][GRID_SIZE], list<C_GameUnits*>& lB){
		vector <S_boat> l;
		S_boat temp = {1,1,14};
		l.push_back(temp);
		temp.x = 6;
		temp.y = 14;
		l.push_back(temp);
		temp.x = 2;
		temp.y = 12;
		l.push_back(temp);
		temp.x = 7;
		l.push_back(temp);
		temp.x = 5;
		l.push_back(temp);


		for (size_t i = 0; i < l.size();i++){
		grid[l[i].x][l[i].y].main = new C_invaders(l[i].x,l[i].y,l[i].rank);
		lB.push_back(grid[l[i].x][l[i].y].main);
	}
	cout << "Next wave" << endl;
}

