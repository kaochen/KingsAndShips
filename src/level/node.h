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

//Node are used to store information for the pathfinding A* like G,H,and F costs

#ifndef NODE_H
#define NODE_H
#include <map>

#include "../coord.h"

#define G_HV 10
#define G_DIAG 14

class C_Node {
public:
	C_Node(const int x_grid,const int y_grid, const bool block);
	~C_Node();

	void setTown(bool town);
	bool getTown() const;
	void setBlock(bool block);
	bool getBlock() const;
	bool getBarricade()
	{
		return m_barricade;
	};
	void setBarricade(bool barricade )
	{
		m_barricade = barricade;
	};
	int getXGrid() const;
	int getYGrid() const;
	void setParent(C_Node * parent);
	C_Node* getParent();
	void setChild(C_Node * child);
	C_Node* getChild();
	void displayStatus();

	void calcH(const C_Node* target);
	int getG() const;
	int getH() const;
	int getF() const;
	C_Coord* getCoord() const;
	bool getOpen() const;
	void setOpen(bool open);
	void setF(int G);
	void setDist(int dist, double angle);
	int getDist() const;
	double getAngle() const;
	void highlight();
	void prepareRender();
	void render();
	bool screenAtCenter();
	void regenScreenCoord();
	int calcG_offset(int x_from, int y_from,int x_dest, int y_dest);
protected:
	C_Coord * m_coord;
	bool m_block; /*!< If Unit, that do not move, can not be cross by a boat like a ground*/
	bool m_barricade; /*!< If Unit, that do not move, can be cross after destruction like a barricade*/
	int m_G; // G cost (distance from the starting node)
	int m_H; //H cost heuristic (distance from end to end)
	int m_F; //G cost + H Cost
	bool m_Town;
	bool m_open;
	C_Node* m_parent;
	double m_angle;
	int m_dist;
	//
	std::string m_g_texture_name;
	std::string m_h_texture_name;
	std::string m_f_texture_name;
};

#endif
