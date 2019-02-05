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

#include "grid.h"
#include "boat.h"
#include "towers.h"
#include "town.h"
#include "landscape.h"

#include "../texture.h"
#include "../message.h"
#include "../menu/menu.h"


using namespace std;

C_Grid C_Grid::m_instance=C_Grid();

C_Grid::C_Grid()
{
	cout << "Create Grid" << endl;
	//m_factory = C_UnitFactory();
}

C_Grid::~C_Grid()
{
	m_vgrid.clear();
}

void C_Grid::reset(int size)
{
	for (size_t y = 0; y < m_vgrid.size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			m_vgrid[x][y].delAll();
		}
	}
	createAnEmptyGrid(size);
	m_factory = C_UnitFactory();
}

void C_Grid::createAnEmptyGrid(int size)
{
	m_size = size + 2;
	m_vgrid.clear();
	for (int x = 0; x < m_size; x++) {
		vector <C_ZLayer> line;
		for (int y = 0; y < m_size; y++) {
			C_ZLayer z(x,y);
			line.push_back(z);
		}
		m_vgrid.push_back(line);
		cout << endl;
	}

	C_Message m;
	ostringstream message;
	message << "Construct Grid " << m_vgrid.size() << "x" << m_vgrid.size() << endl;
	m.printM(message.str());
}


void C_Grid::renderLayer(int layer)
{
	//int c = 0;
	int size = (int)m_vgrid.size();
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			if(x >= 0 && x < size && y >= 0 && y < size) {
				//cout << "|" << x << ":"<< y;
				if(m_vgrid[x][y].render(layer)) {
					//c++;
				};

			}
		}
	}
	//cout << "render: "<< c <<"tiles"<< endl;
}


void C_Grid::addANewBoat(S_Unit boat)
{
	if (waterway(boat.coord.x,boat.coord.y)) {
		C_GameUnits *tmp = m_factory.create(boat);
		if(tmp != nullptr) {
			m_vgrid[boat.coord.x][boat.coord.y].set(FIELD,tmp);
		}

	} else {
		C_Message m;
		m.printM("You should place the boat into the water\n");
	}
}

int C_Grid::addUnit(string &type, int x_grid, int y_grid)
{
	int success = EXIT_FAILURE;
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		if (m_vgrid[x_grid][y_grid].get(FIELD) == nullptr) {
			if(type == "AddTower") {
				if (!waterway(x_grid,y_grid)) {
					S_Unit tower;
					tower.name = "ArcherTower_0";
					tower.coord = {x_grid,y_grid};
					C_GameUnits *tmp = m_factory.create(tower);
					if(tmp != nullptr) {
						m_vgrid[x_grid][y_grid].set(FIELD,tmp);
					}
					success = EXIT_SUCCESS;
				}
			} else if(type == "AddTurbine") {
				if (!waterway(x_grid,y_grid)) {
					S_Unit turbine;
					turbine.name = "Turbine_0";
					turbine.coord = {x_grid,y_grid};
					C_GameUnits *tmp = m_factory.create(turbine);
					if(tmp != nullptr) {
						m_vgrid[x_grid][y_grid].set(FIELD,tmp);
					}
					success = EXIT_SUCCESS;
				}
			} else if(type == "AddBarricade") {
				if (waterway(x_grid,y_grid)) {
					S_Unit unit;
					unit.name = "barricade_1";
					unit.coord = {x_grid,y_grid};
					C_GameUnits *tmp = m_factory.create(unit);
					if(tmp != nullptr) {
						m_vgrid[x_grid][y_grid].set(FIELD,tmp);
					}
					success = EXIT_SUCCESS;
				}
			}
		}
	}
	return success;
}


void C_Grid::moveUnit(int x_from, int y_from, int x_dest, int y_dest)
{
	if(x_from == x_dest && y_from == y_dest) {}
	else {
		if(x_dest >= 0 && x_dest < (int)(m_vgrid.size()) && y_dest >= 0 && y_dest < (int)(m_vgrid.size())) {
			if(m_vgrid[x_from][y_from].get(FIELD) != nullptr) {
				m_vgrid[x_dest][y_dest].set(FIELD,m_vgrid[x_from][y_from].get(FIELD));
				m_vgrid[x_from][y_from].set(FIELD,nullptr);
			}
		}
	}
}


void C_Grid::setGround(int x, int y, int id)
{
	C_TextureList& t=C_TextureList::Instances();
	if(id !=0) {
		string str = t.getNameFromID(id);
		m_vgrid[x][y].set(GROUND,new C_Ground(str,x,y));
	}
}

void C_Grid::setDecors(int x, int y, int id)
{
	C_TextureList& t=C_TextureList::Instances();
	string rocks = "rocks";
	string trees = "trees";
	string town = "town";
	if(id !=0) {
		string str = t.getNameFromID(id);
		if(str.find(town) != std::string::npos) {
			setTown(x,y);
		} else if(str.find(rocks) != std::string::npos) {
			m_vgrid[x][y].set(FIELD,new C_Decors(str,x,y));
		} else if(str.find(trees) != std::string::npos) {
			m_vgrid[x][y].set(FIELD,new C_Trees(str,x,y));
		}
	}
}

bool C_Grid::waterway(int x_grid, int y_grid)
{
	bool waterway = false;
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		if(m_vgrid[x_grid][y_grid].get(GROUND) != nullptr) {
			string str = m_vgrid[x_grid][y_grid].get(GROUND)->getName();
			if(str.find("Water") != std::string::npos)
				waterway = true;
			else
				waterway = false;
		}
	}
	return waterway;
}

bool C_Grid::testBarricade(int x_grid, int y_grid)
{
	bool barricade = false;
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		if(m_vgrid[x_grid][y_grid].get(FIELD) != nullptr) {
			string str = m_vgrid[x_grid][y_grid].get(FIELD)->getType();
			if(str == "barricade" )
				barricade = true;
			else
				barricade = false;
		}
	}
	return barricade;
}

bool C_Grid::isThisConstructible(S_Coord grid)
{
	if(grid.x >= 0 && grid.x < (int)(m_vgrid.size()) && grid.y >= 0 && grid.y < (int)(m_vgrid.size())) {
		if ( waterway(grid.x, grid.y)) {
			return false;
		} else if(m_vgrid[grid.x][grid.y].get(FIELD)!= nullptr) {
			return false;
		} else {
			return true ;
		}
	} else {
		return false;
	}
}

bool C_Grid::isThisConstructible(int x_grid,int y_grid)
{
	S_Coord tmp;
	tmp.x = x_grid;
	tmp.y = y_grid;
	return isThisConstructible(tmp);
}


void C_Grid::moveToDead(int x_grid, int y_grid)
{
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		m_vgrid[x_grid][y_grid].set(DEAD,m_vgrid[x_grid][y_grid].get(FIELD));
		m_vgrid[x_grid][y_grid].set(FIELD,nullptr);
	} else {
		C_Message m;
		m.printM("moveTodead outside the grid");
	}
}

C_GameUnits* C_Grid::getUnits(int x_grid, int y_grid)
{
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size()))
		return m_vgrid[x_grid][y_grid].get(FIELD);
	else
		return nullptr;
};


void C_Grid::displayStatus()
{
	for (size_t y = 0; y < m_vgrid.size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			if (m_vgrid[x][y].get(FIELD) != nullptr) {
				m_vgrid[x][y].get(FIELD)->displayStatus();
			}
		}
	}
}

void C_Grid::playAllUnits(int layer)
{
	//int c = 0;
	int size = (int)m_vgrid.size();
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			if(x >= 0 && x < size && y >= 0 && y < size) {
				//cout << "|" << x << ":"<< y;
				if(m_vgrid[x][y].play(layer)) {
					//c++;
				};

			}
		}
	}
	//cout << "render: "<< c <<"tiles"<< endl;
}


void C_Grid::deleteGrid()
{
	for (size_t y = 0; y < m_vgrid[0].size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			m_vgrid[x][y].delAll();
		}
	}
}


bool C_Grid::selectATower(C_Coord clic)
{
	S_Coord grid = clic.getGrid();
	bool selected = false, top = false, bottom = false;
	C_Settings& settings=C_Settings::Instances();
	C_Message m;
	string message ="";
	string type ="";
	unselectedAll();
	if(grid.x >= 0 && grid.x < (int)(m_vgrid.size()-1) && grid.y >= 0 && grid.y < (int)(m_vgrid.size()-1)) {
		if (m_vgrid[grid.x+1][grid.y+1].get(FIELD) != nullptr) {
			type = m_vgrid[grid.x+1][grid.y+1].get(FIELD)->getType();
			if(type == "ArcherTower" || type == "Turbine") {
				bottom = true;
			}
		}
		if (m_vgrid[grid.x][grid.y].get(FIELD) != nullptr) {
			type = m_vgrid[grid.x][grid.y].get(FIELD)->getType();
			if(type == "ArcherTower"|| type == "Turbine") {
				top = true;
			}
		}

		if(bottom && !top){
			message =  "Found bottom " + type;
			m_vgrid[grid.x+1][grid.y+1].get(FIELD)->reverseSelectedStatus();
			selected = true;
		} else if(bottom && top){
			message =  "Found bottom " + type;
			m_vgrid[grid.x+1][grid.y+1].get(FIELD)->reverseSelectedStatus();
			selected = true;
		} else if(!bottom && top){
			message =  "Found top " + type;
			m_vgrid[grid.x][grid.y].get(FIELD)->reverseSelectedStatus();
			selected = true;
		} else {
			message = "Nothing";
			selected = false;
		}
	} else {
		message = "Nothing";
		selected = false;
	}

	m.printDebug(message + " at " + to_string(grid.x) + ":" + to_string(grid.y));
	if(settings.getDebugMode())
		clic.displayStatus();
	return selected;
}


void C_Grid::unselectedAll(){
	for (size_t y = 0; y < m_vgrid[0].size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			if ( m_vgrid[x][y].get(FIELD) != nullptr)
				m_vgrid[x][y].get(FIELD)->setSelectedStatus(false);
		}
	}
}

C_GameUnits* C_Grid::getSelected(){
    C_GameUnits * ret = nullptr;
	for (size_t y = 0; y < m_vgrid[0].size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			if ( m_vgrid[x][y].get(FIELD) != nullptr)
				if(m_vgrid[x][y].get(FIELD)->getSelectedStatus()){
				    C_GameUnits *check = m_vgrid[x][y].get(FIELD);
				    if(check != nullptr){
				    	ret = check;
				    }
				}
		}
	}
	return ret;
}


bool C_Grid::mainEmpty(int x_grid, int y_grid, C_GameUnits *current)
{
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		if(m_vgrid[x_grid][y_grid].get(FIELD) == nullptr || m_vgrid[x_grid][y_grid].get(FIELD) == current ) {
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
}
bool C_Grid::mainEmpty(int x_grid, int y_grid)
{
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		if(m_vgrid[x_grid][y_grid].get(FIELD) == nullptr) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

string C_Grid::getUnitType(int layer, int x_grid, int y_grid)
{
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		if(m_vgrid[x_grid][y_grid].get(layer) != nullptr)
			return m_vgrid[x_grid][y_grid].get(FIELD)->getType();
		else
			return "nothing";
	} else {
		return "outsideGrid";
	}
}



void C_Grid::setTown(int x_grid, int y_grid)
{
	//first reset
	for (size_t x = 0; x < m_vgrid.size(); x++) {
		for(size_t y = 0; y < m_vgrid.size(); y++) {
			if(m_vgrid[x][y].get(FIELD) != nullptr) {
				if(m_vgrid[x][y].get(FIELD)->getName() == "town") {
					m_vgrid[x][y].del(FIELD);
				}
			}
		}
	}
	//then set
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		S_Unit town;
		town.name = "town_1";
		town.coord = {x_grid,y_grid};
		C_GameUnits *tmp = m_factory.create(town);
		C_GameUnits *empty1 = new C_EmptyUnit();
		C_GameUnits *empty2 = new C_EmptyUnit();
		C_GameUnits *empty3 = new C_EmptyUnit();
		if(tmp != nullptr) {
			//town use 4 tiles
			m_vgrid[x_grid][y_grid].set(FIELD,tmp);
			m_vgrid[x_grid-1][y_grid].set(FIELD,empty1);
			m_vgrid[x_grid][y_grid-1].set(FIELD,empty2);
			m_vgrid[x_grid-1][y_grid-1].set(FIELD,empty3);
		}
	} else {
		C_Message m;
		m.printM("Set Town outside the grid");
	}
}

S_Coord C_Grid::foundTown()
{
	//FIXME found only one town, the closest one would be better
	S_Coord coord = {0,0};
	for (size_t x = 0; x < m_vgrid.size(); x++) {
		for (size_t y = 0; y < m_vgrid[0].size(); y++) {
			if(m_vgrid[x][y].get(FIELD) != nullptr) {
				string str = m_vgrid[x][y].get(FIELD)->getName();
				if(str.find("town") != std::string::npos) {
					coord.x = x;
					coord.y = y;
				}
			}
		}
	}
	return coord;
}


int C_Grid::getAllTownsLifeLevel()
{
	int life = 0;
	int c = 0;
	for (size_t y = 0; y < m_vgrid.size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			if(m_vgrid[x][y].get(FIELD) != nullptr) {
				string str = m_vgrid[x][y].get(FIELD)->getName();
				if(str.find("town") != std::string::npos) {
					life +=  m_vgrid[x][y].get(FIELD)->getHealth();
					c++;
				}
			}
		}
	}
	if (life !=0)
		return life/c;
	else
		return 0;
}
