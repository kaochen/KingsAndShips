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

//Menu elements like progressbar and buttons
enum menuLayer {BACK,FRONT};

#ifndef MENUITEMS_H
#define MENUITEMS_H

#include <iostream>
#include <string>
#include "level/shooter.h"

enum buttonState{ACTIVE,HOVER,DISABLED};

class C_MenuItem
{
	public:
  C_MenuItem(std::string name, int x_screen, int y_screen);
	virtual ~C_MenuItem(){};
	virtual int getXScreen() const{return m_x_screen;};
	virtual int getYScreen() const{return m_y_screen;};
	virtual int getWidth() const{return m_width;};
	virtual int getHeight() const{return m_height;};
	virtual	std::string getName(){return m_name;};
  virtual int getLayer(){return m_layer;};

  virtual void setPercentage(int percentage){std::cout << percentage;};
  virtual void setPercentage(int a, int b){std::cout << a << b;};
	virtual	void render();
  virtual int getEnable(){return m_enable;};
  virtual void setState(int state) {std::cout << state;};
  virtual void drag(S_Coord screen){std::cout << "GCC calm: " << screen.x;};

  //set Text
  virtual void setText(std::string text, int fontSize);
  virtual void setTextPosition(int x_text, int y_text);
  virtual void setTextColor(SDL_Color color){ m_color = color;};

	protected:
	std::string m_name;
	int m_x_screen;
	int m_y_screen;
	int m_width;
	int m_height;
  int m_layer;
  bool m_enable;
  //text
  std::string m_textName;
  std::string m_text;
  std::string m_oldText;
  int  m_fontSize;
  SDL_Color m_color;
  int m_x_text;
  int m_y_text;
};



class C_Button: public C_MenuItem
{
	public:
  C_Button(std::string name,std::string image_out,int x_screen, int y_screen);
	~C_Button(){};

	virtual void render();
  virtual int getState(){return m_state;};
  virtual void setState(int state){m_state = state;};
	protected:
	std::string m_image_out;
  int m_state;
};

class C_ButtonAddUnit: public C_Button
{
	public:
  C_ButtonAddUnit(std::string name,std::string image_out,int x_screen, int y_screen);
	~C_ButtonAddUnit();
  virtual void drag(S_Coord screen);
  virtual void render();

	protected:
  C_Shooter * m_unit;
};


class C_ProgressBar: public C_MenuItem
{
	public:
  C_ProgressBar(std::string name,int x_screen, int y_screen);
	virtual void render();
  virtual void setPercentage(int percentage){m_percentage = percentage;};
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
};


#endif
