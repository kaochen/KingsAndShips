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


#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <queue>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define ASPECT_RATIO 9/16
#define FRAMERATE 30
#define TILE_HALF_HEIGHT 24
#define TILE_HALF_WIDTH 48
#define GRID_SIZE 32
#define BOAT_LIST_SIZE 4
#define MAX_LIFE 100
#define LAYER 2
#define SPRITE_SIZE 128
#define MAX_TARGETS 3
#define MAX_LEVELS 1

struct S_Coord{
	int x;
	int y;
};
struct S_NodeCoord{
	S_Coord screen;
	S_Coord grid;
};

struct S_Weapon{
	int damage;
	int speedImpact;
	int fireRate;
	int fireRange;
  std::string direction;
};

enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
enum GroundType {GROUND_01,GROUND_02};
enum Speed {VERY_SLOW,SLOW, NORMAL,FAST,VERY_FAST};
enum Status {ALIVE,DEAD};

//singleton
class C_Settings
{
public:
	static	C_Settings& Instances() {return m_instance;};
  //window
	int getWindowWidth() {return m_windowWidth;};
	int getWindowHeight() {return m_windowHeight;};
  //grid
	int getGridSize() {return m_gridSize;};
	int getGridWidth() {return m_windowWidth / TILE_HALF_WIDTH;};
	int getGridHeight() {return m_windowHeight / TILE_HALF_HEIGHT;};
	int getGridNbrOfLine() {return m_gridNbrOfLine;};
	int getGridNbrOfRow() {return m_gridNbrOfRow;};
	int getGridFirstTileX() {return m_first_tile_x;};
	int getGridFirstTileY() {return m_first_tile_y;};
  //debug
	void setDebugMode();
	bool getDebugMode() {return m_debugMode;};
  void setDebugPathMode();
  bool getDebugPathMode() {return m_debugPath;};
	void displayDebugMode();
  //images
  void initTSXfileList();
  std::queue<std::string>* getTSXfileList(){return &m_tsxFileList;};
  std::string getImgFolder(){return m_imgFolder;};
  std::string getThemePath(){return m_imgFolder + m_theme;};
  //level
  int setCurrentLevelNbr(int nbr);
  int getCurrentLevelNbr(){return m_currentLevel;};
  int getNbrOfLevels();
  std::string getLevelFolder(){return m_levelFolder;};

private:
	C_Settings& operator= (const C_Settings&){return *this;}
	C_Settings (const C_Settings&){}
  bool fileExist(const std::string &file);

	static C_Settings m_instance;
	C_Settings();
	~C_Settings();

  //window
	int m_windowWidth;
	int m_windowHeight;
  //grid
	int m_gridSize;
	int m_gridNbrOfLine; //nbr of tiles in a line
	int m_gridNbrOfRow; //nbr of tiles in a row
	int m_first_tile_x; //first tile to display
	int m_first_tile_y;
  //debug
	bool m_debugMode;
  bool m_debugPath;
  //images
  std::string m_imgFolder;
  std::string m_theme;
  std::queue<std::string> m_tsxFileList;
  //levels
  int m_currentLevel;
  int m_nbrOfLevels;
  std::string m_levelFolder;
};

#endif
