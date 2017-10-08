#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600
#define FRAMERATE 30
#define SPRITE_SIZE 40

class C_Settings
{
	public:
	//methods
	C_Settings();
	~C_Settings();

	static int getWindowWith();
	static int getWindowHeight();

	private:
	//attibuts
	static int windowWidth;
	static int windowHeight;
};


#endif
