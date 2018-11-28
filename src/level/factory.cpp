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

using namespace std;

C_UnitFactory::C_UnitFactory(){

    S_UnitModel unit = extractProperties("data/img/boat_00.tsx");
    m_models[unit.name]= unit;
    unit = extractProperties("data/img/boat_01.tsx");
    m_models[unit.name]= unit;
}
C_GameUnits* C_UnitFactory::create(string type, S_Coord pos){
    cout << "create:" << type << endl;
    C_GameUnits* unit = nullptr;
    if(type == "boat_1_A" || type == "boat_0_A"  ){
        S_UnitModel current = m_models[type];
        current.coord = pos;
        unit = new C_Boat(current);
        //C_GameUnits *boat = new C_ArcherTower(pos.x,pos.y,current.rank);
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
    unit.rank = stoi(tsx.extractStrValue("property","name","rank","value"));
    unit.health = stoi(tsx.extractStrValue("property","name","health","value"));
    unit.coord = {0,0};
    unit.cost = stoi(tsx.extractStrValue("property","name","cost","value"));
    unit.speed = stoi(tsx.extractStrValue("property","name","speed","value"));
    unit.weapon.damage = stoi(tsx.extractStrValue("property","name","damage","value"));
    unit.weapon.fireRate = stoi(tsx.extractStrValue("property","name","firerate","value"));
    unit.weapon.fireRange = stoi(tsx.extractStrValue("property","name","firerange","value"));
    unit.alive = true;
    return unit;
}
