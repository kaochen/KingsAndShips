/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>

#include "pathfinding.h"
#include "grid.h"

#include "../settings.h"
#include "../message.h"

using namespace std;

C_Path::C_Path(int x_dest, int y_dest)
{
	C_Grid& grid=C_Grid::Instances();
	C_Settings& settings=C_Settings::Instances();
    for (size_t y = 0; y < settings.getGridSize(); y++){
	    vector <C_Node> line;
		for (size_t x = 0; x < settings.getGridSize(); x++){
		    bool block = true;
		    if (grid.waterway(x,y)){
		        if(grid.mainEmpty(x,y) || grid.boatInMain(x,y)){
			        block = false;
			    }
		    }
	        C_Node z(x,y,block);
            line.push_back(z);
	    }
	    m_vgridNode.push_back(line);
	}

	m_start = nullptr;
	m_destination = nullptr;


	setTown(x_dest,y_dest);
	for (size_t y = 0; y < m_vgridNode.size(); y++){
		for (size_t x = 0; x < m_vgridNode.size(); x++){
		m_vgridNode[x][y].calcH(&m_vgridNode[x_dest][y_dest]);
		}
	}
	C_Message m;
	m.printDebugPath("Construct C_Path done\n");
}
C_Path::~C_Path()
{
}


void C_Path::calcPath(int x_start,int y_start, int x_dest, int y_dest){
	m_start = &m_vgridNode[x_start][y_start];
	m_vgridNode[x_start][y_start].setBlock(false);
	m_openNodes.insert(pair<int, C_Node*>(0,m_start));

	m_destination = &m_vgridNode[x_dest][y_dest];
    C_Message m;
    ostringstream message;

       std::multimap<int, C_Node*>::reverse_iterator rit;

       for(int count = 1; count > 0; count--){
                if(m_openNodes.size()>0){
              		int lowestF = findLowestF();
              		message << "map size: " << m_openNodes.size() << " lowestF " << lowestF;
     			int c = 0;
     		       std::multimap<int, C_Node*>::reverse_iterator rit2;
		       for (rit2=m_openNodes.rbegin(); rit2!=m_openNodes.rend(); rit2++){
		       			if((*rit2).first == lowestF && c < 1){
		       				c++;
				       		std::multimap<int, C_Node*> tmpList;
				        	message << ": " << (*rit2).first;

					       	if(m_openNodes.size()>1){
					       		m_openNodes.erase(--(rit2.base()));
					       	}
					       	else{
					       		//cout << "list is empty" << endl;
					       	}

						std::multimap<int, C_Node*>::iterator it2;
						for (it2=tmpList.begin(); it2!=tmpList.end(); it2++){
							m_openNodes.insert(pair<int, C_Node*>((*it2).first,(*it2).second));
							}
					}
			count = m_openNodes.size();
			}
			}
	}
	message << "\n---------\n";
	m.printDebugPath(message.str());

	if(m_openNodes.size()>0){
	loadPath();
	}
    else{
        m.printM("not path to load\n");
    }

}

void C_Path::displayOpenList(){
	std::multimap<int, C_Node*>::iterator it;
	C_Message m;
	m.printDebugPath("Open Nodes: \n");
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
	for (size_t y = 0; y < m_vgridNode.size(); y++){
		for (size_t x = 0; x < m_vgridNode.size(); x++){
		m_vgridNode[x][y].setTown(false);
		}
	}
	//set
	m_vgridNode[x_grid][y_grid].setTown(true);
	for(int j = -1; j <= 1; j++){
    	for(int i = -1; i <= 1; i++){
        	if(x_grid+i >= 0 && x_grid+i <= m_vgridNode.size() && y_grid+j >= 0 && y_grid+j <= m_vgridNode.size()){
        		    m_vgridNode[x_grid+i][y_grid+j].setBlock(false);
        		}
    		}
    	}
}

void C_Path::loadPath(){
	C_Node* current = m_destination;
	C_Message m;
	//clear path before

	while(!m_path.empty()){
	    m_path.pop();}
	if(current->getParent() == nullptr){
		current = closestNode();
	}
	if(current != nullptr){
	    while(current->getParent() != nullptr){
		     m_path.push(current);
		     current = current->getParent();
		     //cout <<"parent: "<< current->getXGrid() << ":" << current->getYGrid() << endl;
		     }
		     //if the boat is not already in the center of the tile, add the current tile to the path.
		     //m_path.push(current); //force the boat to recenter itself on its current tile.
		m.printDebugPath("path is loaded\n");
	    //prepare render for debug
	    C_Settings& settings=C_Settings::Instances();
	    if(settings.getDebugPathMode()){
		    for (size_t y = 0; y < m_vgridNode.size(); y++){
			    for (size_t x = 0; x < m_vgridNode.size(); x++){
			    m_vgridNode[x][y].prepareRender ();
			    }
		    }
	    }
	}
	//m_path.push(m_start); //do not forget the start
}

void C_Path::addANodeAtTheStartOfThePath(S_Coord grid){
		     m_path.push(&m_vgridNode[grid.x][grid.y]);
}


void C_Path::showPath(){
	C_Settings& settings=C_Settings::Instances();
	if(settings.getDebugPathMode()){
	    C_Message m;
	    string message ="";
	    show_H_G_F();
	    stack<C_Node*> tmp = m_path;
	    int c =0;
	    while(tmp.empty() == false){
		    message += to_string(tmp.top()->getXGrid()) + ":" + to_string(tmp.top()->getYGrid()) + " >> ";
		    tmp.pop();
		    c++;
	    }
	    m.printDebugPath(message + "steps: " + to_string(c) + "\n");
	}
}

void C_Path::show_H_G_F(){
    C_Message m;
    string message = "";
	for(size_t y= 0; y < m_vgridNode.size(); y++){
		for(size_t x= 0; x < m_vgridNode.size(); x++){
			C_Node* c = &m_vgridNode[x][y];
			message += "|" + to_string(c->getF());
		}
        m.printDebugPath(message + "\n");
        message ="";
	}
}

C_Node* C_Path::closestNode(){
	C_Node *closest = nullptr;
	size_t lowestF = 10000;
	for(size_t y= 0; y < m_vgridNode.size(); y++){
		for(size_t x= 0; x < m_vgridNode.size(); x++){
			size_t F = m_vgridNode[x][y].getF();
			if (F > 0 && F < lowestF){
				closest = &m_vgridNode[x][y];
				lowestF = F;
			}
		}
	}
	size_t lowestH = 10000;
	for(size_t y= 0; y < m_vgridNode.size(); y++){
		for(size_t x= 0; x < m_vgridNode.size(); x++){
			size_t H = m_vgridNode[x][y].getH();
			size_t F = m_vgridNode[x][y].getF();
			if(F == lowestF){
				if (H > 0 && H < lowestH){
					if(closest != nullptr){
						closest = &m_vgridNode[x][y];
						lowestH = H;
					}
				}
			}
		}
	}

	return closest;
}

bool C_Path::closeToDestination(int x_grid, int y_grid, int steps){
    int x_dist = m_destination->getXGrid() - x_grid;
    int y_dist = m_destination->getYGrid() - y_grid;

    if((x_dist >=-steps && x_dist <= steps)&&(y_dist >=-steps && y_dist <= steps))
        return true;
    else
        return false;
}

void C_Path::displayPath(){
	C_Settings& settings=C_Settings::Instances();
	if(settings.getDebugMode()){
	    //highlight the path in front of the boats
	    stack<C_Node*> tmp = m_path;
	    while(tmp.empty() == false){
		    tmp.top()->highlight();
		    tmp.pop();
	    }
	}
	if(settings.getDebugPathMode()){
	    //display H G and F numbers for debugging Pathfinding A*
		for (size_t y = 0; y < m_vgridNode.size(); y++){
			for (size_t x = 0; x < m_vgridNode.size(); x++){
			m_vgridNode[x][y].render();
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

