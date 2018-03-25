#include "invaders.h"
#include "time.h"
#include "grid.h"

#include "window.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank):C_Shooter("boat", x_grid, y_grid ,rank)
{

	m_weapon = new C_Weapon("BOAT",10,0,500,2);
	m_moving = false;
	m_speed = NORMAL;
	m_speedImpact = 0;
	m_speedImpactLoop = 10;
	m_coord->centerOnTile();
	m_C_Path = new C_Path(27,15);
	m_C_Path->calcPath(x_grid,y_grid,27,15);
	m_C_Path->showPath();
}

C_invaders::~C_invaders()
{
}

void C_invaders::play(){
	this->move();

	if(!this->alive())
		this->kill();
};


void C_invaders::move()
{
	m_moving = true;

	C_Grid& grid=C_Grid::Instances();
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
		        angle = angle *180/3.14159265359  + 45;
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
		            m_C_Path = new C_Path(27,15);
	                m_C_Path->calcPath(old_x_grid,old_y_grid,27,15);
		        	m_C_Path->showPath();
		        }
		    }
      }
	}

	if(!m_C_Path->closeToDestination(m_coord->getXGrid(),m_coord->getYGrid())){
       if(path.size() == 0){
		            delete m_C_Path;
		            m_C_Path = new C_Path(27,15);
	                m_C_Path->calcPath(m_coord->getXGrid(),m_coord->getYGrid(),27,15);
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


void C_invaders::renderLifeBar(int x_screen, int y_screen)
	{
		C_Window& win=C_Window::Instances();
		SDL_Renderer* renderer = win.getRenderer();
		//add a life status above the boat
		int l = m_life / 2;
		int red = 0, green = 200;
		SDL_Rect r, b;
		    r.x = x_screen - TILE_HALF_WIDTH/2;
		    r.y = y_screen - 85;
		    r.w = l;
		    r.h = 4;
		    if (m_life < MAX_LIFE/2){
		    	red = 200;
		    	green = 0 ;
		    	}
		    b.x = r.x - 1;
		    b.y = r.y - 1;
		    b.w = MAX_LIFE/2 + 2;
		    b.h = r.h + 2;

		    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		    SDL_RenderFillRect( renderer, &b );
		    SDL_SetRenderDrawColor( renderer, red, green, 0, 255 );
		    // Render rect
		    SDL_RenderFillRect( renderer, &r );
	}

void C_invaders::render(S_Coord screen){
	string name = getName();
	C_TextureList& t=C_TextureList::Instances();
	int imageNbr = 0;
	if (m_moving)
		imageNbr = m_animation[MAIN_ANIM]->getAnimNbr(1,2,300);
	string direction = "_SS_";
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

	t.renderTexture(fileName, screen.x,screen.y);
	renderLifeBar(screen.x, screen.y);
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
