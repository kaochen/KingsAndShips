#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define ASPECT_RATIO 9/16
#define FRAMERATE 30
#define TILE_HALF_HEIGHT 25
#define TILE_HALF_WIDTH 50
#define TABLE_SIZE 30

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
	static int getGridSize();

	private:
	//attibuts
	static int windowWidth;
	static int windowHeight;
	static int gridSize;
};


#endif
