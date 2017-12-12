#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL2/SDL.h>
#include <map>
#include <string>

class C_MenuItem
{
	public:
	C_MenuItem(std::string name,int nbr);
	virtual ~C_MenuItem();
	virtual int getXScreen() const;
	virtual int getYScreen() const;
	virtual int getWidth() const;
	virtual int getHeight() const;

	virtual	void render() = 0;
	protected:
	std::string m_name;
	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
};


enum button{ADDNEWTOWER};

class C_Button: public C_MenuItem
{
	public:
	C_Button(std::string name,std::string image_out,int nbr);
	~C_Button();

	void render();

	protected:
	std::string m_image_out;

};


class C_Menu
{
	public:
	static	C_Menu& Instances();
	void render();
	C_MenuItem * getMenuItem(int button);
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

	std::map<int,C_MenuItem*> m_map_menuItems;
};

#endif
