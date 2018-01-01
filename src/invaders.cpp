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
	m_C_Path = new C_Path(27,15);
	m_C_Path->calcPath(x_grid,y_grid,27,15);
	m_C_Path->showPath();
}

C_invaders::~C_invaders()
{
}


void C_invaders::move()
{
	C_Grid& grid=C_Grid::Instances();
	m_moving = true;
	updateDirection();

}

void C_invaders::updateDirection(){
	std::stack<C_Node*> path;
	path = m_C_Path->getPath();
	int nextX = path.top()->getXGrid();
	int nextY = path.top()->getYGrid();
	cout << m_x_grid << ":" << m_y_grid << "->" << nextX << ":" << nextY << endl;
//enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
	if(nextX == m_x_grid && nextY == m_y_grid){
		m_C_Path->goNextStep();
		}
	else if(nextX > m_x_grid && nextY == m_y_grid)
		m_direction = EAST;
	else if(nextX > m_x_grid && nextY > m_y_grid)
		m_direction = SOUTH_EAST;
	else if(nextX == m_x_grid && nextY > m_y_grid)
		m_direction = SOUTH;
	else if(nextX > m_x_grid && nextY < m_y_grid)
		m_direction = NORTH_EAST;
	else if(nextX < m_x_grid && nextY == m_y_grid)
		m_direction = WEST;
	else if(nextX < m_x_grid && nextY < m_y_grid)
		m_direction = NORTH_WEST;
	else if(nextX == m_x_grid && nextY < m_y_grid)
		m_direction = NORTH;
	else if(nextX < m_x_grid && nextY > m_y_grid)
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
		    r.y = y_screen + 64;
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

void C_invaders::render(int x_screen, int y_screen){
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

	t.renderTexture(fileName, x_screen,y_screen + m_y_center_offset);
	renderLifeBar(x_screen, y_screen);
	m_C_Path->displayPath();
}

