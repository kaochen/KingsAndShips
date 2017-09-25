#include "towers.h"

using namespace std;

C_Towers::C_Towers(SDL_Renderer* renderer):C_Shooter("TOWER",1)
{
}

C_Towers::C_Towers(SDL_Renderer* renderer,int rank):C_Shooter("TOWER", rank)
{
	SDL_Texture* img = nullptr;
	switch(rank){
		case 1:
			img = loadTexture("data/img/original/Tower_01.png",renderer);
			m_texture = new C_Texture("Tower_01",img,1);
		break;
		case 2:
			img = loadTexture("data/img/original/Tower_02.png",renderer);
			m_texture = new C_Texture("Tower_02",img,1);
		break;
		case 3:
			img = loadTexture("data/img/original/Tower_03.png",renderer);
			m_texture = new C_Texture("Tower_03",img,1);
		break;
		}

}


void C_Towers::printOnScreen(SDL_Renderer *renderer){

	cout << "marker 2" << endl;
	renderTexture(m_texture->getImage(), renderer, m_x, m_y);
	cout << "marker 3" << endl;
}

