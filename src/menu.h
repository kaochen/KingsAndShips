#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL2/SDL.h>


class C_Menu
{
	public:
	static	C_Menu& Instances();
	void render();

	protected:

	void drawBackground();

	private:
	C_Menu& operator= (const C_Menu&){return *this;}
	C_Menu (const C_Menu&){}

	static C_Menu m_instance;
	C_Menu();
	~C_Menu();

	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;

};


#endif
