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
};

struct S_tmxLayer{
  std::string name;
  int width;
  int height;
  std::string data;
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

	protected:
  //methods
	S_tmxLayer extractTMXfile(std::string tmx_File_Path, std::string layerName);
  void loadGroundLayerIntoTheGrid(std::string tmx_File_Path);
  void loadWave(std::string tmx_File_Path);
	//attibuts
	std::string m_name;
	int m_count;
	int m_id;
  S_tmxLayer m_groundLayer;
};







#endif
