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

struct S_LevelData {
	int nbr;
	std::string filename;
	std::string name;
	int width;
	int height;
	int tilewidth;
	int tileheight;
	int gridSize;
	std::string backgroundcolor;
    int currentWave;
    int totalWaves;
    int status; //level status ONGOING, LOSE, WIN
};

class C_Wave {
public:
	C_Wave();
	~C_Wave() {};
	void add(std::string name, S_Coord coord);
	void cliStatus();
	void loadIntoGrid();

private:
//attibutes
	std::vector <S_Unit> m_boatList;
};

class C_Level {

public:
//methods
	C_Level(S_LevelData model);
	~C_Level();
	bool load(int levelNbr);
	void sendNextWave();
	void cliWaveStatus(int i);
	void loadWaveIntoGrid(int i);
	void render();
	void renderSelected();
	void play();
	bool selectATower(S_Coord clic);
	void unselectedAll();
	void addUnit(std::string &type, S_Coord clic);
	bool endOfALevel();
	S_Coord getGridTown();
    S_LevelData getData(){return m_data;};
    void setStatus(int status);

protected:
//methods
	void loadLayerIntoTheGrid(std::string type);
	void loadWave(std::string tmx_File_Path, int waveNbr);
	S_Coord getFirstTile(S_tmxLayer &layer);
	void setWallet();
	void createLandscape();

//attibutes
	int m_id;
	S_tmxLayer m_decorLayer;
    C_Tmx* m_tmx;

    //waves
	long m_lastWaveTime;
	std::vector <C_Wave> m_waves;

	C_Landscape* m_landscape;

//properties
    S_LevelData m_data;
};


#endif
