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
#define ASPECT_RATIO 9/16
#define FRAMERATE 30
#define TILE_HALF_HEIGHT 24
#define TILE_HALF_WIDTH 48

#define MAX_TARGETS 3

struct S_Coord{
	int x;
	int y;
};

struct S_Size{
  int w;
  int h;
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
  int direction;
};

enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
enum GroundType {GROUND_01,GROUND_02};
enum Speed {VERY_SLOW,SLOW, NORMAL,FAST,VERY_FAST};
enum Status {ALIVE,DEAD};
enum buttonType {ACTION,DRAGUNIT,STATUS};
enum texture_align {LEFT,CENTER,RIGHT,CENTER_TILE};


//singleton
class C_Settings
{
public:
	static	C_Settings& Instances() {return m_instance;};
  //window
	int getWindowWidth() {return m_windowWidth;};
	int getWindowHeight() {return m_windowHeight;};
  S_Coord getCameraPosition(){return m_cameraPos;}; /*!< get the position of the window over the level map*/
  void setCameraPosition(const S_Coord &pos);
  void centerCameraPosition();
  void moveCameraPosition(const int &rigth,const int &left,const int &down,const int &up);
  //grid
	size_t getGridSize() {return m_gridSize;};
	int getGridWidth() {return m_windowWidth / TILE_HALF_WIDTH;};
	int getGridHeight() {return m_windowHeight / TILE_HALF_HEIGHT;};
  S_Size getNbrOfTilesToDisplay();
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
  void calcGridSize();

	static C_Settings m_instance;
	C_Settings();
	~C_Settings();

  //window
	int m_windowWidth;
	int m_windowHeight;
  S_Coord m_cameraPos; /*!< get the position of the window over the level map*/
  //grid
	size_t m_gridSize; //the grid is a square even if the screen is non-square.

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
