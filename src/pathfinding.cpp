#include "pathfinding.h"

using namespace std;

C_Node::C_Node(const int x_grid,const int y_grid){
	m_x_grid = x_grid;
	m_y_grid = y_grid;
	m_block = true;
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

bool C_Node::getTown() const{
	return m_Town;
}

void C_Node::setBlock(bool block){
	m_block = block;
}

bool C_Node::getBlock() const{
	return m_block;
}

int C_Node::getXGrid() const{
	return m_x_grid;
}

int C_Node::getYGrid() const{
	return m_y_grid;
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

void C_Node::calcH(const C_Node* target){
	if (m_Town == false && m_block == false){
		int moveOnX =  target->getXGrid() - m_x_grid;
			if (moveOnX < 0)
				moveOnX *= -1;
		int moveOnY =  target->getYGrid() - m_y_grid;
			if (moveOnY < 0)
				moveOnY *= -1;
		m_H = moveOnX + moveOnY;
	}
}

