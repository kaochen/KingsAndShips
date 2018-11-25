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


struct S_tmxLayer{
  std::string name;
  int width;
  int height;
  std::string data;
};

class C_Wave{
  public:
  C_Wave();
  ~C_Wave();
  void add(std::string name,int rank, S_Coord coord);
  void cliStatus();
  void loadIntoGrid();
  void addToDeadCounter(int nbr);

  private:
  //attibutes
  std::vector <S_UnitModel> m_boatList;
  int m_count;
  int m_count_dead;
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
  void playAllUnits ();
  bool selectATower(S_Coord clic);
  void addUnit(std::string &type, S_Coord clic);
  S_Coord getGridTown();

	protected:
  //methods
  void extractInfosFromTmx(int levelNbr);
  std::string extractPropertyFromTmxFile(std::string tmx_File_Path, const std::string &name);
	S_tmxLayer extractTMXfile(std::string tmx_File_Path, std::string layerName);
  void loadGroundLayerIntoTheGrid(std::string tmx_File_Path);
  void loadWave(std::string tmx_File_Path, int waveNbr);
  void loadDecorLayerIntoTheGrid(std::string tmx_File_Path);
  int countAttributes(std::string tmx_File_Path, std::string pattern);
  S_Coord getFirstTile(S_tmxLayer &layer);
  void setWallet();
  void createLandscape();
  std::string extractValueFromTmxFile(std::string tmx_File_Path, const std::string &node, const std::string &attribute);

	//attibutes
	std::string m_name;
	int m_count;
	int m_id;
  S_tmxLayer m_groundLayer;
  S_tmxLayer m_decorLayer;
  int m_nbrOfWaves;
  int m_currentWaveNbr;
  std::vector <C_Wave> m_waves;
  int calcGridSize();
  C_Landscape* m_landscape;

  //properties
  std::string m_filename;
  int m_width;
  int m_height;
  int m_gridSize;
  int m_tilewidth;
  int m_tileheight;
  std::string m_backgroundcolor;
};






#endif
