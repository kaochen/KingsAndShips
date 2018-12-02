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
    cout << "create:" << type.name << endl;
    C_GameUnits* unit = nullptr;
    if(type.name == "boat_1_A" || type.name == "boat_0_A"  ){
        S_UnitModel current = m_models[type.name];
        current.coord = type.coord;
        unit = new C_Boat(current);
        //C_GameUnits *boat = new C_ArcherTower(pos.x,pos.y,current.rank);
    }
    else if(type.name == "ArcherTower_0_A" || type.name == "ArcherTower_1_A"){
        S_UnitModel current = m_models[type.name];
        current.coord = type.coord;
        unit = new C_ArcherTower(current);
    }
    return unit;
}
/*  <property name="damage" type="int" value="1"/>
  <property name="firerange" type="int" value="2"/>
  <property name="firerate" type="int" value="1500"/>
  <property name="price" type="int" value="50"/>
  <property name="rank" type="int" value="0"/>
  <property name="speed" type="int" value="2"/>
*/

S_UnitModel C_UnitFactory::extractProperties(string filename){
    C_Xml tsx(filename);
    S_UnitModel unit;
    unit.name = tsx.extractStrValue("tileset","name");
    unit.rank = tsx.getIntProperty("rank", 0);
    //unit.health = stoi(tsx.extractStrValue("property","name","health","value"));
    unit.health = tsx.getIntProperty("health", 100);
    unit.coord = {0,0};
    unit.cost = tsx.getIntProperty("cost", 50);
    unit.speed = tsx.getIntProperty("speed", 1);
    unit.alive = true;

    //S_Weapon
    unit.weapon.damage = tsx.getIntProperty("damage", 2);
    unit.weapon.speedImpact = tsx.getIntProperty("speedImpact", 0);
    unit.weapon.fireRate = tsx.getIntProperty("firerate", 2000);
    unit.weapon.fireRange = tsx.getIntProperty("firerange", 2);
    unit.weapon.direction = EAST;
    return unit;
}


bool C_UnitFactory::tsxExist(const string &file){
    ifstream tmp(file.c_str());
    return !tmp.fail();
}

