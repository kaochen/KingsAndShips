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



#include "zlayer.h"
#include "../locator.h"

using namespace std;


C_ZLayer::C_ZLayer(int x_grid, int y_grid):
	m_coord(C_CoordGrid(x_grid,y_grid)),
	m_field(nullptr),
	m_grave(nullptr),
	m_ground(nullptr),
	m_cloud(nullptr)
{
	//cliStatus();
}

C_ZLayer::~C_ZLayer()
{
	delete m_field;
	delete m_grave;
	delete m_ground;
	delete m_cloud;
}

void C_ZLayer::set(int layer, C_GameUnits * unit)
{

	switch(layer) {
	case GROUND :
		m_ground = unit;
		break;
	case GRAVEYARD :
		m_grave = unit;
		break;
	case  FIELD:
		m_field = unit;
		break;
	case  CLOUD:
		m_cloud = unit;
		break;
	}
}


C_GameUnits* C_ZLayer::get(int layer)
{
	C_GameUnits * unit = nullptr;
	switch(layer) {
	case GROUND :
		if(m_ground != nullptr)
			unit = m_ground;
		break;
	case GRAVEYARD :
		if(m_grave != nullptr)
			unit = m_grave;
		break;
	case  FIELD:
		if(m_field != nullptr)
			unit = m_field;
		break;
	case  CLOUD:
		if(m_cloud != nullptr)
			unit = m_cloud;
		break;
	}

	return unit;
}

void C_ZLayer::del(int layer)
{
	switch(layer) {
	case GROUND :
		if(m_ground != nullptr) {
			delete m_ground;
		}
		m_ground = nullptr;
		break;
	case GRAVEYARD :
		if(m_grave != nullptr) {
			delete m_grave;
		}
		m_grave = nullptr;
		break;
	case  FIELD:
		if(m_grave != nullptr) {
			delete m_field;
		}
		m_field = nullptr;
		break;
	case CLOUD :
		if(m_cloud != nullptr) {
			delete m_cloud;
		}
		m_cloud = nullptr;
		break;
	}
}

void C_ZLayer::delAll()
{
	if(m_ground != nullptr) {
		delete m_ground;
	}
	m_ground = nullptr;
	if(m_grave != nullptr) {
		delete m_grave;
	}
	m_grave = nullptr;
	if(m_field != nullptr) {
		delete m_field;
	}
	m_field = nullptr;
	if(m_cloud != nullptr) {
		delete m_cloud;
	}
	m_cloud = nullptr;

}



void C_ZLayer::cliStatus()
{
	cout << m_coord.getXGrid() << ":"<< m_coord.getYGrid() << " ";
}

bool C_ZLayer::play(int layer)
{
	C_Settings& settings=C_Locator::getSettings();
	bool ret = false;
	if(settings.getPlaying() == PLAY) {
		switch(layer) {
		case GROUND :
			if(m_ground != nullptr) {
				m_ground->play();
				ret = true;
			}
			break;
		case GRAVEYARD :
			if(m_grave != nullptr) {
				//m_grave->play();
				ret = true;
			}
			break;
		case  FIELD:
			if(m_field != nullptr) {
				m_field->play();
				ret = true;
			}
			break;
		case CLOUD :
			if(m_cloud != nullptr) {
				m_cloud->play();
				ret = true;
			}
			break;
		}
	}
	return ret;
}


bool C_ZLayer::render(int layer)
{

	bool ret = false;
	//m_coord.regenScreenCoord();
	if(m_coord.onScreen()) { //check if tile is visible on screen
		switch(layer) {
		case GROUND :
			if(m_ground != nullptr) {
				m_ground->render();
				ret = true;
			}
			break;
		case GRAVEYARD :
			if(m_grave != nullptr) {
				m_grave->render(m_grave->getScreen());
				ret = true;
			}
			break;
		case  FIELD:
			if(m_field != nullptr) {
				m_field->render(m_field->getScreen());
				ret = true;
			}
			break;
		case  WEAPONS:
			if(m_field != nullptr) {
				m_field->renderWeapon();
				ret = true;
			}
			break;
		case  CLOUD:
			if(m_cloud != nullptr) {
                m_cloud->render();
				ret = true;
			}
			break;
		}
	}
	return ret;
}

