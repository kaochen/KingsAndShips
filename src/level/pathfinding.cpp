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
#include "../locator.h"
#include "../settings.h"
#include "../message.h"

using namespace std;

C_Path::C_Path(int x_dest, int y_dest)
{
	C_Grid& grid= C_Locator::getGrid();
	for (int x = 0; x < grid.getFullSize(); x++) {
		vector <C_Node> line;
		for (int y = 0; y < grid.getFullSize(); y++) {
			bool block = true;
			if (grid.waterway(x,y)) {
				block = false;
			}
			C_Node z(x,y,block);
			if(grid.testBarricade(x,y)) {
				z.setBarricade(true);
				//cout << "Barricade in : " << x << ":" << y << endl;
			}
			line.push_back(z);
		}
		m_vgridNode.push_back(line);
	}

	m_start = nullptr;
	m_destination = nullptr;


	setTown(x_dest,y_dest);
	for (size_t x = 0; x < m_vgridNode.size(); x++) {
		for (size_t y = 0; y < m_vgridNode.size(); y++) {
			m_vgridNode[x][y].calcH(&m_vgridNode[x_dest][y_dest]);
		}
	}
	C_Message::printDebugPath("Construct C_Path done\n");
}
C_Path::~C_Path()
{
}


void C_Path::calcPath(int x_start,int y_start, int x_dest, int y_dest)
{
	m_start = &m_vgridNode[x_start][y_start];
	m_destination = &m_vgridNode[x_dest][y_dest];
	m_vgridNode[x_start][y_start].setBlock(false);
	m_vopenNodes.push_back(m_start);
	m_start->setF(0);

	ostringstream message;

	//cout << "New Path from " << x_start << ":"<< y_start << "to" << x_dest << ":"<< y_dest << endl;
	while (m_vopenNodes.size()>0) {
		int lowestF = findLowestF();
		//cout << "size "<< m_vopenNodes.size() << " ";
		//displayOpenList();
		if(m_vopenNodes[lowestF] != nullptr) {
			//erase current form openList, do no need anymore, clean if town
			calcG_Around(m_vopenNodes[lowestF]);
			m_vopenNodes[lowestF]->setOpen(false);
			m_vopenNodes.erase(m_vopenNodes.begin() + lowestF);
			for(size_t i = 0; i < m_vopenNodes.size(); i++) {
				if(m_vopenNodes[i]!= nullptr) {
					if(m_vopenNodes[i]->getTown()) {
						//cout << "found Town" << endl;
						m_vopenNodes.clear();
						break;
					}
				}
			}
		}
	}


	message << "\n---------\n";
	C_Message::printDebugPath(message.str());

	//if(m_vopenNodes.size()>0){
	loadPath();
	//}
	//else{
	//    m.printM("not path to load\n");
	//}

}

void C_Path::displayOpenList()
{
	C_Message::printDebugPath("Open Nodes: \n");

	for (size_t i = 0; i < m_vopenNodes.size(); i++) {
		if(m_vopenNodes[i])
			m_vopenNodes[i]->displayStatus();
	}

}

size_t C_Path::findLowestF()
{
	int lowest = 10000;
	int c = 0;
	for(size_t i = 0; i < m_vopenNodes.size(); i++) {
		if(m_vopenNodes[i]->getF()<lowest) {
			lowest = m_vopenNodes[i]->getF();
			c = i;
		}
	}

	return c;
}

void C_Path::regenScreenCoord()
{
	int size = m_vgridNode.size();
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			m_vgridNode[x][y].regenScreenCoord();
		}
	}
}


void C_Path::setTown(int x_grid,int y_grid)
{
	//reset
	int size = m_vgridNode.size();
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			m_vgridNode[x][y].setTown(false);
		}
	}
	//set
	m_vgridNode[x_grid][y_grid].setTown(true);
	for(int j = -1; j <= 1; j++) {
		for(int i = -1; i <= 1; i++) {
			if(x_grid+i >= 0 && x_grid+i <= size && y_grid+j >= 0 && y_grid+j <= size) {
				m_vgridNode[x_grid+i][y_grid+j].setBlock(false);
			}
		}
	}
}

void C_Path::loadPath()
{
	C_Node* current = m_destination;
	//clear path before

	while(!m_path.empty()) {
		m_path.pop();
	}
	if(current->getParent() == nullptr) {
		current = closestNode();
	}
	if(current != nullptr) {
		while(current->getParent() != nullptr) {
			m_path.push(current);
			current = current->getParent();
			//cout <<"parent: "<< current->getXGrid() << ":" << current->getYGrid() << endl;
		}
		//if the boat is not already in the center of the tile, add the current tile to the path.
		//m_path.push(current); //force the boat to recenter itself on its current tile.
		C_Message::printDebugPath("path is loaded\n");
		//prepare render for debug
		C_Settings& settings=C_Locator::getSettings();
		if(settings.getDebugPathMode()) {
			for (size_t x = 0; x < m_vgridNode.size(); x++) {
				for (size_t y = 0; y < m_vgridNode.size(); y++) {
					m_vgridNode[x][y].prepareRender ();
				}
			}
		}
	}
	//m_path.push(m_start); //do not forget the start
}

void C_Path::addANodeAtTheStartOfThePath(S_Coord grid)
{
	m_path.push(&m_vgridNode[grid.x][grid.y]);
}


void C_Path::showPath()
{
	C_Settings& settings=C_Locator::getSettings();
	if(settings.getDebugPathMode()) {
		string message ="";
		show_H_G_F();
		stack<C_Node*> tmp = m_path;
		int c =0;
		while(tmp.empty() == false) {
			message += to_string(tmp.top()->getXGrid()) + ":" + to_string(tmp.top()->getYGrid()) + " >> ";
			tmp.pop();
			c++;
		}
		C_Message::printDebugPath(message + "steps: " + to_string(c) + "\n");
	}
}

void C_Path::show_H_G_F()
{
	string message = "";
	for(size_t x= 0; x < m_vgridNode.size(); x++) {
		for(size_t y= 0; y < m_vgridNode.size(); y++) {
			C_Node* c = &m_vgridNode[x][y];
			message += "|" + to_string(c->getF());
		}
		C_Message::printDebugPath(message + "\n");
		message ="";
	}
}

C_Node* C_Path::closestNode()
{
	C_Node *closest = nullptr;
	size_t lowestF = 10000;
	for(size_t x= 0; x < m_vgridNode.size(); x++) {
		for(size_t y= 0; y < m_vgridNode.size(); y++) {
			size_t F = m_vgridNode[x][y].getF();
			if (F > 0 && F < lowestF) {
				closest = &m_vgridNode[x][y];
				lowestF = F;
			}
		}
	}
	size_t lowestH = 10000;
	for(size_t x= 0; x < m_vgridNode.size(); x++) {
		for(size_t y= 0; y < m_vgridNode.size(); y++) {
			size_t H = m_vgridNode[x][y].getH();
			size_t F = m_vgridNode[x][y].getF();
			if(F == lowestF) {
				if (H > 0 && H < lowestH) {
					if(closest != nullptr) {
						closest = &m_vgridNode[x][y];
						lowestH = H;
					}
				}
			}
		}
	}

	return closest;
}

bool C_Path::closeToDestination(int x_grid, int y_grid, int steps)
{
	int x_dist = m_destination->getXGrid() - x_grid;
	int y_dist = m_destination->getYGrid() - y_grid;

	if((x_dist >=-steps && x_dist <= steps)&&(y_dist >=-steps && y_dist <= steps))
		return true;
	else
		return false;
}

void C_Path::displayPath()
{
	C_Settings& settings=C_Locator::getSettings();
	if(settings.getDebugMode()) {
		//highlight the path in front of the boats
		stack<C_Node*> tmp = m_path;
		while(tmp.empty() == false) {
			tmp.top()->highlight();
			tmp.pop();
		}
	}
	if(settings.getDebugPathMode()) {
		//display H G and F numbers for debugging Pathfinding A*

		for (size_t x = 0; x < m_vgridNode.size(); x++) {
			for (size_t y = 0; y < m_vgridNode.size(); y++) {
				m_vgridNode[x][y].render();
			}
		}
	}
}

stack<C_Node*> C_Path::getPath()
{
	return m_path;
}


void C_Path::goNextStep()
{
	if(m_path.size() > 1 ||!m_path.empty())
		m_path.pop();
}

void C_Path::calcG_Around(C_Node *current)
{
	int  x_grid = current->getXGrid();
	int  y_grid = current->getYGrid();
	ostringstream message;
	message << "calcG for " << x_grid << ":" << y_grid << " F: "<< current->getF() << " -Testing : ";
	int size = m_vgridNode.size();

	current->setOpen(false);
	for(int y = y_grid - 1; y <= y_grid + 1; y++) { //test around current
		for(int x = x_grid - 1; x <= x_grid + 1; x++) {
			if(x >=0 && x <= size && y >=0 && y <= size) {//do not test outside the gridNode
				C_Node *tested = &m_vgridNode[x][y];
				if(tested != nullptr) {
					bool corner = crossACorner(x_grid,y_grid,x,y);
					if(!tested->getBlock() && !corner) {
						if(tested->getOpen()) {
							int G_offset = tested->calcG_offset(x_grid,y_grid,x,y);
							int tested_G = tested->getG();
							int currentG = current->getG();
							if(tested_G == 0 || (currentG + G_offset) < tested_G) {
								tested->setF(currentG + G_offset);
								tested->setParent(current);
								m_vopenNodes.push_back(tested);
							}
							message << x <<":"<< y << " F:" << tested->getF() <<" | ";

						}
					}
				}
			}
		}
	}
	message << endl;
	C_Message::printDebugPath(message.str());
}

//Help to turn around a corner
bool C_Path::crossACorner(int x_from, int y_from, int x_dest, int y_dest)
{
	if(m_vgridNode[x_from][y_dest].getBlock()||m_vgridNode[x_dest][y_from].getBlock() )
		return true;
	else
		return false;
}
