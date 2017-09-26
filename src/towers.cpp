#include "towers.h"

using namespace std;

C_Towers::C_Towers(SDL_Renderer* renderer):C_Shooter("TOWER",1)
{
}

C_Towers::C_Towers(SDL_Renderer* renderer,int rank):C_Shooter("TOWER", rank)
{
}




