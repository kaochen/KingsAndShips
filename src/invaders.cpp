#include "invaders.h"
#include "time.h"
#include "grid.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank):C_Shooter("boat", x_grid, y_grid ,rank)
{
	m_moving = false;
	m_animNbr = 0;
	m_lastAnimTime = 0;
}

C_invaders::~C_invaders()
{
	m_moving = false;
	m_animNbr = 0;
	m_lastAnimTime = 0;
}


void C_invaders::move(int direction)
{

	C_Grid& grid=C_Grid::Instances();
	m_moving = true;
	int speed = 2;
	switch (direction){
		case EAST:
			m_x_screen += speed;
			m_y_screen += speed/2;
		break;
		case WEST:
			m_x_screen -= speed;
			m_y_screen -= speed/2;
		break;
		case NORTH:
			m_x_screen += speed;
			m_y_screen -= speed/2;
		break;
		case SOUTH:
			m_x_screen -= speed;
			m_y_screen += speed/2;
		break;
	}

	int new_x_grid = xScreenToXGrid (m_x_screen, m_y_screen);
	int new_y_grid = yScreenToYGrid (m_x_screen, m_y_screen);

	if(m_x_grid != new_x_grid || m_y_grid != new_y_grid){
		grid.moveUnit(m_x_grid, m_y_grid, new_x_grid, new_y_grid);
		//cout << "Move from:" << m_x_grid << ":" << m_y_grid << " to:" << new_x_grid << ":" << new_y_grid << endl;
		m_x_grid = new_x_grid;
		m_y_grid = new_y_grid;
		}

}


void C_invaders::renderLifeBar(int x_screen, int y_screen, SDL_Renderer *renderer)
	{
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

void C_invaders::render(int x_screen, int y_screen, SDL_Renderer *renderer){
	string name = getName();
	C_Time& time=C_Time::Instances();
	long delay = time.getFrameDuration()*4;
	long current = SDL_GetTicks();
	if (current > m_lastAnimTime + delay){
		m_animNbr++;
		m_lastAnimTime = current;
		}

	if (m_animNbr > 1){
		m_animNbr = 0;
	}
	int rank = getRank();
	string fileName = name + "_0" + to_string(rank) + "_00.png" ;
	if (m_moving == true)
		fileName = name + "Moving_0" + to_string(rank) +"_" + to_string(m_animNbr) + "0.png" ;

	//cout << "image name is "<< fileName << endl;
	renderTexture(C_Texture::getText(fileName), renderer, x_screen,y_screen + m_y_center_offset);
	renderLifeBar(x_screen, y_screen, renderer);
}

