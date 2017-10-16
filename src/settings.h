#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

#define WINDOW_HEIGHT 400
#define WINDOW_WIDTH 800
#define FRAMERATE 30
#define SPRITE_SIZE 40

class C_Settings
{
	public:
	//methods
	C_Settings();
	~C_Settings();

	static int getWindowWidth();
	static int getWindowHeight();

	static int getTableWidth();
	static int getTableHeight();

	private:
	//attibuts
	static int windowWidth;
	static int windowHeight;
};


#endif
