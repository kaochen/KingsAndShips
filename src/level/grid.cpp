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
#include "../menu.h"


using namespace std;

C_Grid C_Grid::m_instance=C_Grid();

C_Grid::C_Grid()
{

        C_Message m;
        ostringstream message;
        message << "Construct Grid " << GRID_SIZE << "x" << GRID_SIZE << endl;
        m.printM(message.str());

	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		m_grid[x][y].main = nullptr;
		m_grid[x][y].dead = nullptr;
		m_grid[x][y].ground = nullptr;
		}
	}

	for (int y = 0; y < GRID_SIZE; y++){
	    vector <C_ZLayer> line;
		for (int x = 0; x < GRID_SIZE; x++){
		    C_ZLayer z(x,y);
            line.push_back(z);
		}
		m_vgrid.push_back(line);
		cout << endl;
	}
}

C_Grid::~C_Grid()
{
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
	            delete m_grid[x][y].main;
		        delete m_grid[x][y].dead;
		        delete m_grid[x][y].ground;
		}
	}

}

void C_Grid::reset()
{
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
		    if(m_grid[x][y].main != nullptr){
		            delete m_grid[x][y].main;
		            m_grid[x][y].main = nullptr;
		    }
		    if(m_grid[x][y].dead != nullptr){
		            delete m_grid[x][y].dead;
		            m_grid[x][y].dead = nullptr;
		    }
		    if(m_grid[x][y].ground != nullptr){
		            delete m_grid[x][y].ground;
		            m_grid[x][y].ground = nullptr;
		    }
		}
	}
}


void C_Grid::renderLayer(int layer){
	C_Settings& settings=C_Settings::Instances();

	int x_start = 0, x_end = x_start + settings.getGridWidth() + 4;
	int y_start = 13, y_end = y_start + settings.getGridHeight() + 3;

	//cout << "Line ";
	for (int lineNbr = y_start; lineNbr < y_end; lineNbr++){
		int x = x_start;
		int y = y_start;
	for (int rowNbr = x_start; rowNbr < x_end; rowNbr++){
				//cout << "|" << x << ":"<< y << ":";
				if (layer == GROUND){
				        if (m_grid[x][y].ground != nullptr){
							        m_grid[x][y].ground->render();
							        }
						}
						//draw the deads
				if (layer == GRAVEYARD){
						if (m_grid[x][y].dead != nullptr){
							m_grid[x][y].dead->render(m_grid[x][y].dead->getScreen());
							}
						}
				if (layer == UNITS){
						if (m_grid[x][y].main != nullptr){
								m_grid[x][y].main->render(m_grid[x][y].main->getScreen());
							}
						}
				x++;
				y--;
				}
		if (lineNbr%2 == 0)
			x_start++;
		else
			y_start++;

		//cout << endl;
	}
}


void C_Grid::addANewBoat(int x, int y, int rank,C_Wave* parent){
	if (waterway(x,y)){
		m_grid[x][y].main = new C_Boat(x,y,rank,parent);
	}
	else{
	    C_Message m;
	    m.printM("You should place the boat into the water\n");
	}
}

void C_Grid::addANewTower(int type, int x, int y, int rank){
	if (m_grid[x][y].main == nullptr && !waterway(x,y)){
		switch(type){
		 case ADDNEWTOWER :
			m_grid[x][y].main = new C_ArcherTower(x,y,rank);
		 break;
		 case ADDNEWTURBINE :
			m_grid[x][y].main = new C_Turbine(x,y,rank);
		 break;
		 case  NONE:
		 break;
		 }
	}
}

void C_Grid::moveUnit(int x_from, int y_from, int x_dest, int y_dest){
if(x_from == x_dest && y_from == y_dest){
	}
else{
	if (m_grid[x_from][y_from].main != nullptr){
		m_grid[x_dest][y_dest].main = m_grid[x_from][y_from].main;
		m_grid[x_from][y_from].main = nullptr;
	}
    }
}


void C_Grid::setGround(int x, int y, int id){
	C_TextureList& t=C_TextureList::Instances();
	if(id !=0){
	    string str = t.getNameFromID(id);
	    m_grid[x][y].ground = new C_Ground(str,x,y);
	}
}

void C_Grid::setDecors(int x, int y, int id){
	C_TextureList& t=C_TextureList::Instances();
	string rocks = "rocks";
	string trees = "trees";
	string town = "town";
	if(id !=0){
	    string str = t.getNameFromID(id);
	    if(str.find(town) != std::string::npos){
	        setTown(x,y);
	    }
	    else if(str.find(rocks) != std::string::npos){
	        m_grid[x][y].main = new C_Decors(str,x,y);
	    }
	    else if(str.find(trees) != std::string::npos){
	        m_grid[x][y].main = new C_Trees(str,x,y);
	    }
	}
}

bool C_Grid::waterway(int x_grid, int y_grid)
{
    bool waterway = false;
    if(m_grid[x_grid][y_grid].ground != nullptr){
        string str = m_grid[x_grid][y_grid].ground->getName();
        if(str.find("Water") != std::string::npos)
	        waterway = true;
	    else
	        waterway = false;
	}
	return waterway;
}

bool C_Grid::isThisConstructible(S_Coord grid){
	if ( waterway(grid.x, grid.y)){
		return false;
		}
	else if(m_grid[grid.x][grid.y].main != nullptr){
		return false;
	}
	else{
		return true ;
		}
}

bool C_Grid::isThisConstructible(int x_grid,int y_grid){
	S_Coord tmp;
	tmp.x = x_grid;
	tmp.y = y_grid;
	return isThisConstructible(tmp);
}


void C_Grid::moveToDead(int x_grid, int y_grid){
	m_grid[x_grid][y_grid].dead = m_grid[x_grid][y_grid].main;
	m_grid[x_grid][y_grid].main = nullptr;
	}



void C_Grid::displayStatus(){
	for (size_t y = 0; y < GRID_SIZE; y++){
		for (size_t x = 0; x < GRID_SIZE; x++){
			if (m_grid[x][y].main != nullptr){
					m_grid[x][y].main->displayStatus();
				}
			}
		}
}

void C_Grid::playAllUnits(){
	for (size_t y = 0; y < GRID_SIZE; y++){
			for (size_t x = 0; x < GRID_SIZE; x++){
				if (m_grid[x][y].main != nullptr){
					m_grid[x][y].main->play();
					}
			}
		}
}


void C_Grid::deleteGrid(){
	for (size_t y = 0; y < GRID_SIZE; y++){
			for (size_t x = 0; x < GRID_SIZE; x++){
				if (m_grid[x][y].main != nullptr){
					delete m_grid[x][y].main;
					m_grid[x][y].main = nullptr;
					}
				 if (m_grid[x][y].dead != nullptr){
					delete m_grid[x][y].dead;
					m_grid[x][y].dead = nullptr;
					}
			}
		}
}



bool C_Grid::selectATower(C_Coord clic){
	S_Coord grid = clic.getGrid();
	bool selected = false;
	C_Settings& settings=C_Settings::Instances();
	C_Message m;
	string message ="";
	string name ="";
	if (m_grid[grid.x+1][grid.y+1].main != nullptr){
		unselectedAll(grid.x+1,grid.x+1);
		name = m_grid[grid.x+1][grid.y+1].main->getName();
			if(name == "ArcherTower" || name == "Turbine"){
				message =  "Found top " + name;
				m_grid[grid.x+1][grid.y+1].main->reverseSelectedStatus();
				selected = true;
			}
			else{
			    message = "Found " + name;
			}
	}
	else{
		if (m_grid[grid.x][grid.y].main != nullptr){
			name = m_grid[grid.x][grid.y].main->getName();
				unselectedAll(grid.x,grid.y);
				if(name == "ArcherTower"|| name == "Turbine"){
				    message =  "Found bottom" + name;
					m_grid[grid.x][grid.y].main->reverseSelectedStatus();
					selected = true;
				}
				else{
				    message = "Found " + name;
				}
		}
		else{
			unselectedAll(grid.x,grid.y);
			message = "Nothing";
		}

	}
	m.printDebug(message + " at " + to_string(grid.x) + ":" + to_string(grid.y));
	if(settings.getDebugMode())
		    clic.displayStatus();
	return selected;
}

void C_Grid::unselectedAll(int x_grid, int y_grid){
	bool status = false;
	//backup status
	if ( m_grid[x_grid][y_grid].main != nullptr){
		status = m_grid[x_grid][y_grid].main->getSelectedStatus();
		}
		//erase all
		for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
				if ( m_grid[x][y].main != nullptr)
					m_grid[x][y].main->setSelectedStatus(false);
			}
		}
	//restore status
	if ( m_grid[x_grid][y_grid].main != nullptr){
		m_grid[x_grid][y_grid].main->setSelectedStatus(status);
		}


}


C_GameUnits* C_Grid::getSelectedUnit(){
		C_GameUnits* current = nullptr;
		for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
				if ( m_grid[x][y].main != nullptr)
					if (m_grid[x][y].main->getSelectedStatus())
						current = m_grid[x][y].main->getUnit();
			}
		}
		return current;
}

bool C_Grid::mainEmpty(int x_grid, int y_grid, C_GameUnits *current){
    if(m_grid[x_grid][y_grid].main == nullptr || m_grid[x_grid][y_grid].main == current ){
        return false;
    }
    else{
        return true;
    }
};
bool C_Grid::mainEmpty(int x_grid, int y_grid){
    if(m_grid[x_grid][y_grid].main == nullptr){
        return true;
    }
    else{
        return false;
    }
};

bool C_Grid::boatInMain(int x_grid, int y_grid){
    if(m_grid[x_grid][y_grid].main != nullptr){
        if(m_grid[x_grid][y_grid].main->getName() == "boat"){
           // cout << "found boat on " << x_grid << ":"<< y_grid << endl;
            return true;
            }
            else{
            return false;
            }
    }
    else{
        return false;
    }
};



void C_Grid::setTown(int x_grid, int y_grid){
    //first reset
    for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
				 if(m_grid[x][y].main != nullptr){
				     if(m_grid[x][y].main->getName() == "town"){
				        delete m_grid[x][y].main;
				        m_grid[x][y].main = nullptr;
				        }
				    }
			}
		}
	//then set
	 m_grid[x_grid][y_grid].main = new C_Town(x_grid,y_grid);
}

S_Coord C_Grid::foundTown(){
        //FIXME found only one town, the closest one would be better
        S_Coord coord = {0,0};
		for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
			    if(m_grid[x][y].main != nullptr){
                    string str = m_grid[x][y].main->getName();
                    if(str.find("town") != std::string::npos){
				        coord.x = x;
				        coord.y = y;
				        }
			    }
			}
		}
		return coord;
}


int C_Grid::getAllTownsLifeLevel(){
        int life = 0;
        int c = 0;
        for (int y = 0; y < GRID_SIZE; y++){
			for (int x = 0; x < GRID_SIZE; x++){
		        if(m_grid[x][y].main != nullptr){
                    string str = m_grid[x][y].main->getName();
                    if(str.find("town") != std::string::npos){
				        life +=  m_grid[x][y].main->getLife();
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
