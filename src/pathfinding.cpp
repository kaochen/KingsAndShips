#include "pathfinding.h"
#include "grid.h"

using namespace std;

C_Node::C_Node(const int x_grid,const int y_grid, const bool block){
	m_x_grid = x_grid;
	m_y_grid = y_grid;
	m_block = block;
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

bool C_Node::getOpen() const{
	return m_open;
}

void C_Node::setOpen(bool open){
	m_open = open;
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

void C_Node::calcG(C_Node* gridNode[GRID_SIZE][GRID_SIZE],
		    multimap <int,C_Node*>* m_openNodes){

	m_open = false; //close the current node
	int c = 0;

	cout << "For : " << m_x_grid << ":" << m_y_grid << endl;
	for (int y = m_y_grid - 1; y < (m_y_grid + 2); y++){
		for (int x = m_x_grid - 1; x < (m_x_grid + 2); x++){
			if(x >= 0 && x <= GRID_SIZE && y >= 0 && y <= GRID_SIZE){
				c++;
				if (c != 5){
					int G_offset = G_HV;
					if ( (x+y) % 2 == 0)
						G_offset = G_DIAG;

					C_Node *current = gridNode[x][y];
						if (current != nullptr){
							if (current->getBlock() == false){
								cout << "	Testing : " << x << ":" << y << endl;
								int tmpG = current->getG();
								int tmpF = current->getH();
								if (tmpG == 0 || tmpF > (tmpG + G_offset)){
									current->setG(tmpG + G_offset);
									if(current->getOpen() == true){
										m_openNodes->insert(pair<int, C_Node*>(current->getF(),current));
										current->setOpen(false);
										}
								}
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

int C_Node::getF() const{
	return m_F;
}

void C_Node::setG(int value){
	m_G = value;
	m_F = m_H + m_G;
}


//---------------------------------------------------------

C_Path::C_Path(int x_dest, int y_dest)
{

	C_Grid& grid=C_Grid::Instances();
	m_start = nullptr;
	m_destination = nullptr;
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
			bool block = true;
			if (grid.waterway(x,y)){
				block = false;
			}
		m_gridNode[x][y] = new C_Node(x,y,block);
		}
	}
	setTown(x_dest,y_dest);
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		m_gridNode[x][y]->calcH(m_gridNode[x_dest][y_dest]);
		}
	}
	cout << "Construct C_Path done" << endl;
}
C_Path::~C_Path()
{
}


void C_Path::calcPath(int x_start,int y_start, int x_dest, int y_dest){
	m_start = m_gridNode[x_start][y_start];
	m_openNodes.insert(pair<int, C_Node*>(0,m_start));
	m_destination = m_gridNode[x_dest][y_dest];

	std::multimap<int, C_Node*>::iterator it;
	for (it=m_openNodes.begin(); it!=m_openNodes.end(); it++){
		(*it).second->calcG(m_gridNode,&m_openNodes);
		m_openNodes.erase(it);
		cout << "---------" << endl;
	}
}

void C_Path::displayOpenList(){
	std::multimap<int, C_Node*>::iterator it;
	for (it=m_openNodes.begin(); it!=m_openNodes.end(); it++){
		cout << "Open Nodes: " << endl;
		(*it).second->displayStatus();
	}

}


C_Node* C_Path::searchOpenList(int F){
	std::multimap<int, C_Node*>::iterator it;
	C_Node * current = nullptr;
	for (it=m_openNodes.begin(); it!=m_openNodes.end(); it++){
		if ((*it).first == F){
			current = (*it).second;
		}
		else{
			current = nullptr;
		}
	}
	return current;
}

void C_Path::setTown(int x_grid,int y_grid){
	//reset
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		m_gridNode[x][y]->setTown(false);
		}
	}
	//set
	m_gridNode[x_grid][y_grid]->setTown(true);
}

