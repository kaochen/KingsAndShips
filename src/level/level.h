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

//manage the level, read, load ...

#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>
#include <list>
#include <vector>

#include "landscape.h"
#include "../coord.h"
#include "../xml.h"



class C_Wave{
  public:
  C_Wave();
  ~C_Wave(){};
  void add(std::string name, S_Coord coord);
  void cliStatus();
  void loadIntoGrid();

  private:
  //attibutes
  std::vector <S_Unit> m_boatList;
  int m_count;
};

class C_Level
{

	public:

	//methods
	C_Level();
	~C_Level();
  void load(int levelNbr);
	void cliStatus();
	void sendNextWave();
  void cliWaveStatus(int i);
  void loadWaveIntoGrid(int i);
  void updateMenuInfo();
  void render();
	void play();
  bool selectATower(S_Coord clic);
  void addUnit(std::string &type, S_Coord clic);
  S_Coord getGridTown();

protected:
  //methods
  void extractInfosFromTmx(int levelNbr);
  void loadGroundLayerIntoTheGrid(std::string tmx_File_Path);
  void loadWave(std::string tmx_File_Path, int waveNbr);
  void loadDecorLayerIntoTheGrid(std::string tmx_File_Path);
  S_Coord getFirstTile(S_tmxLayer &layer);
  void setWallet();
  void createLandscape();
	void playAllUnits ();

	//attibutes
	std::string m_name;
	int m_count;
	int m_id;
  S_tmxLayer m_groundLayer;
  S_tmxLayer m_decorLayer;
  int m_nbrOfWaves;
  int m_currentWaveNbr;
	long m_lastWaveTime;
  std::vector <C_Wave> m_waves;
  int calcGridSize();
  C_Landscape* m_landscape;

  //properties
  std::string m_filename;
  std::string m_levelName;
  int m_width;
  int m_height;
  int m_gridSize;
  int m_tilewidth;
  int m_tileheight;
  std::string m_backgroundcolor;
};


#endif
