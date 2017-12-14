#include "menu.h"
#include "window.h"
#include "settings.h"
#include "surfaces.h"
#include <string>

#include <SDL2_gfxPrimitives.h>

using namespace std;
C_MenuItem::C_MenuItem(string name, int displayOrder):
	m_name(name),
	m_width(64),
	m_height(64)
{
	C_Menu& menu=C_Menu::Instances();

	m_x_screen = menu.getXScreen() + m_width*displayOrder + 10;
	m_y_screen = 2;
}
C_MenuItem::~C_MenuItem()
{
}

int C_MenuItem::getXScreen() const{
	return m_x_screen;
}
int C_MenuItem::getYScreen() const{
	return m_y_screen;
}
int C_MenuItem::getWidth() const{
	return m_width;
}
int C_MenuItem::getHeight() const{
	return m_height;
}
string C_MenuItem::getName(){
	return m_name;
	}

//-------------------------------------------------------------

C_Button::C_Button(string name,string image_out,int displayOrder)
	:C_MenuItem(name,displayOrder),
	m_image_out(image_out)
{
}

C_Button::~C_Button()
{
}

void C_Button::render(){
		C_TextureList& t=C_TextureList::Instances();
		t.renderTexture(m_image_out, m_x_screen + m_width/2,m_y_screen);
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
		m_map_menuItems[ADDNEWTOWER] = new C_Button("addNewTower","Buttons_AddTowerOut",0);
		m_map_menuItems[ADDNEWMILL] = new C_Button("addNewMill","Buttons_AddMillOut",1);
		m_button_count += 2;
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
		m_map_menuItems[i]->render();
	}
}
C_MenuItem * C_Menu::getMenuItem(int menuItem){
	return m_map_menuItems[menuItem];
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





