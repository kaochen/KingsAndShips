/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL2/SDL.h>
#include <map>
#include <string>
#include "settings.h"

class C_MenuItem
{
	public:
  C_MenuItem(std::string name, int x_screen, int y_screen);
	virtual ~C_MenuItem();
	virtual int getXScreen() const;
	virtual int getYScreen() const;
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual	std::string getName();

  virtual void setPercentage(int percentage){std::cout << percentage;};
  virtual void setPercentage(int a, int b){std::cout << a << b;};
	virtual	void render();
	protected:
	std::string m_name;
	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
};


enum button{ADDNEWTOWER,ADDNEWTURBINE,PLAYERLIFE,FOX_ICON,INVADER_LIFE,WAVES_STATUS,NONE};

class C_Button: public C_MenuItem
{
	public:
  C_Button(std::string name,std::string image_out,int x_screen, int y_screen);
	~C_Button();

  virtual void setPercentage(int percentage);
  virtual void setPercentage(int a, int b);
	virtual void render();
	protected:
	std::string m_image_out;

};


class C_ProgressBar: public C_MenuItem
{
	public:
  C_ProgressBar(std::string name,int x_screen, int y_screen);
	virtual void render();
  virtual void setPercentage(int percentage);
  virtual void setPercentage(int a, int b);
  protected:
  void littledots(int x_screen, int y_screen, int width, int height);
  int m_percentage;
};


class C_MenuText: public C_MenuItem
{
  public:
  C_MenuText(std::string name, std::string text, int fontSize, int x_screen, int y_screen);
  virtual void render();
  private:
  std::string m_text;
  int m_fontSize;
};



class C_Menu
{
	public:
	static	C_Menu& Instances();
	void render();
	C_MenuItem * getMenuItem(int button);
	int getXScreen();
  void updateLevelInfos(int current_wave, int total_waves);
  void resetValues();

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

  //information to display
  int m_current_wave;
  int m_total_waves;

	std::map<int,C_MenuItem*> m_map_menuItems;
};

#endif
