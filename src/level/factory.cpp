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
#include "grid.h"
#include "../message.h"
#include <fstream>

using namespace std;

C_UnitFactory::C_UnitFactory(){
    int size = 7;
    string file[size] = {"boat_00.tsx","boat_01.tsx","archerTower_00.tsx",
    "archerTower_01.tsx","barricade_01.tsx","town_01.tsx","turbine_00.tsx"};
    for(int i = 0; i < size; i++){
        string path = "data/img/" + file[i];
        if(tsxExist(path)){
            S_UnitModel unit = extractProperties(path);
            m_models[unit.name]= unit;
            cout << endl;
        }
        else{
            C_Message m;
            m.printM("Can not find the tsx file: " + path);
        }
    }
}

C_GameUnits* C_UnitFactory::create(S_Unit type){
    C_GameUnits* unit = nullptr;
    S_UnitModel current = m_models[type.name];
    current.coord = type.coord;

    if(type.name == "boat_1" || type.name == "boat_0"  ){
        unit = new C_Boat(current);
    }
    else if(type.name == "ArcherTower_0" || type.name == "ArcherTower_1"){
        unit = new C_ArcherTower(current);
    }
    else if(type.name == "barricade_1"){
        unit = new C_Barricade(current);
    }
    else if(type.name == "town_1"){
        unit = new C_Town(current);
    }
    else if(type.name == "Turbine_0"){
        unit = new C_Turbine(current);
    }
    return unit;
}

S_UnitModel C_UnitFactory::extractProperties(string filename){
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
    unit.weapon.speedImpact = tsx.getIntProperty("weapon.speedImpact", 0);
    unit.weapon.fireRate = tsx.getIntProperty("weapon.firerate", 2000);
    unit.weapon.fireRange = tsx.getIntProperty("weapon.firerange", 2);
    unit.weapon.direction = EAST;
    return unit;
}


bool C_UnitFactory::tsxExist(const string &file){
    ifstream tmp(file.c_str());
    return !tmp.fail();
}

//-------------------------------Level Factory --------------------
C_LevelFactory::C_LevelFactory(){
        for(int i = 1; i <= 6; i++){
                m_levelList.push_back(extractInfosFromTmx(i));
        }
}

S_LevelModel C_LevelFactory::extractInfosFromTmx(int levelNbr){
        S_LevelModel level;
        level.nbr = levelNbr;
	C_Settings& settings=C_Settings::Instances();
	level.filename = settings.getLevelFolder() + "Level_" + to_string(levelNbr) + ".tmx";
	C_Message m;
	struct stat buffer;
        if (stat (level.filename.c_str(),  &buffer) == 0){
                C_Xml tmx(level.filename);
                level.width = stoi( tmx.extractStrValue("map","width"));
                level.height = stoi( tmx.extractStrValue( "map", "height"));
                level.gridSize = calcGridSize(level.width,level.height);;
                m.printM("Grid size should be " + to_string(level.gridSize) + "\n");
                level.tilewidth = stoi( tmx.extractStrValue( "map", "tilewidth"));
                level.tileheight = stoi( tmx.extractStrValue( "map", "tileheight"));
                level.backgroundcolor =  tmx.extractStrValue( "map", "backgroundcolor");
                level.name = tmx.extractStrValue("property","name","subname","value");
	}
	else{
    	        m.printM("Can not find " + level.filename+"\n");
	}
	return level;
}

int C_LevelFactory::calcGridSize(int width, int height){
    if(width > height)
        return width;
    else
        return height;
}
