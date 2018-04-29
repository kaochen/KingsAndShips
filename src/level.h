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


#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "gameUnits.h"
#include "invaders.h"

struct S_boat{
	int rank;
	int x;
	int y;
  bool alive;
};

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
  void add(int rank, int x, int y);
  void display();
  void loadIntoGrid();

  private:
  //attibutes
  std::vector <S_boat> m_boatList;
};

class C_Level
{

	public:

	//methods
	C_Level();
	~C_Level();
  void load(int levelNbr);
	void status();
	void sendNextWave();
  void displayWave(int i);
  void loadWaveIntoGrid(int i);
	protected:
  //methods
	S_tmxLayer extractTMXfile(std::string tmx_File_Path, std::string layerName);
  void loadGroundLayerIntoTheGrid(std::string tmx_File_Path);
  void loadWave(std::string tmx_File_Path, int waveNbr);
  void loadDecorLayerIntoTheGrid(std::string tmx_File_Path);
	//attibutes
	std::string m_name;
	int m_count;
	int m_id;
  S_tmxLayer m_groundLayer;
  S_tmxLayer m_decorLayer;
  int m_nbrOfWaves;
  int m_currentWaveNbr;
  std::vector <C_Wave> m_waves;
};






#endif
