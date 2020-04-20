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

#include "node.h"
#include "grid.h"

#include "../settings.h"
#include "../locator.h"
#include "../window.h"
#include "../locator.h"
#include "../textureList.h"
#include "../message.h"

using namespace std;

C_Node::C_Node(const int x_grid,const int y_grid, const bool block):
	m_coord(C_CoordGrid(x_grid,y_grid))
{

	setBlock(block);
	m_G = 0;
	m_H = 0;
	m_F = m_G + m_H;
	m_parent = nullptr;
	m_h_texture_name = "";
	m_g_texture_name = "";
	m_f_texture_name = "";
};

C_Node::~C_Node()
{
	m_parent = nullptr;

	C_TextureList& t= C_Locator::getTextureList();
	if(m_h_texture_name != "")
		t.freeTexture(m_h_texture_name );
	if(m_g_texture_name != "")
		t.freeTexture(m_g_texture_name );
	if(m_f_texture_name != "")
		t.freeTexture(m_f_texture_name );

};


void C_Node::setBlock(bool block)
{
	m_block = block;
	if (m_block)
		m_open = false;
	else
		m_open = true;
}


void C_Node::displayStatus()
{
	ostringstream message;
	message << " Node :" << m_coord.getGrid().x << ":" << m_coord.getGrid().y
			<< " F:" << m_F << " G:" << m_G << " H:" << m_H <<"\n";
	C_Message::printM(message.str());
};

void C_Node::calcH(C_Node* target)
{
	if (m_block == false) {
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
	C_Grid& grid= C_Locator::getGrid();
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


void C_Node::setF(int G)
{
	m_G = G;
	m_F = m_H + m_G;
}

void C_Node::highlight()
{
	C_TextureList& t= C_Locator::getTextureList();
	int x_screen = m_coord.getXScreen ();
	int y_screen = m_coord.getYScreen ();
	t.renderTexture("Menu_details_Ellipse", x_screen,y_screen,CENTER);
}

void C_Node::prepareRender()
{
	int x_screen = m_coord.getXScreen ();
	int y_screen = m_coord.getYScreen ();
	SDL_Color color = {0,0,0,255};
	C_TextureList& t= C_Locator::getTextureList();

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

	C_Settings& settings=C_Locator::getSettings();
	y_screen += settings.getTileHeight(); //FIXME
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(m_h_texture_name,x_screen - 20,y_screen + 12);
	t.renderTexture(m_g_texture_name,x_screen + 20,y_screen + 12);
	t.renderTexture(m_f_texture_name,x_screen,y_screen + 25);

}

