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
	m_destination = &m_vgridNode[x_dest][y_dest];
	m_vgridNode[x_start][y_start].setBlock(false);
    m_vopenNodes.push_back(m_start);

    C_Message m;
    ostringstream message;

    int lowestF = findLowestF();

    cout << "New Path\n";
    while (m_vopenNodes.size()>0){
            cout << "size "<< m_vopenNodes.size() << " ";
                if(m_vopenNodes[0] != nullptr){
                     C_Coord coord = *m_vopenNodes[0]->getCoord();
                     calcG(coord.getXGrid(),coord.getYGrid());
                     m_vopenNodes.erase(m_vopenNodes.begin() + 0);
                }
    }



	message << "\n---------\n";
	m.printDebugPath(message.str());

	//if(m_vopenNodes.size()>0){
	    loadPath();
	//}
    //else{
    //    m.printM("not path to load\n");
    //}

}

void C_Path::displayOpenList(){
	C_Message m;
	m.printDebugPath("Open Nodes: \n");

	for (size_t i = 0; i < m_vopenNodes.size(); i++){
	    if(m_vopenNodes[i])
		    m_vopenNodes[i]->displayStatus();
	}

}

size_t C_Path::findLowestF(){
	int lowest = 10000;
	for(size_t i = 0; i < m_vopenNodes.size();i++){
	    if(m_vopenNodes[i]->getF()<lowest){
	        lowest = m_vopenNodes[i]->getF();
	    }
	}
	return lowest;
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


void C_Path::calcG(size_t x_grid, size_t y_grid){
    C_Node * current = &m_vgridNode[x_grid][y_grid];
    cout << "calcG for :" << x_grid << ":" << y_grid << endl;
    current->setOpen(false);
    int currentG = current->getG();
    for(int y = y_grid - 1; y <= y_grid + 1; y++){ //test around current
        for(int x = x_grid - 1; x <= x_grid + 1; x++){
            if(x >=0 && x <= m_vgridNode.size() && y >=0 && y <= m_vgridNode.size()){//do not test outside the gridNode
                if((x != x_grid || y != y_grid)){ //do not cacl the current
                    C_Node *tested = &m_vgridNode[x][y];
                        if(tested != nullptr){
                            if(tested->getBlock() == false){
                                int G_offset = tested->calcG_offset(x_grid,y_grid,x,y);
                                int tmp_G = tested->getG();
                                    if(tmp_G == 0 || (currentG + G_offset) < tmp_G){
                                        tested->setG(currentG + G_offset);
                                        if(tested->getOpen()){
                                            cout << x <<":"<< y << endl;
                                            m_vopenNodes.push_back(tested);
                                            tested->setOpen(false);
                                            tested->setParent(current);
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
