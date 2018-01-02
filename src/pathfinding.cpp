#include "pathfinding.h"
#include "window.h"
#include "grid.h"

#include <SDL2_gfxPrimitives.h>
#include <queue>

using namespace std;

C_Node::C_Node(const int x_grid,const int y_grid, const bool block){
	S_Coord coo;
	coo.x = x_grid;
	coo.y = y_grid;
	m_coord = new C_CoordGrid(coo);
	m_block = block;
	m_G = 0;
	m_H = 0;
	m_F = m_G + m_H;
	m_Town = false;
	m_parent = nullptr;
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

void C_Node::setParent(C_Node * parent){
	m_parent = parent;
}

C_Node* C_Node::getParent(){
	return m_parent;
}

int C_Node::getXGrid() const{

	return m_coord->getGrid().x;
}

int C_Node::getYGrid() const{
	return m_coord->getGrid().y;
}

void C_Node::displayStatus(){
	cout << "Node :" << m_coord->getGrid().x << ":" << m_coord->getGrid().y;
	cout << " F:" << m_F << " G:" << m_G << " H:" << m_H;
	if (m_Town)
		cout << " Town: true <---";
	else
		cout << " Town: false";
	cout << endl;
};

void C_Node::calcH(const C_Node* target){
	if (m_Town == false && m_block == false){
		int moveOnX =  target->getXGrid() - m_coord->getGrid().x;
			if (moveOnX < 0)
				moveOnX *= -1;
		int moveOnY =  target->getYGrid() - m_coord->getGrid().y;
			if (moveOnY < 0)
				moveOnY *= -1;
		m_H = (moveOnX + moveOnY) *10;
	}
}

void C_Node::calcG(C_Node* gridNode[GRID_SIZE][GRID_SIZE],
		    multimap <int,C_Node*>* m_openNodes){

	m_open = false; //close the current node
	int c = 0;
	int x_grid = m_coord->getGrid().x;
	int y_grid = m_coord->getGrid().y;
	cout << "For : " << x_grid << ":" << y_grid << " F: " << m_F << endl;

	cout << "	Testing : ";
	for (int y = y_grid - 1; y < (y_grid + 2); y++){
		for (int x = x_grid - 1; x < (x_grid + 2); x++){
			if(x >= 0 && x <= GRID_SIZE && y >= 0 && y <= GRID_SIZE){
				c++;
				if (c != 5){
					int G_offset = G_HV;
					if ( (x+y) % 2 == 0)
						G_offset = G_DIAG;

					C_Node *current = gridNode[x][y];
						if (current != nullptr){
							if (current->getBlock() == false){
								cout << x << ":" << y << " ";
								int tmpG = current->getG();
								int tmpF = current->getH();
								if (tmpG == 0 || tmpF > (tmpG + G_offset)){
									current->setG(tmpG + G_offset);
									if(current->getOpen() == true){
										m_openNodes->insert(pair<int, C_Node*>(current->getF(),current));
										current->setOpen(false);
										current->setParent(gridNode[x_grid][y_grid]);
										}
								}
							}
						}
				}
			}
		}
	}
	cout << endl;
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

	cout << "---------" << endl;
	std::multimap<int, C_Node*>::reverse_iterator rit;
	for (rit=m_openNodes.rbegin(); rit!=m_openNodes.rend(); rit--){
			cout << (*rit).first << " : ";
			(*rit).second->calcG(m_gridNode,&m_openNodes);
			m_openNodes.erase(--(rit.base()));
	}
	cout << "---------" << endl;
	loadPath();
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

void C_Path::loadPath(){
	C_Node* current = m_destination;

	while(current->getParent() != nullptr){
		 m_path.push(current);
		 current = current->getParent();
		 //cout <<"parent: "<< current->getXGrid() << ":" << current->getYGrid() << endl;
		 }

	//m_path.push(m_start); //do not forget the start
}

void C_Path::showPath(){
	stack<C_Node*> tmp = m_path;
	int c =0;
	while(tmp.empty() == false){
		cout << tmp.top()->getXGrid() << ":" << tmp.top()->getYGrid() << " >> ";
		tmp.pop();
		c++;
	}
	cout << "steps:" << c << endl;
}

void C_Path::displayPath(){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	int R = 0, G = 0, B = 220, A = 100;
	C_Grid& grid=C_Grid::Instances();
	stack<C_Node*> tmp = m_path;
	while(tmp.empty() == false){
		int x_grid = tmp.top()->getXGrid() + 2;
		int y_grid = tmp.top()->getYGrid() + 2;
		int x_screen = grid.xGridToXScreen(x_grid,y_grid);
		int y_screen = grid.yGridToYScreen(x_grid,y_grid) + TILE_HALF_HEIGHT;
		filledEllipseRGBA(renderer,x_screen,y_screen,10,5,R,G,B,A);
		tmp.pop();
	}
}

stack<C_Node*> C_Path::getPath(){
	return m_path;
}

void C_Path::goNextStep(){
	if(!m_path.empty())
		m_path.pop();
}


