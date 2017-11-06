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

void C_Level::sendNextWave(list<C_GameUnits*> boatList){
	cout << "Next wave" << endl;
}

