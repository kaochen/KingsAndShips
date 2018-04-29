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


#include "invaders.h"
#include "time.h"
#include "grid.h"

#include "window.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank):C_Shooter("boat", x_grid, y_grid ,rank)
{

	m_weapon = new C_Weapon("BOAT",2,0,2000,2);
	m_moving = false;
	m_speed = SLOW;
	m_speedImpact = 0;
	m_speedImpactLoop = 10;
	m_coord->centerOnTile();
	//Find a way to town
	C_Grid& grid=C_Grid::Instances();
	S_Coord town = grid.foundTown();
	m_C_Path = new C_Path(town.x,town.y);
	m_C_Path->calcPath(x_grid,y_grid,town.x,town.y);
	m_C_Path->showPath();
	m_y_center_offset = 18;
}

C_invaders::~C_invaders()
{
}

void C_invaders::play(){
	this->move();
	string list[3] = {"town","ArcherTower","Turbine"};
	this->shoot(list, 3);

	if(!this->alive())
		this->kill();
};


void C_invaders::move()
{
	m_moving = true;

	C_Grid& grid=C_Grid::Instances();
	S_Coord town =  grid.foundTown();
	std::stack<C_Node*> path;
	path = m_C_Path->getPath();
	if(!m_C_Path->closeToDestination(m_coord->getXGrid(),m_coord->getYGrid())){
       if(path.size() > 0){

		    //determine an angle
		    *m_old_coord = *m_coord;
		    int old_x_grid = m_coord->getXGrid();
		    int old_y_grid = m_coord->getYGrid();
		    C_Coord destCoord = *path.top()->getCoord();
		    destCoord.centerOnTile();
		    S_Coord start = m_coord->getScreen();
		    S_Coord dest = destCoord.getScreen();
		    int ab = dest.x - start.x;
		    int bc = dest.y - start.y;
		    double angle = atan2(ab,bc);

            int speed = m_speed - m_speedImpact;
            if (speed < 0){
                    speed = VERY_SLOW;
                    }
            //cout << "speed" << speed << "=" << m_speed << "-" << m_speedImpact << endl;
            if (m_speedImpactLoop > 0){
                m_speedImpactLoop--;
            }
            else{
                m_speedImpact = 0;
                m_speedImpactLoop=10;
                }
		    //move following angle and speed
		    C_Coord tmp = *m_coord;
		    tmp.move(angle,speed);
		    tmp.regenGridCoord();
		    bool nextEmpty = grid.mainEmpty(tmp.getXGrid(),tmp.getYGrid(),this);
            if(!nextEmpty){
		        m_coord->move(angle,speed);
		        m_countStop = 0;

		        angle = 180 - (angle *180/3.14159265359);
			    if(angle < 0)
				    angle +=360;

		        m_direction = destCoord.angleToDirection(angle);

		        //apply offset + offset
		        //refresh grid position
		        m_coord->regenGridCoord();

		        grid.moveUnit(old_x_grid, old_y_grid,  m_coord->getXGrid (), m_coord->getYGrid ());
			        if(*m_coord == destCoord){
				        m_C_Path->goNextStep();
			        }
		    }
		    else{
		        m_countStop++;
		        if (m_countStop > 10){
		            cout << "OldCoord" << old_x_grid <<":"<< old_y_grid << endl;
		            delete m_C_Path;
		            m_C_Path = new C_Path(town.x,town.y);
	                m_C_Path->calcPath(old_x_grid,old_y_grid,town.x,town.y);
		        	m_C_Path->showPath();
		        }
		    }
      }
	}

	if(!m_C_Path->closeToDestination(m_coord->getXGrid(),m_coord->getYGrid())){
	   int	pauseNbr = m_animation[PAUSESEARCHPATH]->getAnimNbr(1,2,600);
       if(path.size() == 0 && pauseNbr == 2){
		            delete m_C_Path;
		            m_C_Path = new C_Path(town.x,town.y);
	                m_C_Path->calcPath(m_coord->getXGrid(),m_coord->getYGrid(),town.x,town.y);
		        	m_C_Path->showPath();
       }
	}

}



void C_invaders::updateDirection(){
	std::stack<C_Node*> path;
	path = m_C_Path->getPath();
	int x_grid = m_coord->getXGrid ();
	int y_grid = m_coord->getYGrid ();
	int next_x_grid = path.top()->getXGrid();
	int next_y_grid = path.top()->getYGrid();
	cout << x_grid << ":" << y_grid << "->" << next_x_grid << ":" << next_y_grid << endl;
//enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
	if(next_x_grid == x_grid && next_y_grid == y_grid){
		m_C_Path->goNextStep();
		}
	else if(next_x_grid > x_grid && next_y_grid == y_grid)
		m_direction = EAST;
	else if(next_x_grid > x_grid && next_y_grid > y_grid)
		m_direction = SOUTH_EAST;
	else if(next_x_grid == x_grid && next_y_grid > y_grid)
		m_direction = SOUTH;
	else if(next_x_grid > x_grid && next_y_grid < y_grid)
		m_direction = NORTH_EAST;
	else if(next_x_grid < x_grid && next_y_grid == y_grid)
		m_direction = WEST;
	else if(next_x_grid < x_grid && next_y_grid < y_grid)
		m_direction = NORTH_WEST;
	else if(next_x_grid == x_grid && next_y_grid < y_grid)
		m_direction = NORTH;
	else if(next_x_grid < x_grid && next_y_grid > y_grid)
		m_direction = SOUTH_WEST;
	else
		m_direction = UNKNOWN;

};




void C_invaders::render(S_Coord screen){

	if (m_weapon->getShooting())
		m_weapon->render();
	renderLifeBar(screen.x, screen.y + m_y_center_offset);

	string name = getName();
	C_TextureList& t=C_TextureList::Instances();
	int imageNbr = 0;
	if (m_moving)
		imageNbr = m_animation[MAIN_ANIM]->getAnimNbr(1,2,300);
	string direction = "_EE_";
		switch(m_direction){
			case SOUTH:
				direction = "_SS_";
			break;
			case SOUTH_EAST:
				direction = "_SE_";
			break;
			case EAST:
				direction = "_EE_";
			break;
			case NORTH_EAST:
				direction = "_NE_";
			break;
			case NORTH:
				direction = "_NN_";
			break;
			case NORTH_WEST:
				direction = "_NW_";
			break;
			case WEST:
				direction = "_WW_";
			break;
			case SOUTH_WEST:
				direction = "_SW_";
			break;
		}

	//boat_01_SW_0
	string fileName = name + "_0" + to_string(m_rank) + direction + to_string(imageNbr) ;

	//cout << "image name is "<< fileName << endl;

	t.renderTexture(fileName, screen.x,screen.y + m_y_center_offset);
	m_C_Path->displayPath();
}


void C_invaders::receiveDamage(S_Weapon weapon)
{
	m_life -=weapon.damage;
	m_speedImpact = weapon.speedImpact;
	if (m_life < 0)
	{
		m_life = 0;
	}
}
