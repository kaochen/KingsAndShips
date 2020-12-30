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



#include "grid.h"
#include "boat.h"
#include "towers.h"
#include "town.h"
#include "landscape.h"

#include "../locator.h"
#include "../message.h"
#include "../menu/menu.h"


using namespace std;

C_Grid::C_Grid()
{
	C_Message::printM("Constructor C_Grid() : done\n");
}

C_Grid::~C_Grid()
{
	for (size_t y = 0; y < m_vgrid.size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			m_vgrid[x][y].delAll();
		}
	}
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
		//cout << endl;
	}
	addAllClouds();

	C_Message::printM("Construct Grid " + to_string(m_vgrid.size()) + "x" + to_string(m_vgrid.size()) +"\n");
}


void C_Grid::renderLayer(int layer)
{
	//int c = 0;
	int size = (int)m_vgrid.size();
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			if(x >= 0 && x < size && y >= 0 && y < size) {
				//cout << "|" << x << ":"<< y;
				if(m_vgrid[x][y].isCloud() && layer != CLOUD){

				} else {
				    if(m_vgrid[x][y].render(layer)) {
                        //c++;
				    }
				}

			}
		}
	}
	//cout << "render: "<< c <<" tiles for layer :" << layer << endl;
}


void C_Grid::addANewBoat(S_Unit boat)
{
	if (waterway(boat.coord.x,boat.coord.y)) {
		C_GameUnits *tmp = m_factory.create(boat);
		if(tmp != nullptr) {
			m_vgrid[boat.coord.x][boat.coord.y].set(FIELD,tmp);
		}

	} else {
		C_Message::printM("You should place the boat into the water\n");
	}
}

bool C_Grid::addUnit(string &type, S_Coord coord)
{
	bool ret = false;
	bool typeOk = false;

	//on land
	S_Unit unit;
	unit.coord = coord;
	if(type == "AddTower") {
		if(isThisConstructible(coord, false)){
			unit.name = "ArcherTower_0";
			typeOk = true;
		}
	} else if(type == "AddCatapult") {
		if(isThisConstructible(coord, false)){
			unit.name = "Catapult_0";
			typeOk = true;
		}
	} else if(type == "AddBarricade") {
		if(isThisConstructible(coord, true)){
			unit.name = "barricade_1";
			typeOk = true;
		}
	}
	if(typeOk){
		if(addANewTower(unit)){
			ret = true;
		}
	}


	return ret;
}


bool C_Grid::addANewTower(S_Unit unit){
	bool ret = false;
		C_GameUnits *tmp = m_factory.create(unit);
		if(tmp != nullptr) {
			m_vgrid[unit.coord.x][unit.coord.y].set(FIELD,tmp);
			cleanClouds(unit.coord, 3);
			ret = true;
		}
	return ret;
}

bool C_Grid::moveUnit(int x_from, int y_from, int x_dest, int y_dest)
{
	bool ret = false;
	if(x_from == x_dest && y_from == y_dest) {}
	else {
		if(x_from >= 0 && x_from < (int)(m_vgrid.size()) && y_from >= 0 && y_from < (int)(m_vgrid.size())) {
			if(x_dest >= 0 && x_dest < (int)(m_vgrid.size()) && y_dest >= 0 && y_dest < (int)(m_vgrid.size())) {
				if(m_vgrid[x_from][y_from].get(FIELD) != nullptr) {
					m_vgrid[x_dest][y_dest].set(FIELD,m_vgrid[x_from][y_from].get(FIELD));
					m_vgrid[x_from][y_from].set(FIELD,nullptr);
					ret = true;
				}
			}
		}
	}
	return ret;
}


void C_Grid::setGround(int x, int y, std::string name)
{
	if(!name.empty()){
		if(name.find("Ground") != std::string::npos) {
			m_vgrid[x][y].set(GROUND,new C_Ground(name,x,y));
		} else if(name.find("Water") != std::string::npos) {
			m_vgrid[x][y].set(GROUND,new C_Water(name,x,y));
		} else if(name.find("town") != std::string::npos) {
			setTown(x,y);
		} else if(name.find("rocks") != std::string::npos) {
			m_vgrid[x][y].set(FIELD,new C_Decors(name,x,y));
		} else if(name.find("trees") != std::string::npos) {
			m_vgrid[x][y].set(FIELD,new C_Trees(name,x,y));
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


bool C_Grid::isThisConstructible(S_Coord grid, bool water)
{
	bool test = true;
	if(grid.x > 0 && grid.x < getUsefullSize() && grid.y > 0 && grid.y < getUsefullSize()) {
		if(!water){
			if ( waterway(grid.x, grid.y)){
					test = false;
					}
		} else {
			if (!waterway(grid.x, grid.y))
					test = false;
		}

		if(m_vgrid[grid.x][grid.y].get(FIELD)!= nullptr){
			test = false;
			}
		if(m_vgrid[grid.x][grid.y].get(CLOUD)!= nullptr){
			test = false;
			}
	} else {
		test = false;
	}
	return test;
}


void C_Grid::moveToDead(int x_grid, int y_grid)
{
	if(x_grid >= 0 && x_grid < (int)(m_vgrid.size()) && y_grid >= 0 && y_grid < (int)(m_vgrid.size())) {
		m_vgrid[x_grid][y_grid].set(DEAD,m_vgrid[x_grid][y_grid].get(FIELD));
		m_vgrid[x_grid][y_grid].set(FIELD,nullptr);
	} else {
		C_Message::printM("moveTodead outside the grid");
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

void C_Grid::addAllClouds()
{
	for (size_t y = 0; y < m_vgrid.size(); y++) {
		for (size_t x = 0; x < m_vgrid.size() -1; x++) {
			m_vgrid[x][y].set(CLOUD,new C_Clouds(x,y));
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
	//cout << "Select: " << grid.x << ":" << grid.y << endl;
	bool selected = false, top = false, bottom = false;
	C_Settings& settings=C_Locator::getSettings();
	string message ="";
	string type ="";
	unselectedAll();
	if(grid.x >= 0 && grid.x < (int)(m_vgrid.size()-1) && grid.y >= 0 && grid.y < (int)(m_vgrid.size()-1)) {
		if (m_vgrid[grid.x+1][grid.y+1].get(FIELD) != nullptr) {
			type = m_vgrid[grid.x+1][grid.y+1].get(FIELD)->getType();
			if(type == "ArcherTower" || type == "Catapult") {
				bottom = true;
			}
		}
		if (m_vgrid[grid.x][grid.y].get(FIELD) != nullptr) {
			type = m_vgrid[grid.x][grid.y].get(FIELD)->getType();
			if(type == "ArcherTower"|| type == "Catapult") {
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

	C_Message::printDebug(message + " at " + to_string(grid.x) + ":" + to_string(grid.y));
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
		if(tmp != nullptr) {
			m_vgrid[x_grid][y_grid].set(FIELD,tmp);
		} else {
		    C_Message::printM("Cannot create the Town");
		}
		cleanClouds(town.coord, 6);

	} else {
		C_Message::printM("Set Town outside the grid");
	}
}

void C_Grid::cleanClouds(S_Coord grid, int range){

	for(int y = (grid.y - range); y <= (grid.y + range); y++) {
		for(int x = (grid.x - range); x <= (grid.x + range); x++) {
			if(x >= 0 && x < m_size && y >= 0 && y < m_size) {
				m_vgrid[x][y].del(CLOUD);
			}
		}
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

int C_Grid::nbrOfboatStillAlive()
{
	int count = 0;
	for (size_t y = 0; y < m_vgrid.size(); y++) {
		for (size_t x = 0; x < m_vgrid.size(); x++) {
			if (m_vgrid[x][y].get(FIELD) != nullptr) {
				string type = m_vgrid[x][y].get(FIELD)->getType();
				if(type == "boat"){
					count++;
				}
			}
		}
	}
	return count;
}
