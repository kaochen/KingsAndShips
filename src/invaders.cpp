#include "invaders.h"
#include "time.h"
#include "grid.h"

#include "window.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank):C_Shooter("boat", x_grid, y_grid ,rank)
{
	m_moving = false;
	m_speed = 8;
	m_speedImpact = 0;
	m_coord->centerOnTile();
	m_C_Path = new C_Path(27,15);
	m_C_Path->calcPath(x_grid,y_grid,27,15);
	m_C_Path->showPath();
}

C_invaders::~C_invaders()
{
}


void C_invaders::move()
{
	m_moving = true;
	std::stack<C_Node*> path;
	path = m_C_Path->getPath();
	int speed = m_speed - m_speedImpact;
	if(path.size() > 1){
		//m_coord->displayStatus();
		int old_x_grid = m_coord->getXGrid ();
		int old_y_grid = m_coord->getYGrid ();

		int hyp = path.top()->getDist();

		//cout << "old hyp" << hyp << endl;
		double angle = path.top()->getAngle();
		C_Coord destCoord = *path.top()->getCoord();
		destCoord.centerOnTile();
		S_Coord start = m_coord->getScreen();
		S_Coord dest = destCoord.getScreen();


		int ab = dest.x - start.x;
		int bc = dest.y - start.y;
		hyp = sqrt((ab*ab + bc*bc));
		//cout << "hyp: " << hyp << endl;
		angle = atan2(ab,bc);


		path.top()->setDist(hyp - speed, angle);

		m_coord->move(angle,speed);
		angle = angle *180/3.14159265359  + 45;
		m_direction = destCoord.angleToDirection(angle);

		C_Grid& grid=C_Grid::Instances();


		if(hyp < speed){
			//cout << "got next" << endl;
			grid.moveUnit(old_x_grid, old_y_grid, m_coord->getXGrid (), m_coord->getYGrid ());
			m_C_Path->goNextStep();
			m_speedImpact = 0;
			//cout << "Move to : " << endl;
			//m_coord->displayStatus();
			//cout << "-------------------- " << endl;
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
	int rank = getRank();
	//boat_01_SW_0
	string fileName = name + "_0" + to_string(rank) + direction + to_string(imageNbr) ;

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
