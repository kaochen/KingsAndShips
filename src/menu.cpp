#include "menu.h"
#include "window.h"
#include "settings.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;

//constructor

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_y_screen(0),
	m_height(50)
{
		C_Set& settings=C_Set::Instances();
		m_width = (settings.getWindowWidth()*80)/100;
		m_x_screen = (settings.getWindowWidth() - m_width)/2;

}

C_Menu::~C_Menu(){
}

C_Menu& C_Menu::Instances()
{
	return m_instance;
}

void C_Menu::render(){
	drawBackground();
}


void C_Menu::drawBackground(){
		C_Window& win=C_Window::Instances();
		Sint16 x_tr = m_width + m_x_screen; //x top right
		Sint16 y_tr = m_y_screen -5;
		Sint16 x_bl = m_x_screen; //x bottom left
		Sint16 y_bl = m_height;
		Uint8 R = 50, G = 50, B = 50, A = 200;
		roundedBoxRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,5,R,G,B,A);
		roundedRectangleRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,5,R,G,B,255);

}
