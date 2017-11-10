#include "towers.h"
#include "time.h"

using namespace std;


C_Towers::C_Towers(int x_grid,
		   int y_grid,
		   int rank):C_Shooter("Tower", x_grid, y_grid, rank)
{
	m_lastSmokeTime = SDL_GetTicks();
	m_smokeNbr = 1;
	m_justAdded = true;
}


void C_Towers::renderSmoke(SDL_Renderer *renderer){
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
	renderTexture(C_Texture::getText(fileName), renderer, m_x_screen,m_y_screen + 36);
}

void C_Towers::render(int x_screen, int y_screen, SDL_Renderer *renderer){
	C_Shooter::render(x_screen, y_screen,renderer);
	if (m_justAdded)
		renderSmoke(renderer);
}



