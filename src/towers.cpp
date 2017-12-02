#include "towers.h"
#include "time.h"
#include "grid.h"

#include <SDL2_gfxPrimitives.h>

using namespace std;


C_Towers::C_Towers(int x_grid,
		   int y_grid,
		   int rank):C_Shooter("Tower", x_grid, y_grid, rank)
{
	m_lastSmokeTime = SDL_GetTicks();
	m_smokeNbr = 1;
	m_justAdded = true;
}


void C_Towers::renderSmoke(){
	C_Time& time=C_Time::Instances();
	long delay = time.getFrameDuration()*4;
	long current = SDL_GetTicks();
	if (current > m_lastSmokeTime + delay){
		m_smokeNbr++;
		m_lastSmokeTime = current;
		}

	if (m_smokeNbr > 8){
		m_smokeNbr = 1;
		m_justAdded = false;
	}
	string fileName = "smoke_0" + to_string(m_smokeNbr) + ".png" ;
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, m_x_screen,m_y_screen + 36);
}

void C_Towers::render(int x_screen, int y_screen){
	C_Shooter::render(x_screen, y_screen);
	if (m_justAdded)
		renderSmoke();
}

void C_Towers::drag(int x_screen, int y_screen)
{
	m_justAdded = false;
	C_TextureList& t=C_TextureList::Instances();
	C_Grid& grid=C_Grid::Instances();
	int xGrid = xScreenToXGrid(x_screen,y_screen - 100);
	int yGrid = yScreenToYGrid(x_screen,y_screen - 100);
	if (grid.isThisConstructible(xGrid, yGrid)){
		drawElipse(x_screen,y_screen,100, true);
	 	t.renderTexture("Tile_Highlight_Green.png", x_screen,y_screen - 100);
	 	}
	else{
		drawElipse(x_screen,y_screen,100, false);
		}



	C_Shooter::render(x_screen, y_screen - 200);
}

void C_Towers::drawElipse(int x,
		int y,
		int width,
		bool ok){
		C_Window& win=C_Window::Instances();
		int height = width/2;
		int R = 0, G = 200, B = 0, A = 128;
			if(ok == false)
				R = 120, G = 0, B = 0, A = 128;

		ellipseRGBA(win.getRenderer(),x,y,width+1,height+1,R,G,B,A);
		A = 32;
		filledEllipseRGBA(win.getRenderer(),x,y,100,50,R,G,B,A);
}

