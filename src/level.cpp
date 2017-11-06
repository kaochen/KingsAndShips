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

void C_Level::sendNextWave(S_boat list[BOAT_LIST_SIZE]){
		list[0].rank = 1;
		list[0].x = 1;
		list[0].y = 14;
		list[1].rank = 1;
		list[1].x = 6;
		list[1].y = 14;
		list[2].rank = 1;
		list[2].x = 5;
		list[2].y = 12;
		list[3].rank = 1;
		list[3].x = 8;
		list[3].y = 12;
	cout << "Next wave" << endl;
}

