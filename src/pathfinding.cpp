#include "pathfinding.h"
#include "grid.h"

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
	m_open = true;
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
	if (m_block)
		m_open = false;
	else
		m_open = true;
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
		m_H = (moveOnX + moveOnY) *10;
	}
}

void C_Node::calcG(){
	C_Grid& grid=C_Grid::Instances();
	m_open = false; //close the current node
	int c = 0;
for (int y = m_y_grid - 1; y < (m_y_grid + 2); y++){
	for (int x = m_x_grid - 1; x < (m_x_grid + 2); x++){
		c++;
		if (c != 5){
			int G_offset = G_HV;
			if ( (x+y) % 2 == 0)
				G_offset = G_DIAG;

			C_Node *current = grid.getNode(x,y);
				if (current != nullptr){
					if (current->getBlock() == false){
						cout << "Testing : " << x << ":" << y << endl;
						int tmpG = current->getG();
						int tmpF = current->getH();
						if (tmpG == 0 || tmpF > (tmpG + G_offset)){
							current->setG(tmpG + G_offset);
						}
					}
				}
		}
	}
}
}

int C_Node::getG() const{
	return m_G;
}

int C_Node::getH() const{
	return m_H;
}

void C_Node::setG(int value){
	m_G = value;
	m_F = m_H + m_G;
}


//---------------------------------------------------------

C_Path::C_Path()
{
	m_start = nullptr;
	m_destination = nullptr;
}
C_Path::~C_Path()
{
}


void C_Path::CalcPath(C_Node* start, C_Node* destination){
	if (start != nullptr && destination != nullptr){
		m_start = start;
		v_openNodes.push_back(start);
		m_destination = destination;
		}
}

void C_Path::displayOpenList(){
	for (size_t i = 0; i < v_openNodes.size(); i++){
		v_openNodes[i]->displayStatus();
	}

}
