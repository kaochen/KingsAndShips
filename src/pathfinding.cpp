#include "pathfinding.h"
#include "window.h"
#include "grid.h"
#include "surfaces.h"
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
	m_angle = 0;
	m_dist = 0;
	m_open = true;
};

C_Node::~C_Node()
{
	C_TextureList& t=C_TextureList::Instances();
	t.freeTexture(m_h_texture_name );
	t.freeTexture(m_g_texture_name );
	t.freeTexture(m_f_texture_name );
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
	cout << " dist:" << m_dist << " angle" << m_angle;
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

	int x_grid = m_coord->getGrid().x;
	int y_grid = m_coord->getGrid().y;
	cout << "For : " << x_grid << ":" << y_grid << " F: " << m_F << endl;
	C_Node *tested = gridNode[x_grid][y_grid];
	int currentG = tested->getG();
	cout << "	Testing : ";
	for (int y = y_grid - 1; y <= (y_grid + 1); y++){
		for (int x = x_grid - 1; x <= (x_grid + 1); x++){
			if(x >= 0 && x <= GRID_SIZE && y >= 0 && y <= GRID_SIZE){
				if ((x != x_grid || y != y_grid)){
					//Calc G_offset
					int G_offset = calcG_offset(x_grid, y_grid,x,y);
					bool corner = crossACorner(x_grid, y_grid,x,y, gridNode);
					//
					C_Node *tested = gridNode[x][y];
						if (tested != nullptr){
							if (tested->getBlock() == false && corner == false){
								cout << x << ":" << y << " ";
								int tmpG = tested->getG();
								if (tmpG == 0 || (currentG + G_offset) < tmpG ){
									tested->setG(currentG + G_offset);
									if(tested->getOpen() == true){
										m_openNodes->insert(pair<int, C_Node*>(tested->getF(),tested));
										tested->setOpen(false);
										tested->setParent(gridNode[x_grid][y_grid]);
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


int C_Node::calcG_offset(int x_from, int y_from,
			 int x_dest, int y_dest){

	if(x_from != x_dest && y_from != y_dest){
		return G_DIAG;
	}
	else{
		return G_HV;
	}
}

bool C_Node::crossACorner(int x_from, int y_from,
			  int x_dest, int y_dest,
			  C_Node* gridNode[GRID_SIZE][GRID_SIZE]){
	if(gridNode[x_from][y_dest]->getBlock() || gridNode[x_dest][y_from]->getBlock()){
		return true;
	}
	else{
		return false;
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

C_Coord* C_Node::getCoord() const{
	return m_coord;
}

void C_Node::setG(int value){
	m_G = value;
	m_F = m_H + m_G;
}

void C_Node::setDist(int dist, double angle){
	m_dist = dist;
	m_angle = angle;
}

int C_Node::getDist()const{
	return m_dist;
}

double C_Node::getAngle() const{
	return m_angle;
}

void C_Node::highlight(){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	int R = 200, G = 200, B = 200, A = 100;
	int x_screen = m_coord->getXScreen ();
	int y_screen = m_coord->getYScreen ();
	filledEllipseRGBA(renderer,x_screen, y_screen+ 14 ,10,5,R,G,B,A);
}

void C_Node::prepareRender(){
	int x_screen = m_coord->getXScreen ();
	int y_screen = m_coord->getYScreen ();

	C_TextureList& t=C_TextureList::Instances();

	m_h_texture_name = to_string(x_screen)+to_string(y_screen)+"h_value";
	string value = "H:"+to_string(m_H);
	t.loadTextAsTexturesIntoMap(m_h_texture_name, value, 10);

	m_g_texture_name = to_string(x_screen)+to_string(y_screen)+"g_value";
	value = "G:"+to_string(m_G);
	t.loadTextAsTexturesIntoMap(m_g_texture_name, value, 10);

	m_f_texture_name = to_string(x_screen)+to_string(y_screen)+"f_value";
	value = "F:"+to_string(m_F);
	t.loadTextAsTexturesIntoMap(m_f_texture_name, value, 15);
}

void C_Node::render(){

	int x_screen = m_coord->getXScreen ();
	int y_screen = m_coord->getYScreen ();

 	y_screen +=TILE_HALF_HEIGHT*2; //need a fix
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(m_h_texture_name,x_screen - 20,y_screen + 12);
	t.renderTexture(m_g_texture_name,x_screen + 20,y_screen + 12);
	t.renderTexture(m_f_texture_name,x_screen,y_screen + 25);

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

       std::multimap<int, C_Node*>::reverse_iterator rit;
       for(int count = 1; count > 0; count--){
              		int lowestF = findLowestF();
              		cout << "map size: " << m_openNodes.size() << " lowestF " << lowestF << endl;
     			int c = 0;
     		       std::multimap<int, C_Node*>::reverse_iterator rit2;
		       for (rit2=m_openNodes.rbegin(); rit2!=m_openNodes.rend(); rit2++){
		       			if((*rit2).first == lowestF && c < 1){
		       				c++;
				       		std::multimap<int, C_Node*> tmpList;
				        	cout << (*rit2).first << " : ";
					       	(*rit2).second->calcG(m_gridNode,&tmpList);
					       	if(!m_openNodes.empty()){
					       		m_openNodes.erase(--(rit2.base()));
					       	}
					       	else{
					       		cout << "list is empty" << endl;
					       	}


						std::multimap<int, C_Node*>::iterator it2;
						for (it2=tmpList.begin(); it2!=tmpList.end(); it2++){
							m_openNodes.insert(pair<int, C_Node*>((*it2).first,(*it2).second));
							}
					}
			count = m_openNodes.size();
			}
	}
	cout << "---------" << endl;
	loadPath();
}

void C_Path::displayOpenList(){
	std::multimap<int, C_Node*>::iterator it;
	cout << "Open Nodes: " << endl;
	for (it=m_openNodes.begin(); it!=m_openNodes.end(); it++){
		(*it).second->displayStatus();
	}

}

int C_Path::findLowestF(){
	std::multimap<int, C_Node*>::iterator it;
	int lowest = 10000;
	for (it=m_openNodes.begin(); it!=m_openNodes.end(); it++){
		if ((*it).first < lowest)
			lowest = (*it).first;
	}
	return lowest;
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
	if(current->getParent() == nullptr){
		current = closestNode();
	}
	while(current->getParent() != nullptr){
		 m_path.push(current);
		 current = current->getParent();
		 //cout <<"parent: "<< current->getXGrid() << ":" << current->getYGrid() << endl;
		 }
	cout << "load Path" << endl;
	//prepare render for debug
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		m_gridNode[x][y]->prepareRender ();
		}
	}
	//m_path.push(m_start); //do not forget the start
}

void C_Path::showPath(){
	show_H_G_F();
	stack<C_Node*> tmp = m_path;
	int c =0;
	while(tmp.empty() == false){
		cout << tmp.top()->getXGrid() << ":" << tmp.top()->getYGrid() << " >> ";
		tmp.pop();
		c++;
	}
	cout << "steps:" << c << endl;
}

void C_Path::show_H_G_F(){
	for(int y= 0; y < GRID_SIZE; y++){
		for(int x= 0; x < GRID_SIZE; x++){
			C_Node* c = m_gridNode[x][y];
			cout << "|" << c->getF();
		}
		cout << endl;
	}
}

C_Node* C_Path::closestNode(){
	C_Node *closest = nullptr;
	int lowestF = 10000;
	for(int y= 0; y < GRID_SIZE; y++){
		for(int x= 0; x < GRID_SIZE; x++){
			int F = m_gridNode[x][y]->getF();
			if (F > 0 && F < lowestF){
				closest = m_gridNode[x][y];
				lowestF = F;
			}
		}
	}
	int lowestH = 10000;
	for(int y= 0; y < GRID_SIZE; y++){
		for(int x= 0; x < GRID_SIZE; x++){
			int H = m_gridNode[x][y]->getH();
			int F = m_gridNode[x][y]->getF();
			if(F == lowestF){
				if (H > 0 && H < lowestH){
					if(closest != nullptr){
						closest = m_gridNode[x][y];
						lowestH = H;
					}
				}
			}
		}
	}

	return closest;
}



void C_Path::displayPath(){
	stack<C_Node*> tmp = m_path;
	while(tmp.empty() == false){
		tmp.top()->highlight();
		tmp.pop();
	}
	C_Set& settings=C_Set::Instances();
	if(settings.getDebugModeStatus()){
		for (size_t y = 0; y < GRID_SIZE; y++){
			for (size_t x = 0; x < GRID_SIZE; x++){
			m_gridNode[x][y]->render();
			}
		}
	}
}

stack<C_Node*> C_Path::getPath(){
	return m_path;
}


void C_Path::goNextStep(){
	if(m_path.size() > 1 ||!m_path.empty())
		m_path.pop();
}


