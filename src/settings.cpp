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


int C_Settings::getWindowWith(){
	return windowWidth;
}

int C_Settings::getWindowHeight(){
	return windowHeight;
}
