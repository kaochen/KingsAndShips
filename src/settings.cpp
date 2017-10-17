#include "settings.h"

using namespace std;
int C_Settings::windowWidth = WINDOW_WIDTH;
int C_Settings::windowHeight = WINDOW_HEIGHT;

//constructor
C_Settings::C_Settings()
{
}

C_Settings::~C_Settings()
{
}


int C_Settings::getWindowWidth(){
	return windowWidth;
}

int C_Settings::getWindowHeight(){
	return windowHeight;
}


int C_Settings::getTableWidth(){
	return windowWidth / TILE_HALF_WIDTH;
}

int C_Settings::getTableHeight(){
	return windowHeight / TILE_HALF_HEIGHT;
}
