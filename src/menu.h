#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL2/SDL.h>
#include <map>
#include <string>

enum button{ADDNEWTOWER};

class C_Button
{
	public:
	C_Button(std::string name,int nbr);
	~C_Button();

	void render();
	int getXScreen() const;
	int getYScreen() const;
	int getWidth() const;
	int getHeight() const;

	protected:
	std::string m_name;
	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
};


class C_Menu
{
	public:
	static	C_Menu& Instances();
	void render();
	C_Button * getButton(int button);
	int getXScreen();

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
	int m_button_count;

	std::map<int,C_Button*> m_map_buttons;
};

#endif
