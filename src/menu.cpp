#include "menu.h"
#include "window.h"
#include "settings.h"
#include "surfaces.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;

//constructor
C_Button::C_Button(string name,int nbr):
	m_name(name),
	m_width(64),
	m_height(64)
{

	C_Menu& menu=C_Menu::Instances();

	m_x_screen = menu.getXScreen() + m_width*nbr + 10;
	m_y_screen = 2;
}

C_Button::~C_Button()
{
}

void C_Button::render(){
		C_TextureList& t=C_TextureList::Instances();
		t.renderTexture("Buttons_AddTowerOut", m_x_screen + m_width/2,m_y_screen);
}
int C_Button::getXScreen() const{
	return m_x_screen;
}
int C_Button::getYScreen() const{
	return m_y_screen;
}
int C_Button::getWidth() const{
	return m_width;
}
int C_Button::getHeight() const{
	return m_height;
}

//-------------------------------------------------------------

C_Menu C_Menu::m_instance=C_Menu();

C_Menu::C_Menu():
	m_y_screen(0),
	m_height(72)
{
		C_Set& settings=C_Set::Instances();
		m_width = (settings.getWindowWidth()*50)/100;
		m_x_screen = (settings.getWindowWidth() - m_width)/2;
		m_map_buttons[ADDNEWTOWER] = new C_Button("addNewTower",0);
		m_button_count++;
}

C_Menu::~C_Menu(){
}

C_Menu& C_Menu::Instances()
{
	return m_instance;
}

void C_Menu::render(){
	drawBackground();
	for (int i = 0; i < m_button_count; i++){
		m_map_buttons[ADDNEWTOWER]->render();
	}
}
C_Button * C_Menu::getButton(int button){
	return m_map_buttons[button];
}

int C_Menu::getXScreen(){
	return m_x_screen;
}


void C_Menu::drawBackground(){
		C_Window& win=C_Window::Instances();
		Sint16 x_tr = m_width + m_x_screen; //x top right
		Sint16 y_tr = m_y_screen -5;
		Sint16 x_bl = m_x_screen; //x bottom left
		Sint16 y_bl = m_height;
		Uint8 R = 50, G = 50, B = 50, A = 230;
		roundedBoxRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,5,R,G,B,A);
		roundedRectangleRGBA(win.getRenderer(),x_tr,y_tr,x_bl,y_bl,5,R,G,B,255);

}





