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

#include <SDL2_gfxPrimitives.h>
#include <sstream>

#include "node.h"
#include "grid.h"

#include "../settings.h"
#include "../window.h"
#include "../texture.h"
#include "../message.h"

using namespace std;

C_Node::C_Node(const int x_grid,const int y_grid, const bool block):
	m_coord(C_CoordGrid(x_grid,y_grid))
{

	setBlock(block);
	m_barricade = false;
	m_G = 0;
	m_H = 0;
	m_F = m_G + m_H;
	m_Town = false;
	m_parent = nullptr;
	m_angle = 0;
	m_dist = 0;
	m_h_texture_name = "";
	m_g_texture_name = "";
	m_f_texture_name = "";
};

C_Node::~C_Node()
{
	m_parent = nullptr;

	C_TextureList& t=C_TextureList::Instances();
	if(m_h_texture_name != "")
		t.freeTexture(m_h_texture_name );
	if(m_g_texture_name != "")
		t.freeTexture(m_g_texture_name );
	if(m_f_texture_name != "")
		t.freeTexture(m_f_texture_name );

};

void C_Node::setTown(bool town)
{
	m_Town = town;
	if(town)
		m_block = false;

}

bool C_Node::getTown() const
{
	return m_Town;
}

void C_Node::setBlock(bool block)
{
	m_block = block;
	if (m_block)
		m_open = false;
	else
		m_open = true;
}

bool C_Node::getBlock() const
{
	return m_block;
}

bool C_Node::getOpen() const
{
	return m_open;
}

void C_Node::setOpen(bool open)
{
	m_open = open;
}

void C_Node::setParent(C_Node * parent)
{
	m_parent = parent;
}

C_Node* C_Node::getParent()
{
	return m_parent;
}

int C_Node::getXGrid()
{
	return m_coord.getGrid().x;
}

int C_Node::getYGrid()
{
	return m_coord.getGrid().y;
}

void C_Node::displayStatus()
{
	C_Message m;
	ostringstream message;
	message << " Node :" << m_coord.getGrid().x << ":" << m_coord.getGrid().y
			<< " F:" << m_F << " G:" << m_G << " H:" << m_H << " dist:" << m_dist << " angle" << m_angle;
	if (m_Town)
		message << " Town: true <---\n";
	else
		message << " Town: false\n";
	m.printM(message.str());
};

void C_Node::calcH(C_Node* target)
{
	//cout << "town " << target->getXGrid() << ":"<< target->getYGrid() << endl;
	if (m_Town == false && m_block == false) {
		int moveOnX =  target->getXGrid() - m_coord.getGrid().x;
		if (moveOnX < 0)
			moveOnX *= -1;
		int moveOnY =  target->getYGrid() - m_coord.getGrid().y;
		if (moveOnY < 0)
			moveOnY *= -1;
		m_H = (moveOnX + moveOnY) *10;
	}
}


int C_Node::calcG_offset(int x_from, int y_from,
						 int x_dest, int y_dest)
{
	int offset = 0;
	C_Grid& grid=C_Grid::Instances();
	//if diagonal
	if(x_from != x_dest && y_from != y_dest) {
		offset += G_DIAG;

		//if barricade on a corner add a malus
		string corner1 = grid.getUnitType(FIELD,x_dest,y_from);
		string corner2 = grid.getUnitType(FIELD,x_from,y_dest);
		if(corner1 == "barricade" || corner2 == "barricade") {
			offset += 2*G_DIAG;
		}
	} else {
		offset += G_HV;
	}

	//if boat or barricade on destination add malus
	string name = grid.getUnitType(FIELD,x_dest,y_dest);
	if(name == "barricade") {
		offset += 3*G_HV;
	} else if(name == "boat") {
		offset += 2*G_HV;
	}
	return offset;
}


int C_Node::getG() const
{
	return m_G;
}

int C_Node::getH() const
{
	return m_H;
}

int C_Node::getF() const
{
	return m_F;
}

C_Coord C_Node::getCoord() const
{
	return m_coord;
}

void C_Node::setF(int G)
{
	m_G = G;
	m_F = m_H + m_G;
}

void C_Node::setDist(int dist, double angle)
{
	m_dist = dist;
	m_angle = angle;
}

int C_Node::getDist()const
{
	return m_dist;
}

double C_Node::getAngle() const
{
	return m_angle;
}

void C_Node::highlight()
{
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	int R = 200, G = 200, B = 200, A = 100;
	int x_screen = m_coord.getXScreen ();
	int y_screen = m_coord.getYScreen ();
	filledEllipseRGBA(renderer,x_screen, y_screen,10,5,R,G,B,A);
}

void C_Node::prepareRender()
{
	int x_screen = m_coord.getXScreen ();
	int y_screen = m_coord.getYScreen ();
	SDL_Color color = {0,0,0,255};
	C_TextureList& t=C_TextureList::Instances();

	m_h_texture_name = to_string(x_screen)+to_string(y_screen)+"h_value";
	string value = "H:"+to_string(m_H);
	t.loadTextAsTexturesIntoMap(m_h_texture_name, value, 10, color);

	m_g_texture_name = to_string(x_screen)+to_string(y_screen)+"g_value";
	value = "G:"+to_string(m_G);
	t.loadTextAsTexturesIntoMap(m_g_texture_name, value, 10, color);

	m_f_texture_name = to_string(x_screen)+to_string(y_screen)+"f_value";
	value = "F:"+to_string(m_F);
	t.loadTextAsTexturesIntoMap(m_f_texture_name, value, 15, color);
}

void C_Node::render()
{
	int x_screen = m_coord.getXScreen ();
	int y_screen = m_coord.getYScreen ();

	y_screen +=TILE_HALF_HEIGHT*2; //need a fix
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(m_h_texture_name,x_screen - 20,y_screen + 12);
	t.renderTexture(m_g_texture_name,x_screen + 20,y_screen + 12);
	t.renderTexture(m_f_texture_name,x_screen,y_screen + 25);

}

void C_Node::regenScreenCoord()
{
	m_coord.regenScreenCoord();
}

