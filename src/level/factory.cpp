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


#include <sys/stat.h>
#include <sstream>
#include "factory.h"
#include "../locator.h"
#include "grid.h"
#include "../message.h"
#include <fstream>
#include <algorithm>

using namespace std;

C_UnitFactory::C_UnitFactory()
{
	int size = 15;
	string file[size] = {"boat_00.tsx","boat_01.tsx","boat_02.tsx","boat_03.tsx","archerTower_00.tsx",
			"archerTower_01.tsx","archerTower_02.tsx","archerTower_03.tsx",
			"barricade_01.tsx","town_01.tsx","castle_00.tsx","catapult_00.tsx","catapult_01.tsx",
			"catapult_02.tsx","catapult_03.tsx"
						};
	C_Settings& settings=C_Locator::getSettings();
	string imageFolder = settings.getImgFolder();
	for(int i = 0; i < size; i++) {
		string path = imageFolder + file[i];
		if(tsxExist(path)) {
			C_Message::printV("tsx file: "+ file[i]+"\n");
			S_UnitModel unit = extractProperties(path);
			size_t pos = unit.name.find("_") + 2;
			unit.name = unit.name.substr(0,pos);
			m_models[unit.name]= unit;
    	std::cout << "unit.name " << unit.name << std::endl;
		} else {
			C_Message::printError("Can not find the tsx file: " + path);
		}
	}
}

C_GameUnits* C_UnitFactory::create(S_Unit unit)
{
	C_GameUnits* ret = nullptr;

	size_t pos = unit.name.find("_") + 2;
	string shortname = unit.name.substr(0,pos);
	std::cout << "unit.name " << unit.name << " - " << shortname << std::endl;
	S_UnitModel current = m_models[shortname];
	current.tileSource = unit.name;
	current.coord = unit.coord;
	if(unit.name.find("boat_") != string::npos){
		ret = new C_Boat(current);
	} else if(unit.name.find("ArcherTower_") != string::npos){
		ret = new C_ArcherTower(current);
	} else if(unit.name.find("CastleTower_") != string::npos){
		ret = new C_CastleTower(current);
	} else if(unit.name.find("barricade_") != string::npos){
		ret = new C_Barricade(current);
	} else if(unit.name.find("town_") != string::npos){
		ret = new C_Town(current);
	} else if(unit.name.find("Catapult_") != string::npos){
		ret = new C_Catapult(current);
	}

	return ret;
}

void C_UnitFactory::upgrade(C_GameUnits * unit)
{
	if(unit != nullptr){
		int currentRank = unit->getRank();
		string currentName = unit->getName();
		string type = unit->getType();
		int newRank = currentRank + 1;
		if((type == "ArcherTower" || type == "Catapult") && currentRank < unit->getMaxRank()){
			string up = type +"_"+ to_string(newRank);
			S_UnitModel model = m_models[up];
			C_Wallet& wallet=C_Locator::getWallet();
			if(wallet.getBalance() - model.cost >= 0){ //check if pocket is deep enough
				wallet.debit(model.cost);
				unit->upgrade(model);
				C_Message::printV("Upgrade "+ currentName +" to "+ model.name + ". Cost: " + to_string(model.cost) +"\n");
			}
		}
	}
}

bool C_UnitFactory::isUpgradable(C_GameUnits * unit){
	bool ret = false;
	if(unit != nullptr){
		int currentRank = unit->getRank();
		string currentName = unit->getName();
		string type = unit->getType();
		int newRank = currentRank + 1;
		if((type == "ArcherTower" || type == "Catapult") && currentRank < unit->getMaxRank()){
			string up = type +"_"+ to_string(newRank);
			if(m_models.count(up) > 0){
				C_Wallet& wallet=C_Locator::getWallet();
				if(wallet.getBalance() - m_models[up].cost >= 0){ //check if pocket is deep enough
					ret = true;
				}
			} else {
				//cout << "Model does not exist" << endl;
			}
		}
	}
	return ret;
}

bool C_UnitFactory::getSelectedModel(int increment, S_UnitModel &model){
	bool ret = false;
	C_Grid& grid= C_Locator::getGrid();
	C_GameUnits * unit = grid.getSelected();

	if(unit != nullptr){
		string currentType = unit->getType();
		int currentRank = unit->getRank();
		string name = currentType +"_"+ to_string(currentRank + increment);
		if(m_models.count(name) > 0){
			S_UnitModel copy = m_models[name];
			model = copy;
			ret = true;
		}
	}
	return ret;
}


S_UnitModel C_UnitFactory::extractProperties(string filename)
{
	C_Xml tsx(filename);
	S_UnitModel unit;
	unit.name = tsx.extractStrValue("tileset","name");
	unit.type = tsx.getStrProperty("unit.type","ArcherTower");
	unit.rank = tsx.getIntProperty("unit.rank", 0);
	unit.health = tsx.getIntProperty("unit.health", 100);
	unit.coord = {0,0};
	unit.cost = tsx.getIntProperty("unit.cost", 50);
	unit.speed = tsx.getIntProperty("unit.speed", 1);
	unit.alive = true;

	//S_Weapon

	unit.weapon.type = tsx.getStrProperty("weapon.type","ARCHER");
	unit.weapon.damage = tsx.getIntProperty("weapon.damage", 2);
	unit.weapon.fireRate = tsx.getIntProperty("weapon.firerate", 2000);
	unit.weapon.fireRange = tsx.getIntProperty("weapon.firerange", 2);
	unit.weapon.direction = EAST;
	return unit;
}


bool C_UnitFactory::tsxExist(const string &file)
{
	ifstream tmp(file.c_str());
	return !tmp.fail();
}

//-------------------------------Level Factory --------------------
C_LevelFactory::C_LevelFactory()
{
	m_lastLevelNbr = 3;
	for(int i = 1; i <= m_lastLevelNbr; i++) {
		m_levelList.push_back(extractInfosFromTmx(i));
	}
}

S_LevelData C_LevelFactory::extractInfosFromTmx(int levelNbr)
{
	S_LevelData level;
	level.nbr = levelNbr;
	C_Settings& settings=C_Locator::getSettings();
	level.filename = settings.getLevelFolder() + "Level_" + to_string(levelNbr) + ".tmx";
	struct stat buffer;
	if (stat (level.filename.c_str(),  &buffer) == 0) {
		C_Xml tmx(level.filename);
		level.width = stoi( tmx.extractStrValue("map","width"));
		level.height = stoi( tmx.extractStrValue( "map", "height"));
		level.gridSize = calcGridSize(level.width,level.height);;
		C_Message::printV("Grid size should be " + to_string(level.gridSize) + "\n");
		level.tilewidth = stoi( tmx.extractStrValue( "map", "tilewidth"));
		level.tileheight = stoi( tmx.extractStrValue( "map", "tileheight"));
		level.backgroundcolor =  tmx.extractStrValue( "map", "backgroundcolor");
		level.name = tmx.extractStrValue("property","name","subname","value");
		level.currentWave = 0;
		level.totalWaves = tmx.countAttributes("Wave");
	} else {
		C_Message::printError("Can not find " + level.filename+"\n");
	}
	return level;
}

int C_LevelFactory::calcGridSize(int width, int height)
{
	if(width > height)
		return width;
	else
		return height;
}



C_Level * C_LevelFactory::create(int nbr)
{
	int id = nbr;
	if(id < 1) {
		id = 1;
	} else if(id > m_lastLevelNbr) {
		id = m_lastLevelNbr;
	}
	S_LevelData level = m_levelList[id - 1];
	return  new C_Level(level);
}

S_LevelData C_LevelFactory::getModel(int levelNbr)
{
	int id = levelNbr;
	if(id < 1) {
		id = 1;
	} else if(id > m_lastLevelNbr) {
		id = m_lastLevelNbr;
	}
	return m_levelList[id - 1];
}
