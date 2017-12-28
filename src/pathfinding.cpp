#include "pathfinding.h"

using namespace std;

C_Node::C_Node(const int x_grid,const int y_grid){
	m_x_grid = x_grid;
	m_y_grid = y_grid;
	m_G = 0;
	m_H = 0;
	m_F = m_G + m_H;
	m_Town = false;
	m_Parent = nullptr;
};

C_Node::~C_Node()
{
};

void C_Node::setTown(bool town){
	m_Town = town;
}

void C_Node::displayStatus(){
	cout << "Node :" << m_x_grid << ":" << m_y_grid;
	cout << " F:" << m_F << " G:" << m_G << " H:" << m_H;
	if (m_Town)
		cout << " Town: true <---";
	else
		cout << " Town: false";
	cout << endl;
};

